#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include "sensor.h"
#include "songs.h"


using namespace std;

#define SENSOR		100

int main(){

	int deviceIndex = 0;
	int baudnum = 1;

	printf("-------Sensor TEST PROGRAM-------\n");

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


	Sensor sensor1(SENSOR);
	sensor1.playMelody(FurElise,sizeof(FurElise));
	//sensor1.playMelody(Sirene,sizeof(Sirene));
	//sensor1.playMelody(6);
	
	while(1)
	{

	}

	// Close device
	dxl_terminate();
	return 0;
}

