#include "ArmClass.h"

using namespace std;


ArmClass::ArmClass(int clawRank, int clawMinPos, int clawMaxPos, int clawMaxVel, int stickRank, int stickMinPos, int stickMaxPos, int stickMaxVel, int* servoMaxOpenedPositions, int* servoMaxClosedPositions, int* servoMaxVels, int thumbChannel, int panChannel, int tiltChannel, const char* servoPath, int boomMaxVel, int boomAccel, int boomDecel, int boomMaxAmps, std::string boomPath, int armRotMaxVel, int armRotAccel, int armRotDecel, int armRotMaxAmps, std::string armRotPath, int watchDog, int servoWatchDog){

	boom = new RobotechMotor(boomMaxVel, watchDog, boomAccel,boomAccel,boomMaxAmps);
	armRot = new RobotechMotor(armRotMaxVel, watchDog, armRotAccel,armRotAccel,armRotMaxAmps);
	claw = new Actuator(clawRank, clawMinPos, clawMaxPos, clawMaxVel, watchDog);
	stick = new Actuator(stickRank, stickMinPos, stickMaxPos, stickMaxVel, watchDog);

	servo = new ServoMotor(servoMaxClosedPositions, servoMaxOpenedPositions, servoMaxVels, servoWatchDog);

	boom->connectToDevice(boomPath);
	armRot->connectToDevice(armRotPath);
	servo->connectToDevice(servoPath);
	claw->connectToDevice();
	stick->connectToDevice();

	thumbCH = thumbChannel;
	tiltCH = tiltChannel;
	panCH = panChannel;

	sub = nh.subscribe("Arm",1, &ArmClass::callBack,this);
	

}


void ArmClass::callBack(const Messages::ArmMessage::ConstPtr& msg){
	
	boom->setVelocity(-(msg->boom -1000));
	armRot->setVelocity(-(msg->armRot -1000));
	servo->setVelocity((int) (((double) msg->thumb -1000) / 1000.0 * ((double) servo->getMaxVel(thumbCH))), thumbCH);
	servo->setVelocity((int) (((double) msg->pan -1000) / 1000.0 * ((double) servo->getMaxVel(panCH))), panCH);
	servo->setVelocity((int) (((double) msg->tilt -1000) / 1000.0 * ((double) servo->getMaxVel(tiltCH))), tiltCH);
	cout << "pan " << msg->pan << " " << (int) (((double) msg->pan -1000) / 1000.0 * ((double) servo->getMaxVel(panCH))) <<endl;
	claw->setVelocity((int) -(((double) msg->claw -1000) / 1000.0 * ((double) claw->getMaxVel())));
	ROS_INFO("set claw velocity to : %i\n", (int) -(((double) msg->claw -1000) / 1000.0 * ((double) claw->getMaxVel())));
	stick->setVelocity((int) (((double) msg->stick-1000) / 1000.0 * ((double) stick->getMaxVel())));
	
}
