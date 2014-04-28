/*
 * Copyright (c) 2009-2013 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <jansson.h>

#include "http_functions.h"

#define BUFFER_SIZE  (256 * 1024)  /* 256 KB */

#define URL_FORMAT   "https://wodinaz.com/%s"
#define URL_SIZE     256
int i=0;

//URL's
#define PATH_CONNECT "connect"
#define PATH_DATA "data/"
#define PATH_COMMAND "command/"

//C++ stuff
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

int myID=0;
int testID=0;

//functions



void debug_print_map(map<string,double> mymap){
    for (map<string,double>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    {
      string key = it->first;
      double value = it->second;
      printf ("sensor %s has value %f\n",key.c_str(),value);
    }
}

void debug_print_vector(vector<string> myvector){
    for (vector<string>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    {
      string command = *it;
      printf ("command: %s\n",command.c_str());
    }
}

string convertIntToString(int number)
{
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}
int convertStringToInt(string inputString){
    return atoi(inputString.c_str());
}
double convertStringToDouble(string inputString){
    stringstream ss(inputString);
    double result;
    return ss >> result ? result : 0;
    
}
string convertDoubleToString(double number){
    ostringstream convert;   // stream used for the conversion

    convert << number;      // insert the textual representation of 'Number' in the characters in the stream

    return convert.str(); // set 'Result' to the contents of the stream

}

map<string,double> json_get_data(int id){
    printf("starting get_data\n");
    map<string,double> data_map;
    int root_length=0;
    char *text_response;
    char url[URL_SIZE];
    string id_path=PATH_DATA;

    string id_string = "client_"+convertIntToString(id);
    id_path.append(id_string);
    snprintf(url, URL_SIZE, URL_FORMAT, id_path.c_str());
    printf("url:%s\n",url);

    text_response = http_request(url);
    printf("response:%s\n",text_response);
    json_t *root;
    json_error_t error;
    root = json_loads(text_response, 0, &error);
    free(text_response);
    
    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        throw 202;
    }

    if(!json_is_array(root))
    {
        fprintf(stderr, "error: root is not an object\n");
        json_decref(root);
        root_length=1;
    }
    
    root_length=json_array_size(root);
    printf("root_length:%d\n",root_length );
    //getting the actual data
    json_t *data, *time_stamp, *entry_id, *sensor, *sensor_value, *device_id;
    double timeStamp,entryID,sensorValue, deviceID;
    string sensor_name;
    for (i=0;i<root_length;i++){ //DEBUG i<root_length
        data = json_array_get(root, i);
        if(!json_is_object(data))
        {
            fprintf(stderr, "error: commit data %d is not an object\n", i + 1);
            json_decref(root);
            throw 202;
        }
        
        time_stamp = json_object_get(data,"timestamp");
        if (!json_is_string(time_stamp)){
            printf("throwing jsonException\n");
            throw 202;
        }
        else {

            timeStamp = convertStringToDouble(json_string_value(time_stamp));
            printf("timeStamp:%f\n",timeStamp );
        }
        
        entry_id = json_object_get(data,"_id");
        if (!json_is_string(entry_id)){
            printf("throwing jsonException\n");
            throw 202;
        }
        else {
            entryID =convertStringToDouble(json_string_value(entry_id));
        }
        
        sensor= json_object_get(data,"sensor");
        if (!json_is_string(sensor)){
            printf("throwing jsonException\n");
            throw 202;
        }
        else {
            sensor_name = json_string_value(sensor);
            printf("sensor_name:%s\n",sensor_name.c_str() );
        }
        
        const char* snsr_name = sensor_name.c_str();
        sensor_value = json_object_get(data,snsr_name);
        if (!json_is_string(sensor_value)){
            printf("throwing jsonException at sensor_value\n");
            throw 202;
        }
        else {
            sensorValue= convertStringToDouble(json_string_value(sensor_value));
            printf("sensor_value:%f\n",sensorValue);
        }
        
        device_id = json_object_get(data,"device_id");
        if (!json_is_string(device_id)){
            printf("throwing jsonException at device id\n");
            throw 202;
        }
        else {
            deviceID = convertStringToDouble(json_string_value(device_id));
            printf("deviceID:%f\n",deviceID);
        }
        //put stuff in returning map
        data_map[sensor_name]=sensorValue;
    }
    return data_map;
}

void json_send_data(map<string,double> mymap){
    //printf("starting send_data\n");
    
    char url[URL_SIZE];
    

    string id_string = convertIntToString(myID); 
    string http_path=PATH_DATA;
    http_path.append("client_"+id_string);
    string sensor_name;
    string key;
    double value;
    string value_string;
    string json_string;
    for (map<string,double>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    {
        key = it->first;
        value = it->second;
        value_string=convertDoubleToString(value);
        sensor_name=key;

        string http_path=PATH_DATA;
        http_path.append("client_"+id_string);
        http_path.append("/");
        http_path.append(sensor_name);
        json_string="{";
        json_string.append("\"");

        json_string.append(sensor_name);
        json_string.append("\"");
        json_string.append(":");
        json_string.append(" ");
        json_string.append("\""+value_string+"\""+"}");
        snprintf(url, URL_SIZE, URL_FORMAT, http_path.c_str());
        //printf("url:%s\n",url);
        //printf("json_string:%s\n",json_string.c_str());

        char *json_cstring = new char[json_string.length() + 1];
        strcpy(json_cstring, json_string.c_str());
        // do stuff

        http_post(url,json_cstring);
        free(json_cstring);
    }
}

void json_send_command(string cmd,int id){
    printf("starting send_commands\n");
    
    char url[URL_SIZE];
    string command=cmd;
    string http_path=PATH_COMMAND;  
    string id_string = convertIntToString(id); 
    http_path.append("client_"+id_string);
    string json_string; 
    http_path=PATH_COMMAND;
    http_path.append("client_"+id_string);
    json_string="{";
    json_string.append("\"");

    json_string.append("command");
    json_string.append("\"");
    json_string.append(":");
    json_string.append(" ");
    json_string.append("\""+command+"\""+"}");
    snprintf(url, URL_SIZE, URL_FORMAT, http_path.c_str());
    printf("url:%s\n",url);
    printf("json_string:%s\n",json_string.c_str());

    char *json_cstring = new char[json_string.length() + 1];
    strcpy(json_cstring, json_string.c_str());
    // do stuff
    
    http_post(url,json_cstring);
    free(json_cstring);
}

vector<string> json_get_commands(int id){
    //printf("starting get_commands\n");
    vector<string> commands_vector;
    int root_length=0;
    char *text_response;
    char url[URL_SIZE];
    string id_path=PATH_COMMAND;

    string id_string = "client_"+convertIntToString(id);
    id_path.append(id_string);
    snprintf(url, URL_SIZE, URL_FORMAT, id_path.c_str());
    //printf("url:%s\n",url);

    text_response = http_request(url);
    //printf("response:%s\n",text_response);
    json_t *root;
    json_error_t error;
    root = json_loads(text_response, 0, &error);
    free(text_response);
    
    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        throw 202;
    }

    if(!json_is_array(root))
    {
        fprintf(stderr, "error: root is not an array\n");
        json_decref(root);
        root_length=1;
    }
    
    root_length=json_array_size(root);
    //printf("root_length:%d\n",root_length );
    //getting the actual data
    json_t *data, *time_stamp, *iterator;
    double timeStamp;
    string command="";
    for (i=0;i<root_length;i++){ //DEBUG i<root_length
        data = json_array_get(root, i);
        if(!json_is_object(data))
        {
            fprintf(stderr, "error: commit data %d is not an object\n", i + 1);
            json_decref(root);
            throw 202;
        }

        time_stamp = json_object_get(data,"timestamp");
        if (!json_is_string(time_stamp)){
            printf("throwing jsonException\n");
            throw 202;
        }
        else {

            timeStamp = convertStringToDouble(json_string_value(time_stamp));
            printf("timeStamp:%f\n",timeStamp );
        }
        iterator =json_object_get(data,"command");
        if (!json_is_string(iterator)){
            printf("throwing jsonException\n");
            throw 202;
        }
        else {
            command = json_string_value(iterator);
            //printf("command:%s\n",command.c_str());
        }
        commands_vector.push_back(command);
    }
    return commands_vector;
}
         
  
void json_test_function(){
    map<string,double> debug_map;
    debug_map["test1"]=8.9;
    debug_map["test2"]=5678.456;
    printf("Sending data\n");
    json_send_data(debug_map);
    printf("printing data\n");
    debug_print_map(json_get_data(testID));
    

    string command1="command_one";
    string command2="command two";
    printf("sending commands\n");
    json_send_command(command1,testID);
    json_send_command(command2,testID);
    printf("printing commands\n");
    debug_print_vector(json_get_commands(testID));
}
