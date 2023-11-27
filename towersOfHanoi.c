#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

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

void open(int connection) {
	move_to_location(connection,5,0x01,0xff);
	wait_until_done(connection);
}

void close(int connection) {
	move_to_location(connection,5,0x01,0xff);
	wait_until_done(connection);
}

void move_to_position(int connection, unsigned char positions[4][2]) {//WIll move all actuators to the top positions of the pile (make sure the order is good)
  //I think we should always start by turning motor 2 up slightly so it does not crash into other blocks

  //Then move to position
}

int main(int argc, char* argv[]) {

	int connection = open_connection("/dev/ttyUSB0",B1000000);

	initialise(connection);//Initialises the robot arm to position 1 (may be removable)

	unsigned char positions[8][4][2] = {
    { {},{},{},{} }//The 4 motor positions for the top of pile 1
    { {},{},{},{} }//The 4 motor positions for the middle of pile 1
    { {},{},{},{} }//The 4 motor positions for the bottom of pile 1

    { {},{},{},{} }//The 4 motor positions for the top of pile 2
    { {},{},{},{} }//The 4 motor positions for the bottom of pile 2
  
    { {},{},{},{} }//The 4 motor positions for the top of pile 3
    { {},{},{},{} }//The 4 motor positions for the middle of pile 3
    { {},{},{},{} }//The 4 motor positions for the bottom of pile 3
  }
	
	return 0;

}
