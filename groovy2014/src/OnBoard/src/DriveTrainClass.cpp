#include "DriveTrainClass.h"


using namespace std;

class DriveTrainClass;

DriveTrainClass::DriveTrainClass(std::string BLPath, std::string BRPath, std::string FLPath, std::string FRPath){

	int maxRPM = 150;
	int watchDog = 500;
	int accel = 6000;
	int decel = 6000;
	int maxAmps = 80;
	bool isOpenLoop = true;
	int Igain = 130;
	int Dgain = 10;
	int Pgain = 3;

	BR = new RobotechMotor(maxRPM, watchDog,accel,decel, maxAmps,isOpenLoop,BRPath);
	BL = new RobotechMotor(maxRPM, watchDog,accel,decel, maxAmps,isOpenLoop,BLPath);
	FR = new RobotechMotor(maxRPM, watchDog,accel,decel, maxAmps,isOpenLoop,FRPath);
	FL = new RobotechMotor(maxRPM, watchDog,accel,decel, maxAmps,isOpenLoop,FLPath);
	
	BL->connectToDevice();
	BR->connectToDevice();
	FL->connectToDevice();
	FR->connectToDevice();

	BR->setIGain(Igain);
	BL->setIGain(Igain);
	FR->setIGain(Igain);
	FL->setIGain(Igain);

	BR->setDGain(Dgain);
	BL->setDGain(Dgain);
	FR->setDGain(Dgain);
	FL->setDGain(Dgain);

	BR->setPGain(Dgain);
	BL->setPGain(Dgain);
	FR->setPGain(Dgain);
	FL->setPGain(Dgain);


	sub = nh.subscribe("DriveTrain",1, &DriveTrainClass::callBack,this);
	statusPublisher = nh.advertise<Messages::DriveTrainStatusMessage>("DriveTrainStatus", 1);
	
	cout << "connection successful" << endl;
}


void DriveTrainClass::callBack(const Messages::DriveTrainMessage::ConstPtr& msg){
	int rVel = msg->rightVelocity -1000;
	int lVel = msg->leftVelocity -1000;
	BR->setVelocity(-rVel);
	FR->setVelocity(-rVel);
	BL->setVelocity(lVel);
	FL->setVelocity(lVel);
}

void DriveTrainClass::publishStatusMessage(){
	status.BLCurrent = BL->getCurrent();
	status.FLCurrent = FL->getCurrent();
	status.BRCurrent = BR->getCurrent();
	status.FRCurrent = FR->getCurrent();
	status.BLVelocity = BL->getVelocity();
	status.FLVelocity = FL->getVelocity();
	status.BRVelocity = BR->getVelocity();
	status.FRVelocity = FR->getVelocity();
	status.Voltage1 = BL->getBatteryVoltage();
	statusPublisher.publish(status);
}

void DriveTrainClass::resetStatusMessage(){

	status.BLCurrent = 0;
	status.FLCurrent = 0;
	status.BRCurrent = 0;
	status.FRCurrent = 0;
	status.BLVelocity = 0;
	status.FLVelocity = 0;
	status.BRVelocity = 0;
	status.FRVelocity = 0;
	status.Voltage1 = 0;
}

