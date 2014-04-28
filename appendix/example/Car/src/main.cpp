#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include <time.h>
#include "car.h"

using namespace std;

//put ID of the wheels here
#define FRONT_RIGHT_WHEEL	1
#define BACK_RIGHT_WHEEL	3
#define FRONT_LEFT_WHEEL	0
#define BACK_LEFT_WHEEL		2


int main(){

	int deviceIndex = 0;
	int baudnum = 1;

	printf("-------CAR TEST PROGRAM-------\n");
	
	///////// Open USB2Dynamixel ////////////
	if( dxl_initialize(deviceIndex, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );


	Car car1(FRONT_RIGHT_WHEEL, FRONT_LEFT_WHEEL, BACK_RIGHT_WHEEL, BACK_LEFT_WHEEL);
	sleep(1);
	
	car1.setSpeed(1023,1);
	sleep(2);
	car1.setSpeed(1023,0);
	sleep(2);
	car1.setSpeed(0,1);
	
		while(1)
		{

	
		}

	// Close device
	car1.setSpeed(0,1);
	dxl_terminate();
	return 0;
}


