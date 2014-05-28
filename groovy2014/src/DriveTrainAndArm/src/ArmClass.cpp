#include "ArmClass.h"

using namespace std;


ArmClass::ArmClass(int clawRank, int clawMinPos, int clawMaxPos, int clawMaxVel, int stickRank, int stickMinPos, int stickMaxPos, int stickMaxVel, int thumbMaxClosedPosition, int thumbMaxOpenedPosition, int thumbMaxVel, const char* thumbPath, int boomMaxVel, int boomAccel, int boomDecel, std::string boomPath, int armRotMaxVel, int armRotAccel, int armRotDecel, std::string armRotPath, int watchDogStallTime){

	boom = new RobotechMotor(boomMaxVel, watchDogStallTime, boomAccel,boomAccel);
	armRot = new RobotechMotor(armRotMaxVel, watchDogStallTime, armRotAccel,armRotAccel);
	claw = new Actuator(clawRank, clawMinPos, clawMaxPos, clawMaxVel, watchDogStallTime);
	stick = new Actuator(stickRank, stickMinPos, stickMaxPos, stickMaxVel, watchDogStallTime);
	thumb = new ServoMotor(thumbMaxClosedPosition, thumbMaxOpenedPosition, thumbMaxVel, 20);

	boom->connectToDevice(boomPath);
	armRot->connectToDevice(armRotPath);
	thumb->connectToDevice(thumbPath);
	claw->connectToDevice();
	stick->connectToDevice();

	sub = nh.subscribe("Arm",1, &ArmClass::callBack,this);
	

}


void ArmClass::callBack(const Messages::ArmMessage::ConstPtr& msg){
	
	boom->setVelocity(-(msg->boom -1000));
	armRot->setVelocity(-(msg->armRot -1000));
	thumb->setVelocity((int) (((double) msg->thumb -1000) / 1000.0 * ((double) thumb->getMaxVel())));
	claw->setVelocity((int) -(((double) msg->claw -1000) / 1000.0 * ((double) claw->getMaxVel())));
	stick->setVelocity((int) (((double) msg->stick-1000) / 1000.0 * ((double) stick->getMaxVel())));
	
}
