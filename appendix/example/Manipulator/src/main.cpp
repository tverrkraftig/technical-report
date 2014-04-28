#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include <time.h>
#include "manipulator.h"

using namespace std;

#define MAN_ONE			4		//zero at 511
#define MAN_TWO			7		//zero at 511, not allowed to go under
#define MAN_THREE		5		//zero at 511

#define GRIPPER_LEFT		12
#define GRIPPER_RIGHT		6

int main(){
	
	int deviceIndex = 0;
	int baudnum = 1;

	printf("-------MANIPULATOR TEST PROGRAM-------\n");
	
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
	
	Manipulator manipulator1(MAN_ONE, MAN_TWO, MAN_THREE, GRIPPER_LEFT, GRIPPER_RIGHT);
	sleep(1);
	
	manipulator1.setGripper(0);

	//test drawing
//	manipulator1.setGripper(1);
//	manipulator1.drawLine(50,200,50,150,0);
//	manipulator1.drawLine(50,175,25,175,0);
//	manipulator1.drawLine(25,200,25,150,0);
	
	while(1)
	{

		for(int i = 0; i < 130; i+=1)
		{
			manipulator1.goToPosition(0,170,i);
			usleep(5000);
		}
		for(int i = 130; i > 0; i-=1)
		{
			manipulator1.goToPosition(0,170,i);
			usleep(5000);
		}
		
		for(int i = 0; i < 100; i+=1)
		{
			manipulator1.goToPosition(i,170,0);
			usleep(5000);
		}
		for(int i = 100; i > -100; i-=1)
		{
			manipulator1.goToPosition(i,170,0);
			usleep(5000);
		}
		for(int i = -100; i < 0; i+=1)
		{
			manipulator1.goToPosition(i,170,0);
			usleep(5000);
		}

	}

	
	
	// Close device
	dxl_terminate();
	return 0;
}

