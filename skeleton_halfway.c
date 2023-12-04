#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

unsigned char location[4][2];

void move_to_location(int connection, unsigned char id, unsigned char loc_h, unsigned char loc_l) {

	unsigned char cs = ~ ( id + 0x07 + 0x03 + 0x1e + loc_l + loc_h + 0x30 + 0x00);

	unsigned char arr[] = { 0xff, 0xff, id, 0x07, 0x03, 0x1e, loc_l, loc_h, 0x30, 0x00, cs };

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,11,buff,buff_len);

}

int readMove(int connection, int id){
	unsigned char cs = ~ ( id + 0x04 + 0x02 + 0x2e + 0x01);

	unsigned char arr[] = { 0xff, 0xff, id, 0x04, 0x02, 0x2e, 0x01, cs};

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,8,buff,buff_len);

	return (int) buff[5];
}

void readSpeed(int connection, int id){
	unsigned char cs = ~ ( id + 0x04 + 0x02 + 0x20 + 0x01);

	unsigned char arr[] = { 0xff, 0xff, id, 0x04, 0x02, 0x20, 0x01, cs};

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,8,buff,buff_len);

	printf("%d", ((int) buff[5]));
}

void wait_until_done(int connection) {
	usleep(2000000);
}
/*
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

void position1(int connection) {
	dodge(connection);
	move_to_location(connection,1,0x01,0x90);
	move_to_location(connection,2,0x01,0x40);
	move_to_location(connection,3,0x01,0x90);
	move_to_location(connection,4,0x01,0x01);
	wait_until_done(connection);
	location[0][0] = 0x01;
	location[0][1] = 0x90;
	location[1][0] = 0x01;
	location[1][1] = 0x40;
	location[2][0] = 0x01;
	location[2][1] = 0x90;
	location[3][0] = 0x01;
	location[3][1] = 0x01;
}

void position2(int connection) {
	dodge(connection);
	move_to_location(connection,1,0x01,0xff);
	move_to_location(connection,2,0x01,0x40);
	move_to_location(connection,3,0x01,0x90);
	move_to_location(connection,4,0x01,0x01);
	wait_until_done(connection);
}

void position3(int connection) {
	dodge(connection);
	move_to_location(connection,1,0x02,0x6e);
	move_to_location(connection,2,0x01,0x40);
	move_to_location(connection,3,0x01,0x90);
	move_to_location(connection,4,0x01,0x01);
	wait_until_done(connection);
}

void down(int connection){
	move_to_location(connection,2,0x01,(location[1][1]-0x17));
	move_to_location(connection,4,0x01,(location[3][1]+0x15));
	wait_until_done(connection);
	location[1][1] = location[1][1]-0x17;
	location[3][1] = location[3][1]+0x15;
}

void move(int start, int depthStart, int end, int depthEnd) {
	release(connection);
	
}*/

int main(int argc, char* argv[]) {

	int connection = open_connection("/dev/ttyUSB0",B1000000);

	readSpeed(connection, 1);
	return 0;

}

