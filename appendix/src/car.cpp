#include "car.h"
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex_car = PTHREAD_MUTEX_INITIALIZER;

void Car::setSpeed(int theSpeed, bool dir){

	if(getMode() == FAILSAFE_MODE)
		return;
	
	try{
		switch(turn)
		{
		case NO_TURN:
			//set all wheels same speed
			frontLeftWheel.setSpeed(theSpeed, !dir);
			backLeftWheel.setSpeed(theSpeed, !dir);
			frontRightWheel.setSpeed(theSpeed, dir);
			backRightWheel.setSpeed(theSpeed, dir);	
			break;
		case LEFT_TURN:
			//set left wheels TURN_MAGNITUDE of right wheels
			frontLeftWheel.setSpeed(theSpeed*TURN_MAGNITUDE, !dir);
			backLeftWheel.setSpeed(theSpeed*TURN_MAGNITUDE, !dir);
			frontRightWheel.setSpeed(theSpeed, dir);
			backRightWheel.setSpeed(theSpeed, dir);
			break;
		case RIGHT_TURN:
			//set right wheels TURN_MAGNITUDE of left wheels
			frontLeftWheel.setSpeed(theSpeed, !dir);
			backLeftWheel.setSpeed(theSpeed, !dir);
			frontRightWheel.setSpeed(theSpeed*TURN_MAGNITUDE, dir);
			backRightWheel.setSpeed(theSpeed*TURN_MAGNITUDE, dir);
			break;
		}
		speed = theSpeed;
		direction = dir;
	}
	catch(MotorException e) {
		printf("ID: %d lost\n",e.ID);
		printError(e.status);
		setMode(FAILSAFE_MODE);
		printf("Wheels lost!\n");
		startPing();
	}
	
}

void Car::turnCar(int theTurn){

	if(getMode() == FAILSAFE_MODE)
		return;

	try{
		turn = theTurn;
		if(speed != 0){
			setSpeed(speed, direction);
			return;
		}
		if(turn == NO_TURN){
			setSpeed(0,1);
			return;
		}
		bool dir;
		if(turn == LEFT_TURN)
			dir = 1;
		if(turn == RIGHT_TURN)
			dir = 0;

		printf("direction %d\n",direction);
		frontLeftWheel.setSpeed(1023*TURN_MAGNITUDE, dir);
		backLeftWheel.setSpeed(1023*TURN_MAGNITUDE, dir);
		frontRightWheel.setSpeed(1023*TURN_MAGNITUDE, dir);
		backRightWheel.setSpeed(1023*TURN_MAGNITUDE, dir);
	}
	catch(MotorException e) {
		printf("ID: %d lost\n",e.ID);
		printError(e.status);
		setMode(FAILSAFE_MODE);
		printf("Wheels lost!\n");
		startPing();
	}
	
}

void Car::setMode(int theMode){
	pthread_mutex_lock( &mutex_car );
	mode = theMode;
	pthread_mutex_unlock( &mutex_car );
}

int Car::getMode(){
	pthread_mutex_lock( &mutex_car );
	int temp = mode;
	pthread_mutex_unlock( &mutex_car );
	return temp;
}

void Car::ping(){
	printf("Ping Car\n");
	while(1){
		int count = 0;
		count += frontLeftWheel.ping();
		count += backLeftWheel.ping();
		count += frontRightWheel.ping();
		count += backRightWheel.ping();
	
		if(count == 4){
			printf("All wheels active!\n");
			setMode(IDLE_MODE);
			return;
		}
	}
}

void Car::startPing(){
	pthread_create(&thread_car, NULL, Car::staticEntryPoint, this);
}

void * Car::staticEntryPoint(void * c)
{
    ((Car *) c)->ping();
    return NULL;
}



