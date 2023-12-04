#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

unsigned char location[4][2];//Stores the current location of the arm
unsigned char positions[3][2] = {{0x01,0x90}, {0x01,0xff}, {0x02,0x6e}};//Stores the locations of the 3 piles
int depth[] = {0,3,3};//Stores how low the arm should go for each move

//Creates and sends a package to the robot arm telling it to move to a specified location
void move_to_location(int connection, unsigned char id, unsigned char loc_h, unsigned char loc_l) {

	unsigned char cs = ~ ( id + 0x07 + 0x03 + 0x1e + loc_l + loc_h + 0x30 + 0x00);//Create the checksum

	unsigned char arr[] = { 0xff, 0xff, id, 0x07, 0x03, 0x1e, loc_l, loc_h, 0x30, 0x00, cs };//Create the package

	int buff_len = 100;
	unsigned char buff[buff_len];//Create the buffer location

	int bytes_read = write_to_connection(connection,arr,11,buff,buff_len);//Send the package

}


int isMoving(int connection, int id){
	unsigned char cs = ~ ( id + 0x04 + 0x02 + 0x2e + 0x01);

	unsigned char arr[] = { 0xff, 0xff, id, 0x04, 0x02, 0x2e, 0x01, cs};

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,8,buff,buff_len);

	return (int) buff[5];
}

//Delay the code to allow time for the arm to reach its location
void wait_until_done(int connection, int id) {
	while(isMoving(connection, id)){
		usleep(5000);
	}
}

//Opens the claw
void release(int connection) {
	move_to_location(connection,5,0x01,0xff);
	wait_until_done(connection, 5);
}

//Closes the claw
void grip(int connection) {
	move_to_location(connection,5,0x01,0x47);
	wait_until_done(connection, 5);
}

//Moves the arm up and to a central position between moves so it does not knock any towers over
void dodge(int connection) {
	//Move up
	move_to_location(connection,2,0x01,0x80);
	wait_until_done(connection, 2);

	//Turn to the middle
	move_to_location(connection,1,0x01,0xff);
	wait_until_done(connection, 1);
}

//Moves the arm over a certain pile
void move_to_pile(int connection, unsigned char motor1[2]) {
  	dodge(connection);//Move out the way of any piles to avoid knocking them

	//Update the registers to be at the top of a certain pile
	move_to_location(connection,1,motor1[0],motor1[1]);
	wait_until_done(connection, 1);
	move_to_location(connection,2,0x01,0x40);
	move_to_location(connection,3,0x01,0x90);
	move_to_location(connection,4,0x01,0x01);
	wait_until_done(connection, 4); //Might need to be changed

	//Update the robots current position
	location[0][0] = motor1[0];
	location[0][1] = motor1[1];
	location[1][0] = 0x01;
	location[1][1] = 0x40;
	location[2][0] = 0x01;
	location[2][1] = 0x90;
	location[3][0] = 0x01;
	location[3][1] = 0x01;
}

//Move the robot claw one block down a certain pile and update its position store
void down(int connection){
	move_to_location(connection,2,0x01,(location[1][1]-0x17));
	move_to_location(connection,4,0x01,(location[3][1]+0x15));
	wait_until_done(connection, 4); //Might need to be changed
	location[1][1] = location[1][1]-0x17;
	location[3][1] = location[3][1]+0x15;
}

//Perform one move in the algorithm from a start position to an end position
void move(int connection, int start, int depthStart, int end, int depthEnd) {
	move_to_pile(connection, positions[start]);//Go to the top of the start pile
	
	//Move down the pile to the required block
	int i;
	for(i = 0; i<depthStart; i++){down(connection);}
	grip(connection);//Grab the block
	
	move_to_pile(connection, positions[end]);//Go to the top of the end pile

	//Move down the pile to the required position
	int j;
	for(j = 0; j<depthEnd; j++){down(connection);}
	release(connection);//Release the block
}

//Implement a recursive algorithm to solve the towers of hanoi
void solve(int connection, int numBlocks, int start, int away, int end){
    
    if(numBlocks == 0){
        move(connection, start, depth[start]++, end, ((depth[end]--)-1));
    }
    else{
        solve(connection, numBlocks-1, start, end, away);
        
        move(connection, start, depth[start]++, end, ((depth[end]--)-1));
        
        solve(connection, numBlocks-1, away, start, end);
    }
}

int main(int argc, char* argv[]) {

	int connection = open_connection("/dev/ttyUSB0",B1000000);//Connect to the arm
	
	//Initialise the arm's position
	release(connection);
	move_to_pile(connection, positions[0]);
	
	solve(connection, 2, 0, 1, 2);//Solve the problem
	close_connection(connection);

	return 0;

}
