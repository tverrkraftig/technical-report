#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "manipulator.h"

using namespace std;

#define D2	77	//length of first arm in mm
#define D3	155	//length of second arm in mm

#define ANGLE_TO_VALUE	(float)511*6/(5*PI)

#define GRIPPER_LEFT_ZERO	511-140
#define GRIPPER_RIGHT_ZERO	511+140
#define MAX_COUNT		5

pthread_mutex_t mutex_man = PTHREAD_MUTEX_INITIALIZER;

void Manipulator::goToPosition(int x, int y, int z){

	//return error if beyond max
//	if((x*x+y*y+z*z) > (D2+D3)*(D2+D3))
//	{
//		printf("invalid position!\n");
//		return;
//	}

	if(getMode() == FAILSAFE_MODE)
		return;

	float s3, c3, l;
	
	l = sqrt(x*x+y*y);
	c3 = (z*z + l*l - D2*D2 - D3*D3)/(2*D2*D3);
	s3 = sqrt(1-c3*c3);
	
	theta3 = atan2(s3,c3);
	theta2 = PI/2 - atan2(D3*s3, D2+D3*c3)-atan2(z,l);
	theta1 = atan2(x,y);

	setAngles(theta1, theta2, theta3);
}

void Manipulator::setAngles(float t1, float t2, float t3){

	if(getMode() == FAILSAFE_MODE)
		return;

	try{
		int dummy;

		if(t1 != t1)
			printf("nan theta 1\n");
		else if(t1 > 5*PI/6){
			one.setGoalPosition(1023);
			printf("Theta 1 too high\n");
		}	
		else if(t1 < -5*PI/6){
			one.setGoalPosition(0);
			printf("Theta 1 too low\n");
		}	
		else{
			dummy = (float)(t1*ANGLE_TO_VALUE+511);
			one.setGoalPosition(dummy);
			//printf("one: %d\n",dummy);
		}

		if(t2 != t2)
			printf("nan theta 2\n");
		else if(t2 > 5*PI/6){
			two.setGoalPosition(1023);
			printf("Theta 2 too high\n");
		}	
		else if(t2 < 0){
			two.setGoalPosition(511);
			printf("Theta 2 too low\n");	
		}	
		else{
			dummy = (float)(t2*ANGLE_TO_VALUE+511);
			two.setGoalPosition(dummy);
			//printf("two: %d\n",dummy);
		}

		if(t3 != t3)
			printf("nan theta 3\n");
		else if(t3 > 0.78*PI){
			three.setGoalPosition(989);
			printf("Theta 3 too high\n");
		}	
		else if(t3 < -0.5*PI){
			three.setGoalPosition(51);
			printf("Theta 3 too low\n");
		}	
		else{
			dummy = (float)(t3*ANGLE_TO_VALUE+511);
			three.setGoalPosition(dummy);
			//printf("three: %d\n",dummy);
		}	
	}
	catch(MotorException e) {
		printf("ID: %d lost\n",e.ID);
		printError(e.status);
		setMode(FAILSAFE_MODE);
		printf("Manipulator lost!\n");
		startPing();
	}
}

void Manipulator::setGripper(bool on){

	if(getMode() == FAILSAFE_MODE)
		return;

	try{
		if(!on){
			grip_left.setGoalPosition(511-50);
			grip_right.setGoalPosition(511+50);
			return;
		}

		int positionL, positionR, lastPositionL, lastPositionR;
		int counter = 0;
		//put servo set point to zero degrees
		grip_left.setGoalPosition(GRIPPER_LEFT_ZERO);
		grip_right.setGoalPosition(GRIPPER_RIGHT_ZERO);
		lastPositionR = grip_right.getPosition();
		lastPositionL = grip_left.getPosition();
		while(1){
			positionL = grip_left.getPosition();
			positionR = grip_right.getPosition();
			printf("left: %d\tright: %d\n",positionL,positionR);
		
			if(lastPositionL == positionL || lastPositionR == positionR)
				counter++;
			else
				counter = 0;
			if(counter == MAX_COUNT)
				return;
			lastPositionL = positionL;
			lastPositionR = positionR;
			usleep(10000);
		}
	}
	catch(MotorException e) {
		printf("ID: %d lost\n",e.ID);
		printError(e.status);
		setMode(FAILSAFE_MODE);
		printf("Manipulator lost!\n");
		startPing();
	}
}

void Manipulator::drawLine(int xstart, int ystart, int xend, int yend, int z){

	if(getMode() == FAILSAFE_MODE)
		return;

	try{
		goToPosition(xstart,ystart,z+50);
		sleep(1);
		goToPosition(xstart,ystart,z);
		usleep(100000);
		int x = xend-xstart;
		int y = yend-ystart;
		int length = sqrt(x*x+y*y);
		x /= length;	//normalize
		y /= length;	//normalize
		for(int i = 0; i<length; i++){
			printf("x: %d\ty: %d\n",xstart+i*x, ystart+i*y);
			goToPosition(xstart+i*x, ystart+i*y, z);
			usleep(10000);
		}
	}
	catch(MotorException e) {
		printf("ID: %d lost\n",e.ID);
		printError(e.status);
		setMode(FAILSAFE_MODE);
		printf("Manipulator lost!\n");
		startPing();
	}
}

void Manipulator::drawCircle(int xcenter, int ycenter, int z, int radius, float startAngle, float endAngle){

	if(getMode() == FAILSAFE_MODE)
		return;

	try{
		float t = startAngle;
		float stepSize = 0.01;
		while(t <= endAngle){
			goToPosition(radius*sin(t) + xcenter, radius*cos(t) + ycenter, z);
			t += stepSize;
			usleep(10000);
		}
	}
	catch(MotorException e) {
		printf("ID: %d lost\n",e.ID);
		printError(e.status);
		setMode(FAILSAFE_MODE);
		printf("Manipulator lost!\n");
		startPing();
		
	}
}

void Manipulator::setMode(int theMode){
	pthread_mutex_lock( &mutex_man );
	mode = theMode;
	pthread_mutex_unlock( &mutex_man );
}
int Manipulator::getMode(){
	pthread_mutex_lock( &mutex_man );
	int temp = mode;
	pthread_mutex_unlock( &mutex_man );
	return temp;
}

void Manipulator::ping(){
	printf("Ping Manipulators\n");
	while(1){
		int count = 0;
		count += one.ping();
		count += two.ping();
		count += three.ping();
		count += grip_left.ping();
		count += grip_right.ping();

		if(count == 5){
			printf("All manipulator motors active!\n");
			setMode(IDLE_MODE);
			//printf("Returning to start position\n");
			//goToPosition(XSTART,YSTART,ZSTART);
			//setGripper(0);
			return;
		}
	}
}

void Manipulator::startPing(){
	
	pthread_create(&thread, NULL, Manipulator::staticEntryPoint, this);
}

void * Manipulator::staticEntryPoint(void * c)
{
    ((Manipulator *) c)->ping();
    return NULL;
}

