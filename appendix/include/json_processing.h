
//defines
#define BUFFER_SIZE  (256 * 1024)  /* 256 KB */
#define URL_FORMAT   "https://wodinaz.com/%s"
#define URL_SIZE     256

//includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

//functions
void json_test_function();
//example code that uses the four basic functions to communicate with the server

void debug_print_map(map<string,double> mymap);
// a debug function used to print maps received from the server

void debug_print_vector(vector<string> myvector);
//debug function used to print vectors


void json_send_data(map<string,double> mymap);
// Uploads the provided map of sensor values to the server

map<string,double> json_get_data(int id);
// Downloads sensor data from the server. The user must choose which agent (id) to receive from

void json_send_command(string cmd,int id);
// Uploads a command to the server. 
//The agent with the corresponding id will download this command

vector<string> json_get_commands(int id);
//Download commands from the server. 