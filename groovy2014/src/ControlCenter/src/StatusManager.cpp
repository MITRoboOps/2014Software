#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <Messages/ArmStatusMessage.h>
#include <Messages/DriveTrainStatusMessage.h>
#include <Messages/PilotMessage.h>
#include <Messages/NavigationMessage.h>
#include <Messages/GPSMessage.h>
#include <Messages/CameraBoomStatus.h>
#include <thread>
#include <mutex>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#define LOOPRATE 10
#define PUBLISHRATE 1

struct timeval start, end;


//messages
Messages::NavigationMessage navMsg;
Messages::PilotMessage pilotMsg;

std::mutex pilotMsgLock;
std::mutex navMsgLock;

//0 msgs out TODO



int getRunTime(){
	gettimeofday(&end, NULL);
	std::cout << end.tv_sec  - start.tv_sec << std::endl;
	return (int) end.tv_sec  - start.tv_sec;
}

void armStatusCallBack(const Messages::ArmStatusMessage::ConstPtr& msg){
	
	navMsgLock.lock();
	navMsg.boomCurrent = msg->boomCurrent;
	navMsg.armRotCurrent = msg->armRotCurrent;
	navMsg.thumbPosition = msg->thumbPosition;
	navMsg.clawPosition = msg->clawPosition;
	navMsg.stickPosition = msg->stickPosition;
	navMsg.boomPosition = msg->boomPosition;
	navMsg.armRotPosition = msg->armRotPosition;
	navMsg.panPosition = msg->panPosition;
	navMsg.tiltPosition = msg->tiltPosition;
	navMsgLock.unlock();

}

void driveTrainStatusCallBack(const Messages::DriveTrainStatusMessage::ConstPtr& msg){
	pilotMsgLock.lock();
	pilotMsg.batteryVoltage = msg->Voltage1;
	pilotMsgLock.unlock();
	
	navMsgLock.lock();
	navMsg.BLcurrent = msg->BLCurrent;
	navMsg.FLcurrent = msg->FLCurrent;
	navMsg.BRcurrent = msg->BRCurrent;
	navMsg.FRcurrent = msg->FRCurrent;
	navMsg.BLvel = msg->BLVelocity;
	navMsg.FLvel = msg->FLVelocity;
	navMsg.BRvel = msg->BRVelocity;
	navMsg.FRvel = msg->FRVelocity;
	navMsg.voltage = msg->Voltage1;
	navMsgLock.unlock();
}

void GPSCallBack(const Messages::GPSMessage::ConstPtr& msg){
	pilotMsgLock.lock();
	pilotMsg.latitude = msg->latitude;
	pilotMsg.longitude = msg->longitude;
	pilotMsg.roll = msg->roll;
	pilotMsg.pitch = msg->pitch;
	pilotMsg.yaw = msg->yaw;
	pilotMsgLock.unlock();

	navMsgLock.lock();
	navMsg.latitude = msg->latitude;
	navMsg.longitude = msg->longitude;
	navMsg.roll = msg->roll;
	navMsg.pitch = msg->pitch;
	navMsg.yaw = msg->yaw;
	navMsg.satilites = msg->satilites;
	std::cout << navMsg.roll << std::endl;
	navMsgLock.unlock();
}

void CBoomCallBack(const Messages::CameraBoomStatus::ConstPtr& msg){
	navMsgLock.lock();
	navMsg.CBoomPosition = msg->position;
	navMsgLock.unlock();
}

int main(int argc, char **argv){

	//global time
	gettimeofday(&start, NULL);

	ros::init(argc, argv, "StatusManager");
	ros::NodeHandle nh;

	pilotMsgLock.lock();
	pilotMsg.timeUp = 0;
	pilotMsg.batteryVoltage =0;
	pilotMsg.latitude=0;
	pilotMsg.longitude=0;
	pilotMsg.chargeUsed=0;
	pilotMsg.roll = 0;
	pilotMsg.pitch = 0;
	pilotMsg.yaw = 0;
	pilotMsgLock.unlock();

	//subscribers
	ros::Subscriber DTStatus = nh.subscribe("DriveTrainStatus",1,driveTrainStatusCallBack);
	ros::Subscriber AStatus = nh.subscribe("ArmStatus",1,armStatusCallBack);
	ros::Subscriber GPSStatus = nh.subscribe("GPS",1,GPSCallBack);
	ros::Subscriber CBoomStatus = nh.subscribe("CBoomStatus",1,CBoomCallBack);

	//publishers
	ros::Publisher pilotPub = nh.advertise<Messages::PilotMessage>("Pilot", 1);
	ros::Publisher navPub = nh.advertise<Messages::NavigationMessage>("Navigation",1);

	ros::Rate loop_rate(LOOPRATE);
	int counter = 0;
	while(ros::ok()){
		if(counter%(PUBLISHRATE*LOOPRATE) == 0){
			pilotMsgLock.lock();
			pilotMsg.timeUp = getRunTime();
			//std::cout << getRunTime() << std::endl;
			pilotPub.publish(pilotMsg);
			pilotMsgLock.unlock();
			navMsgLock.lock();
			navMsg.timeUp = getRunTime();
			navPub.publish(navMsg);
			navMsgLock.unlock();
			counter = 0;
		}
		counter++;
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
