#include "Serial.h"
#include <ros/ros.h>
#include <ros/node_handle.h>
#include "std_msgs/String.h"
#include <Messages/GPSMessage.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <thread>
#include <mutex>
#include <iostream>
#define BAUD_RATE 38400
#define LOOPRATE 10
#define PUBLISHRATE 1

//Messages
Messages::GPSMessage GPSmsg;
std::list<int> out;
std::mutex msgLock;

Serial* gps;

using namespace std;

void parseMessage(string msg){

// !!!VER:1.9,RLL:0.07,PCH:0.15,YAW:7.01,IMUH:253,LAT:0,LON:0,ALT:-17,COG:0,SOG:0,FIX:0,SAT:0,TOW:1219000***
	size_t found = msg.find_first_of(",");
	if(found == string::npos) return;
	msg = msg.substr(found + 1, string::npos);
	if(msg.at(0) == 'R'){ //Roll
		msg = msg.substr(4,string::npos);
		found = msg.find_first_of(".");
		if(found == string::npos) {
			parseMessage(msg);
			return;
		}
		const char* roll = (msg.substr(0,found) + msg.substr(found + 1, found +3)).c_str();
		GPSmsg.roll = atoi(roll);
		parseMessage(msg);
	} else if(msg.at(0) == 'P'){ //Pitch
		msg = msg.substr(4,string::npos);
		found = msg.find_first_of(".");
		if(found == string::npos) {
			parseMessage(msg);
			return;
		}
		const char* pitch = (msg.substr(0,found) + msg.substr(found + 1, found +3)).c_str();
		GPSmsg.pitch = atoi(pitch);
		parseMessage(msg);
	} else if(msg.at(0) == 'Y'){ //Yaw
		msg = msg.substr(4,string::npos);
		found = msg.find_first_of(".");
		if(found == string::npos) {
			parseMessage(msg);
			return;
		}
		const char* yaw = (msg.substr(0,found) + msg.substr(found + 1, found +3)).c_str();
		GPSmsg.yaw = atoi(yaw);
		parseMessage(msg);
	}else if(msg.at(0) == 'L'){
		if(msg.at(1) == 'O'){ //Longitude
			msg = msg.substr(4,string::npos);
			found = msg.find_first_of(",");
			if(found == string::npos) {
				parseMessage(msg);
				return;
			}
			const char* longitude = (msg.substr(0,found)).c_str();
			GPSmsg.longitude = atoi(longitude);
			parseMessage(msg);
		} else if(msg.at(1) == 'A'){
			msg = msg.substr(4,string::npos);
			found = msg.find_first_of(",");
			if(found == string::npos) {
				parseMessage(msg);
				return;
			}
			const char* latitude = (msg.substr(0,found)).c_str();
			GPSmsg.latitude = atoi(latitude);
			parseMessage(msg);
		}
	} else if(msg.at(0) == 'S'){ //Satilites
		msg = msg.substr(4,string::npos);
		found = msg.find_first_of(",");
		if(found == string::npos) {
			parseMessage(msg);
			return;
		}
		const char* satilites = (msg.substr(0,found)).c_str();
		GPSmsg.satilites = atoi(satilites);
		parseMessage(msg);
	}

}

void readController(){
	while(ros::ok()){
		string output = gps->readLine();
		if(output.at(1) != '!') continue;
		msgLock.lock();
		parseMessage(output);
		out.push_back(1);
		msgLock.unlock();
	}
}

int main( int argc, char **argv){

	ros::init(argc,argv,"GPSNode");

	// Node Handler
	ros::NodeHandle nh;

	GPSmsg.roll = 0;
	GPSmsg.pitch = 0;
	GPSmsg.yaw = 0;
	GPSmsg.latitude = 0;
	GPSmsg.longitude = 0;
	GPSmsg.satilites = 0;


	// Publishers
	ros::Publisher statusPublisher = nh.advertise<Messages::GPSMessage>("GPS", 1);;

	int numTries = 0;
	int numberToTry = 5;
	while(numTries < numberToTry)
		try{
			gps = new Serial("/dev/ttyUSB8",BAUD_RATE);
			numTries = numberToTry;
		} catch (exception& e) {
			cout << "Connection failed. Retrying " << numberToTry - numTries << " more times." << endl;
			if(numTries == numberToTry) return false;
			numTries++;
			usleep (3000*1000);
			}

	std::thread outMessagesThread(readController);
	outMessagesThread.detach();
	ros::Rate loop_rate(LOOPRATE);
	while(ros::ok()){
		msgLock.lock();
		if(out.empty()){
			msgLock.unlock();
			loop_rate.sleep();
			continue;
		}
		else{
			out.pop_front();
			cout << "roll : " << GPSmsg.roll << " pitch : " << GPSmsg.pitch << " yaw : " << GPSmsg.yaw << " latitude : " << GPSmsg.latitude << " longitude : " << GPSmsg.longitude << " satilites : " << GPSmsg.satilites << endl;
			statusPublisher.publish(GPSmsg);
			msgLock.unlock();
			ros::spinOnce();
		}
	}
	outMessagesThread.~thread();
	
}
