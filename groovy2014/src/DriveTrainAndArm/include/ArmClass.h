#include "ServoMotor.h"
#include "RobotechMotor.h"
#include <ros/ros.h>
#include <ros/node_handle.h>
#include "std_msgs/String.h"
#include <Messages/ArmMessage.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Actuator.h"


class ArmClass {

private:

	// Node Handler
	ros::NodeHandle nh;

	// Subscribers
	ros::Subscriber sub;

	// Publishers
	ros::Publisher pointPub;

public:

	ArmClass(int clawRank, int clawMinPos, int clawMaxPos, int clawMaxVel, int stickRank, int stickMinPos, int stickMaxPos, int stickMaxVel, int thumbMaxClosedPosition, int thumbMaxOpenedPosition, int thumbMaxVel, const char* thumbPath, int boomMaxVel, int boomAccel, int boomDecel, std::string boomPath, int armRotMaxVel, int armRotAccel, int armRotDecel, std::string armRotPath, int watchDogStallTime);

	RobotechMotor *boom;
	RobotechMotor *armRot;
	Actuator *claw;
	Actuator *stick;
	ServoMotor *thumb;

	void callBack(const Messages::ArmMessage::ConstPtr& msg);

};
