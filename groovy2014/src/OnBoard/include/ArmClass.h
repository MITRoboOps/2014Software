#include "ServoMotor.h"
#include "RobotechMotor.h"
#include <ros/ros.h>
#include <ros/node_handle.h>
#include <Messages/ArmMessage.h>
#include <Messages/ArmStatusMessage.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include "Actuator.h"


class ArmClass {

private:

	int thumbCH, panCH, tiltCH;

	std::thread automatedAction;

	bool automated; // if true we are in an automated process

	int armRotMaxVelManual, armRotAccelManual, armRotDecelManual, armRotMaxVelAutomated, armRotAccelAutomated, armRotDecelAutomated;

	Messages::ArmStatusMessage status;
	// Node Handler
	ros::NodeHandle nh;

	// Subscribers
	ros::Subscriber sub;

	// Publishers
	ros::Publisher statusPublisher;

public:

	ArmClass(int clawRank, int clawMinPos, int clawMaxPos, int clawMaxVel, int stickRank, int stickMinPos, int stickMaxPos, int stickMaxVel, int* servoMaxOpenedPositions, int* servoMaxClosedPositions, int* servoMaxVels, int thumbChannel, int panChannel, int tiltChannel, const char* servoPath, int boomMaxVel, int boomAccel, int boomDecel, int boomMaxAmps, std::string boomPath, int armRotMaxVelM, int armRotAccelM, int armRotDecelM,int armRotMaxVelA, int armRotAccelA, int armRotDecelA, int armRotMaxAmps, std::string armRotPath,  int watchDog, int servoWatchDog, bool isTesting);

	RobotechMotor *boom;
	RobotechMotor *armRot;
	Actuator *claw;
	Actuator *stick;
	ServoMotor *servo;

	void callBack(const Messages::ArmMessage::ConstPtr& msg);

	void publishStatusMessage();

	void resetStatusMessage();

	void startAutomatedAction(int id);

	void killAutomatedAction();

	void depositeRock();

	void goToStowedPosition();

	void goToRetrievalPosition();

	void setAutomatedParameters();

	void setManualParameters();

	void resetArmRotCounter();

};
