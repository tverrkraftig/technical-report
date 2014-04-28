#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <time.h>
#include "car.h"
#include "manipulator.h"
#include "json_processing.h"
#include "sensor.h"

using namespace std;

//ID of wheels
#define FRONT_RIGHT_WHEEL	1
#define BACK_RIGHT_WHEEL	3
#define FRONT_LEFT_WHEEL	0
#define BACK_LEFT_WHEEL		2

//ID of manipulator arm
#define MAN_ONE			4		//zero at 511
#define MAN_TWO			7		//zero at 511, not allowed to go under
#define MAN_THREE		5		//zero at 511

//ID of gripper
#define GRIPPER_LEFT		12
#define GRIPPER_RIGHT		6

//ID of sensor
#define SENSOR			100

void *sendSensorData(void *ptr);

int main(){

	pthread_t thread1;
	int deviceIndex = 0;
	int baudnum = 1;
	string command;
	vector <string> commands;
	string strCheck = "position";

	printf("-------MAIN PROGRAM-------\n");
	
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
	Manipulator manipulator1(MAN_ONE, MAN_TWO, MAN_THREE, GRIPPER_LEFT, GRIPPER_RIGHT);
	Sensor sensor1(SENSOR);
	sleep(1);
	
	sensor1.playMelody(6);
	manipulator1.goToPosition(XSTART,YSTART,ZSTART);
	manipulator1.setGripper(0);
	
	//get old commands from server and disregard them
	vector <string> dummy = json_get_commands(0);

	//create thread for sending sensor data
	pthread_create( &thread1, NULL, sendSensorData, &sensor1 );
	
		while(1)
		{

				//get commands
				while(commands.empty())
				{
					commands = json_get_commands(0);
				}
				
				//execute commands
				while(!commands.empty())
				{
					command = commands.front();
					commands.erase(commands.begin());
					if(command == "forward")
						car1.setSpeed(1023,1);
						
					else if(command == "backward")
						car1.setSpeed(1023,0);
						
					else if(command == "stop")
						car1.setSpeed(0,1);
						
					else if(command == "leftTurn")
						car1.turnCar(LEFT_TURN);
						
					else if(command == "rightTurn")
						car1.turnCar(RIGHT_TURN);
						
					else if(command == "noTurn")
						car1.turnCar(NO_TURN);
						
					else if(command == "gripClose")
						manipulator1.setGripper(1);
						
					else if(command == "gripOpen")
						manipulator1.setGripper(0);
						
					else if(command.find(strCheck) != string::npos){
						size_t found1 = command.find(" ");
						size_t found2 = command.find(" ", found1+1);
						size_t found3 = command.find(" ", found2+1);
						string nr1 = command.substr(found1+1, found2-found1);
						string nr2 = command.substr(found2+1, found3-found2);
						string nr3 = command.substr(found3+1);

						int x = atoi(nr1.c_str());
						int y = atoi(nr2.c_str());
						int z = atoi(nr3.c_str());
						manipulator1.goToPosition(x, y, z);
					}
					
					else
						printf("Unknown command\n");

					printf("command: %s\n", command.c_str());
				}

	
		}

	
	
	// Close device
	car1.setSpeed(0,1);
	dxl_terminate();
	return 0;
}

//thread function for continously sending data
void *sendSensorData(void *ptr){

	//initialize sensor here?
	Sensor* p = (Sensor*)ptr;
	int data;
	map <string,double> sensorData;
	while(1){
		//sleep for 100ms
		sleep(1);
		
		if(p->getMode() == FAILSAFE_MODE)
		{
			p->ping();
			continue;
		}
		//get data and put it in the map
		data = p->getIR(CENTER);
		printf("\nIR center: %d\n",data);
		sensorData["IR center"] = data;
		
		data = p->getIR(LEFT);
		printf("IR left: %d\n",data);
		sensorData["IR left"] = data;

		data = p->getIR(RIGHT);
		printf("IR right: %d\n",data);
		sensorData["IR right"] = data;
		//send data
		json_send_data(sensorData);
		//clear map
		sensorData.clear();
	}
	return NULL;
}


