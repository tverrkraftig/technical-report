#ifndef MOTOR_H_
#define MOTOR_H_

#include <dynamixel.h>
#include <pthread.h>

// Control table address
#define CW_ANGLE_LIMIT_L	6
#define CW_ANGLE_LIMIT_H	7
#define CCW_ANGLE_LIMIT_L	8
#define CCW_ANGLE_LIMIT_H	9
#define MAX_TORQUE_L		14
#define MAX_TORQUE_H		15
#define HIGH_LIMIT_VOLTAGE	13
#define GOAL_POSITION_L		30
#define GOAL_POSITION_H		31
#define MOVING_SPEED_L		32
#define MOVING_SPEED_H		33
#define PRESENT_POSITION_L	36
#define PRESENT_POSITION_H	37
#define PRESENT_SPEED_L		38
#define PRESENT_SPEED_H		39
#define MOVING			46

#define WHEELMODE		0
#define SERVOMODE		1

#define CW			1
#define CCW			0

#define IDLE_MODE		0
#define FAILSAFE_MODE		1


class MotorException{
public:
	MotorException(int theID, int theStatus) : ID(theID), status(theStatus){};
	int ID;
	int status;
};

class Motor{
public:
	Motor(int, int);	
	int getMode();
	int getPosition();
	int getSpeed();
	void setGoalPosition(int);
	void setSpeed(int,bool);
	void setMode(int);
	void setRotateDirection(int);
	void printErrorCode(void);
	void checkStatus();
	int ping();
private:
	int position;
	int speed;
	int mode;
	int ID;
	int commStatus;
	int rotateDirection;
};

void pingAll();
void printError(int status);

#endif
