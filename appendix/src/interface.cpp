#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "manipulator.h"

#define KEYMASK ButtonPressMask | KeyPressMask | KeyReleaseMask | ButtonReleaseMask | PointerMotionMask

#define FORWARD 25
#define BACKWARD 39
#define LEFT 38
#define RIGHT 40

#define LEFT_MOUSE_BUTTON 	1
#define RIGHT_MOUSE_BUTTON 	3
#define MOUSE_WHEEL		2
#define MOUSE_WHEEL_FORWARD	4
#define MOUSE_WHEEL_BACKWARD	5

Display *display;
Window window;
XEvent event;
bool button = 0;
bool buttonR = 0;
int xpos = XSTART;
int ypos = YSTART;
int zpos = ZSTART;
int xzero = 0;
int yzero = 0;
 
void windowInit()
{
    int s;
    /* open connection with the server */
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
 
    s = DefaultScreen(display);
 
    /* create window */
    window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 500, 500, 1,
                           BlackPixel(display, s), WhitePixel(display, s));
 
    /* select kind of events we are interested in */
    XSelectInput(display, window, KEYMASK);
 
    /* map (show) the window */
    XMapWindow(display, window);

    //do not detect autorepeating events from keyboard
    XAutoRepeatOff(display);
    printf("Display open\n");
}
void checkEvent(Manipulator *man, Car *car){
        XNextEvent (display, & event);
        switch(event.type){
		case MotionNotify:
			if(button){
				xpos -= event.xmotion.x - xzero;
				ypos -= event.xmotion.y - yzero;
				xzero = event.xmotion.x;
				yzero = event.xmotion.y;
				//printf("xpos: %d\t ypos: %d\n", xpos, ypos);
				man->goToPosition(xpos,ypos,zpos);
			}	
			break;
		case ButtonPress:
			if(event.xkey.keycode == LEFT_MOUSE_BUTTON)
			{
				button = 1;
				xzero = event.xbutton.x;
				yzero = event.xbutton.y;
			}
			if(event.xkey.keycode == RIGHT_MOUSE_BUTTON)
			{
				buttonR ^= 1;
				man->setGripper(buttonR);
			}
			if(event.xkey.keycode == MOUSE_WHEEL_FORWARD)
			{
				zpos-=10;
				man->goToPosition(xpos,ypos,zpos);
			}
			if(event.xkey.keycode == MOUSE_WHEEL_BACKWARD)
			{
				zpos+=10;
				man->goToPosition(xpos,ypos,zpos);
			}
			
			printf( "KeyPress: %d\n", event.xkey.keycode );
			break;
		case ButtonRelease:
			if(event.xkey.keycode == LEFT_MOUSE_BUTTON)
				button = 0;
			break;
		case KeyPress:
			//printf( "KeyPress: %d\n", e.xkey.keycode );
			switch(event.xkey.keycode){
				case FORWARD:
					printf("forward\n");
					car->setSpeed(1023,1);
					break;
				case BACKWARD:
					car->setSpeed(1023,0);
					printf("backward\n");
					break;
				case RIGHT:
					car->turnCar(RIGHT_TURN);
					printf("right\n");
					break;
				case LEFT:
					car->turnCar(LEFT_TURN);
					printf("left\n");
					break;
				default:
					printf("unknown:%d\n",event.xkey.keycode);
			}
			break;
		case KeyRelease:
			//printf( "KeyRelease: %d\n", e.xkey.keycode );
			switch(event.xkey.keycode){
				case FORWARD:
					car->setSpeed(0,1);
					printf("forward released\n");
					break;
				case BACKWARD:
					car->setSpeed(0,1);
					printf("backward released\n");
					break;
				case RIGHT:
					car->turnCar(NO_TURN);
					printf("right released\n");
					break;
				case LEFT:
					car->turnCar(NO_TURN);
					printf("left released\n");
					break;
				default:
					printf("unknown:%d\n",event.xkey.keycode);
			}
			break;
	}
}
