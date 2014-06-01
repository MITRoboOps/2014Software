#include <ros/ros.h>
#include <ros/node_handle.h>
#include <Messages/CameraBoomMessage.h>
#include <Messages/CameraBoomStatus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Actuator.h"

class CameraBoomClass{

private:
	int downPos, upPos;	

	Messages::CameraBoomStatus status;
	// Node Handler
	ros::NodeHandle nh;

	// Subscribers
	ros::Subscriber sub;

	// Publishers
	ros::Publisher statusPublisher;

public:

	CameraBoomClass(int downPosition, int upPosition, int velocity, int rank);

	Actuator* cameraBoom;

	void callBack(const Messages::CameraBoomMessage::ConstPtr& msg);

	void publishStatusMessage();


};
