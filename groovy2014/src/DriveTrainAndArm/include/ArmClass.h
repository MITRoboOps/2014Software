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

	int thumbCH, panCH, tiltCH;

	// Node Handler
	ros::NodeHandle nh;

	// Subscribers
	ros::Subscriber sub;

	// Publishers
	ros::Publisher pointPub;

public:

	ArmClass(int clawRank, int clawMinPos, int clawMaxPos, int clawMaxVel, int stickRank, int stickMinPos, int stickMaxPos, int stickMaxVel, int* servoMaxOpenedPositions, int* servoMaxClosedPositions, int* servoMaxVels, int thumbChannel, int panChannel, int tiltChannel, const char* servoPath, int boomMaxVel, int boomAccel, int boomDecel, int boomMaxAmps, std::string boomPath, int armRotMaxVel, int armRotAccel, int armRotDecel, int armRotMaxAmps, std::string armRotPath,  int watchDog, int servoWatchDog);

	RobotechMotor *boom;
	RobotechMotor *armRot;
	Actuator *claw;
	Actuator *stick;
	ServoMotor *servo;

	void callBack(const Messages::ArmMessage::ConstPtr& msg);

};
