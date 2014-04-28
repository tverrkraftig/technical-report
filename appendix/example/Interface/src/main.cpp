#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include <time.h>
#include "car.h"
#include "manipulator.h"
#include "interface.h"

using namespace std;

//put ID of the wheels here
#define FRONT_RIGHT_WHEEL	1
#define BACK_RIGHT_WHEEL	3
#define FRONT_LEFT_WHEEL	0
#define BACK_LEFT_WHEEL		2

#define MAN_ONE			4		//zero at 511
#define MAN_TWO			7		//zero at 511, not allowed to go under
#define MAN_THREE		5		//zero at 511

#define GRIPPER_LEFT		12
#define GRIPPER_RIGHT		6

int main(){

	int deviceIndex = 0;
	int baudnum = 1;

	printf("-------LOCAL INTERFACE TEST PROGRAM-------\n");
	
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
	
	windowInit();
	Car car1(FRONT_RIGHT_WHEEL, FRONT_LEFT_WHEEL, BACK_RIGHT_WHEEL, BACK_LEFT_WHEEL);
	Manipulator manipulator1(MAN_ONE, MAN_TWO, MAN_THREE, GRIPPER_LEFT, GRIPPER_RIGHT);
	sleep(1);
	
	manipulator1.goToPosition(XSTART,YSTART,ZSTART);
	manipulator1.setGripper(0);
	
		while(1)
		{
		
			checkEvent(&manipulator1, &car1);

		}

	
	
	// Close device
	car1.setSpeed(0,1);
	dxl_terminate();
	return 0;
}


