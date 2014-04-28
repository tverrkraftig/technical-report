#include "motor.h"
#include "dynamixel.h"
#include "stdio.h"
#include "communication.h"

Motor::Motor(int theID, int theMode){
	ID = theID;
	mode = theMode;
	commStatus = COMM_RXSUCCESS;
	setMode(mode);	
}

int Motor::getMode(){
	return mode;
}

int Motor::getPosition(){
		
	int temp = readWord( ID, PRESENT_POSITION_L );
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
		throw MotorException(ID,commStatus);
	printErrorCode();
	position = temp;
	return position;
}

int Motor::getSpeed(){
		
	unsigned short temp = readWord( ID, PRESENT_SPEED_L );
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
		throw MotorException(ID,commStatus);
	printErrorCode();
	speed = temp & 1023;
	return speed;
}

void Motor::setGoalPosition(int thePosition){
		
	writeWord( ID, GOAL_POSITION_L, thePosition );
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
		throw MotorException(ID,commStatus);
	printErrorCode();
}


void Motor::setMode(int theMode){
		
	switch(theMode)
	{
	case WHEELMODE:
		writeWord( ID, CW_ANGLE_LIMIT_L, 0 );
		writeWord( ID, CCW_ANGLE_LIMIT_L, 0 );
		break;
	case SERVOMODE:
		writeWord( ID, CW_ANGLE_LIMIT_L, 0 );
		writeWord( ID, CCW_ANGLE_LIMIT_L, 1023 );
		break;
	default:
		printf("unknown mode: %d\n", theMode);
		return;
	}
	mode = theMode;
}

void Motor::setSpeed(int theSpeed, bool theDirection){
		
	writeWord( ID, MOVING_SPEED_L, theSpeed | (theDirection<<10) );
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
		throw MotorException(ID,commStatus);
	printErrorCode();
}

void Motor::setRotateDirection(int direction){
		
	switch(direction)
	{
	case CW:
		writeWord(ID, MOVING_SPEED_L, 1024);
		break;
	case CCW:
		writeWord(ID, MOVING_SPEED_L, 0);
		break;
	default:
		printf("invalid input: %d\n", direction);
		return;
	}
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
		throw MotorException(ID,commStatus);
	printErrorCode();
	
	rotateDirection = direction;
}

// Print error bit of status packet
void Motor::printErrorCode()
{
	if(getRXpacketError(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(getRXpacketError(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(getRXpacketError(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(getRXpacketError(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(getRXpacketError(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(getRXpacketError(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(getRXpacketError(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}

void Motor::checkStatus(){
		
	unsigned char temp;
	for(int i = 0; i<50; i++)
	{
		if(i == 10 || i == 45)
			continue;
		temp = readByte( ID, i );
		printf("%d:\t%d\t%d\n", ID, i, temp);
	}
	printf("\n");
}

int Motor::ping(){
	pingID(ID);
	commStatus = getResult();
	if( commStatus == COMM_RXSUCCESS )
	{
		//printf("Motor ID: %d active!\n",ID);
		return 1;
	}
	//printf("Motor ID: %d NOT active!\n",ID);
	return 0;
}

void pingAll(){
	for(int i = 0; i<254; i++){
		dxl_ping(i);
		if( dxl_get_result( ) == COMM_RXSUCCESS )
		{
			printf("ID: %d active!\n",i);
		}
	}
}

void printError(int status){
	switch(status)
	{
	case COMM_TXFAIL:

		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

