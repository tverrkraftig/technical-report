#include "motor.h"
#include "sensor.h"
#include "stdio.h"
#include <unistd.h>
#include "communication.h"

Sensor::Sensor(int theID){
	ID = theID;
	commStatus = COMM_RXSUCCESS;
	mode = IDLE_MODE;
}

int Sensor::getLight(int pos){
		
	int data = readByte( ID, LIGHT_LEFT_DATA + pos );
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
	{
		mode = FAILSAFE_MODE;
		printf("sensor lost\n");
	}	
	return data;
}

int Sensor::getIR(int pos){
		
	int data = readByte( ID, IR_LEFT_FIRE_DATA + pos );
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
	{
		mode = FAILSAFE_MODE;
		printf("sensor lost\n");
	}
		
		
	return data;
}

void Sensor::playMelody(int song){

	if(song < 0 || song > 26){
		printf("invalid input\n");
		return;
	}
	writeByte(ID, BUZZER_DATA_TIME, 255);
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
	{
		mode = FAILSAFE_MODE;
		printf("sensor lost\n");
	}
	writeByte(ID, BUZZER_DATA_NOTE, song);
	commStatus = getResult();
	if(commStatus != COMM_RXSUCCESS)
	{
		mode = FAILSAFE_MODE;
		printf("sensor lost\n");
	}
}

void Sensor::playMelody(unsigned char* song, int length){

	
	
	for(int i = 0; i<length; i+=2)
	{

		if(song[i+1] != 100)
		{
			writeByte(ID, BUZZER_DATA_TIME, 254);
			writeByte(ID, BUZZER_DATA_NOTE, song[i+1]);
			usleep(40000*song[i]);
		}
		else
		{
			writeByte(ID, BUZZER_DATA_TIME, 0);
			usleep(40000*song[i]);
		}
			
		
	}
	writeByte(ID, BUZZER_DATA_TIME, 0);
	
}

void Sensor::ping(){
	pingID(ID);
	commStatus = getResult();
	if( commStatus == COMM_RXSUCCESS )
	{
		printf("Sensor ID: %d active!\n",ID);
		setMode(IDLE_MODE);
	}
	else{
		setMode(FAILSAFE_MODE);
	}
}

void Sensor::setMode(int theMode){
	mode = theMode;
}

int Sensor::getMode(){
	return mode;
}

