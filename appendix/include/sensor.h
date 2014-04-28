#ifndef SENSOR_H_
#define SENSOR_H_

#include <dynamixel.h>

//control table adress
#define IR_LEFT_FIRE_DATA	26
#define IR_CENTER_FIRE_DATA	27
#define IR_RIGHT_FIRE_DATA	28
#define LIGHT_LEFT_DATA		29
#define LIGHT_CENTER_DATA	30
#define LIGHT_RIGHT_DATA	31
#define IR_OBSTACLE_DETECTED	32
#define LIGHT_DETECTED		33
#define SOUND_DATA		35
#define BUZZER_DATA_NOTE	40
#define BUZZER_DATA_TIME	41

#define LEFT			0
#define CENTER			1
#define RIGHT			2

/*melody:
	0: Rising
	1: Falling
	2: Fight
	4: Fail
	5: sad
	6: bip bip
	7: sad 2
	10: whistle rise
	11: bip bop
	15: bip bip 2
	16: phone
	21: whistle
	24: rtrtrrtrt
*/

class Sensor{
public:
	Sensor(int);
	int getIR(int);
	int getLight(int);	//only infrared light
	void playMelody(int);	//input range 0-26
	void playMelody(unsigned char*, int);	//play from arrays in songs.h
	void ping();
	void setMode(int);
	int getMode();
private:
	int ID;
	int commStatus;
	int mode;
};

#endif
