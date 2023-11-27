#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

unsigned char location[4][2];
unsigned char positions[3][2] = {{0x01,0x90}, {0x01,0xff}, {0x02,0x6e}};
int depth[] = {0,3,3};


void move_to_location(int connection, unsigned char id, unsigned char loc_h, unsigned char loc_l) {

	unsigned char cs = ~ ( id + 0x07 + 0x03 + 0x1e + loc_l + loc_h + 0x30 + 0x00);

	unsigned char arr[] = { 0xff, 0xff, id, 0x07, 0x03, 0x1e, loc_l, loc_h, 0x30, 0x00, cs };

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,11,buff,buff_len);

}

void wait_until_done(int connection) {
	usleep(2000000);
}

void release(int connection) {
	move_to_location(connection,5,0x01,0xff);
	wait_until_done(connection);
}

void grip(int connection) {
	move_to_location(connection,5,0x01,0x47);
	wait_until_done(connection);
}

void dodge(int connection) {
	move_to_location(connection,2,0x01,0x80);
	wait_until_done(connection);

	move_to_location(connection,1,0x01,0xff);
	wait_until_done(connection);
}

void move_to_pile(int connection, unsigned char motor1[2]) {
  	dodge(connection);

	move_to_location(connection,1,motor1[0],motor1[1]);
	wait_until_done(connection);
	move_to_location(connection,2,0x01,0x40);
	move_to_location(connection,3,0x01,0x90);
	move_to_location(connection,4,0x01,0x01);
	wait_until_done(connection);

	location[0][0] = motor1[0];//TBC
	location[0][1] = motor1[1];
	location[1][0] = 0x01;
	location[1][1] = 0x40;
	location[2][0] = 0x01;
	location[2][1] = 0x90;
	location[3][0] = 0x01;
	location[3][1] = 0x01;
}

void down(int connection){
	move_to_location(connection,2,0x01,(location[1][1]-0x17));
	move_to_location(connection,4,0x01,(location[3][1]+0x15));
	wait_until_done(connection);
	location[1][1] = location[1][1]-0x17;
	location[3][1] = location[3][1]+0x15;
}

void move(int connection, int start, int depthStart, int end, int depthEnd) {
	move_to_pile(connection, positions[start]);
	int i;
	for(i = 0; i<depthStart; i++){down(connection);}
	grip(connection);
	move_to_pile(connection, positions[end]);
	int j;
	for(j = 0; j<depthEnd; j++){down(connection);}
	release(connection);
}

void solve(int connection, int numDisks, int start, int away, int end){
    
    if(numDisks == 0){
        move(connection, start, depth[start]++, end, ((depth[end]--)-1));
    }
    else{
        solve(connection, numDisks-1, start, end, away);
        
        move(connection, start, depth[start]++, end, ((depth[end]--)-1));
        
        solve(connection, numDisks-1, away, start, end);
    }
}

int main(int argc, char* argv[]) {

	int connection = open_connection("/dev/ttyUSB0",B1000000);
	release(connection);
	move_to_pile(connection, positions[0]);
	solve(connection, 2, 0, 1, 2);
	return 0;

}