#include "DriveTrainClass.h"


using namespace std;

class DriveTrainClass;

DriveTrainClass::DriveTrainClass(std::string BLPath, std::string BRPath, std::string FLPath, std::string FRPath){

	BR = new RobotechMotor(150, 100,1500,1500);
	BL = new RobotechMotor(150, 100,1500,1500);
	FR = new RobotechMotor(150, 100,1500,1500);
	FL = new RobotechMotor(150, 100,1500,1500);

	cout << "initialization successful" << endl;	
	
	BL->connectToDevice(BLPath);
	cout << "BL successful" << endl;

	BR->connectToDevice(BRPath);
	cout << "BR successful" << endl;

	FL->connectToDevice(FLPath);
	cout << "FL successful" << endl;

	FR->connectToDevice(FRPath);

	cout << "connection successful" << endl;

	sub = nh.subscribe("DriveTrain",1, &DriveTrainClass::callBack,this);

	cout << "subscribers set up" << endl;
	

}


void DriveTrainClass::callBack(const Messages::DriveTrainMessage::ConstPtr& msg){
	ROS_INFO("I heard: %i left and %i right", msg->leftVelocity, msg->rightVelocity);
	int rVel = msg->rightVelocity -1000;
	int lVel = msg->leftVelocity -1000;
	BR->setVelocity(rVel);
	FR->setVelocity(rVel);
	BL->setVelocity(lVel);
	FL->setVelocity(lVel);
}

