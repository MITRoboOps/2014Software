#include "ArmClass.h"

using namespace std;


ArmClass::ArmClass(int clawRank, int clawMinPos, int clawMaxPos, int clawMaxVel, int stickRank, int stickMinPos, int stickMaxPos, int stickMaxVel, int* servoMaxOpenedPositions, int* servoMaxClosedPositions, int* servoMaxVels, int thumbChannel, int panChannel, int tiltChannel, const char* servoPath, int boomMaxVel, int boomAccel, int boomDecel, int boomMaxAmps, std::string boomPath, int armRotMaxVelM, int armRotAccelM, int armRotDecelM,int armRotMaxVelA, int armRotAccelA, int armRotDecelA, int armRotMaxAmps, std::string armRotPath, int watchDog, int servoWatchDog,bool isTesting){

	boom = new RobotechMotor(boomMaxVel, watchDog, boomAccel,boomAccel,boomMaxAmps,true, boomPath);
	armRot = new RobotechMotor(armRotMaxVelM, watchDog, armRotAccelM, armRotAccelM, armRotMaxAmps, false, armRotPath);
	claw = new Actuator(clawRank, clawMinPos, clawMaxPos, clawMaxVel, watchDog);
	stick = new Actuator(stickRank, stickMinPos, stickMaxPos, stickMaxVel, watchDog);
	servo = new ServoMotor(servoMaxClosedPositions, servoMaxOpenedPositions, servoMaxVels, servoWatchDog);

	boom->connectToDevice();
	armRot->connectToDevice();
	servo->connectToDevice(servoPath);
	claw->connectToDevice();
	stick->connectToDevice();

	armRotMaxVelManual = armRotMaxVelM;
	armRotAccelManual = armRotAccelM;
	armRotDecelManual = armRotDecelM;
	armRotMaxVelAutomated = armRotMaxVelA;
	armRotAccelAutomated = armRotAccelA;
	armRotDecelAutomated = armRotDecelA;

	thumbCH = thumbChannel;
	tiltCH = tiltChannel;
	panCH = panChannel;

	sub = nh.subscribe("Arm",1, &ArmClass::callBack,this);
	
	resetStatusMessage();
	statusPublisher = nh.advertise<Messages::ArmStatusMessage>("ArmStatus", 1);
	automated =false;

}


void ArmClass::callBack(const Messages::ArmMessage::ConstPtr& msg){
	cout << "command is" <<  msg->command << endl;
	if(msg->command < 0 && !automated){
		boom->setVelocity(-(msg->boom -1000));
		cout << "recieved boom vel " << -(msg->boom -1000) << endl;
		armRot->setVelocity(-(msg->armRot -1000));
		cout << "recieved arm vel " << -(msg->armRot -1000) << endl;
		servo->setVelocity((int) (((double) msg->thumb -1000) / 1000.0 * ((double) servo->getMaxVel(thumbCH))), thumbCH);
		servo->setVelocity((int) (((double) msg->pan -1000) / 1000.0 * ((double) servo->getMaxVel(panCH))), panCH);
		servo->setVelocity((int) (((double) msg->tilt -1000) / 1000.0 * ((double) servo->getMaxVel(tiltCH))), tiltCH);
		cout << "pan " << msg->pan << " " << (int) (((double) msg->pan -1000) / 1000.0 * ((double) servo->getMaxVel(panCH))) <<endl;
		claw->setVelocity((int) -(((double) msg->claw -1000) / 1000.0 * ((double) claw->getMaxVel())));
		stick->setVelocity((int) (((double) msg->stick-1000) / 1000.0 * ((double) stick->getMaxVel())));
	}
	else {
		if(msg->command == 3){
			resetArmRotCounter();
			return;			
		}
		else if(msg->command == 4){
			killAutomatedAction();
		}
		else startAutomatedAction(msg->command);
	}
}

void ArmClass::publishStatusMessage(){
	status.boomCurrent = boom->getCurrent();
	status.armRotCurrent = armRot->getCurrent();
	status.thumbPosition = servo->getPosition(thumbCH);
	status.clawPosition = claw->getPosition();
	status.stickPosition = stick->getPosition();
	status.boomPosition = boom->getPosition();
	status.armRotPosition = armRot->getPosition();
	status.panPosition = servo->getPosition(panCH);
	status.tiltPosition = servo->getPosition(tiltCH);
	statusPublisher.publish(status);
}

void ArmClass::resetStatusMessage(){
	status.boomCurrent = 0;
	status.armRotCurrent = 0;
	status.thumbPosition = 0;
	status.clawPosition = 0;
	status.stickPosition = 0;
	status.boomPosition = 0;
	status.armRotPosition = 0;
	status.panPosition = 0;
	status.tiltPosition = 0;
}

void ArmClass::startAutomatedAction(int id){
	if(id < 0 || id > 2) return;
	automatedAction.~thread();
	automated = true;
	setAutomatedParameters();
	cout << "thread initiated" << endl;
	if(id == 0)std::thread automatedAction(&ArmClass::goToStowedPosition,this);
	if(id == 1)std::thread automatedAction(&ArmClass::goToRetrievalPosition,this);
	if(id == 2)std::thread automatedAction(&ArmClass::depositeRock,this);
	//automatedAction.join();
	
	
}

void ArmClass::killAutomatedAction(){
	cout << "thread killed" << endl;
	automatedAction.~thread();
	automated = false;
	setManualParameters();
}

void ArmClass::depositeRock(){
	cout << "thread method started" << endl;
	claw->setPosition(500);
	cout << "claw set" << endl;
	stick->setPosition(500);
	cout << "claw and stick set" << endl;
	while(boom->getFeedBack() < 0 && automated){
		boom->setVelocity(-1000);
		cout << "boom loop" << endl;
		usleep(100*1000);
	}
	while((claw->getPosition() > 520 || stick->getPosition() > 520) && automated){
		usleep(100*1000);
		cout << "actuator loop" << endl;
	}
	if(automated){
		cout << "difference : " << abs(armRot->getPosition() - 2312) << endl;
		armRot->setAbsolutePosition(2312);
		while(abs(armRot->getPosition() - 2312) > 20){
			usleep(100*1000);
		}
	}
	servo->openS(thumbCH);
}

void ArmClass::goToStowedPosition(){
	servo->closeS(thumbCH);
	claw->setPosition(0);
	stick->setPosition(0);
	while(boom->getFeedBack() < -600 && automated){
		boom->setVelocity(-1000);
		std::chrono::milliseconds dura( 100 );
		std::this_thread::sleep_for(dura);
	}
	if(armRot->getPosition() >6200) armRot->setAbsolutePosition(6200);
	while(boom->getFeedBack() < -250 && automated){
		boom->setVelocity(-1000);
		std::chrono::milliseconds dura( 100 );
		std::this_thread::sleep_for(dura);
	}
	while((claw->getPosition() > 20 || stick->getPosition() > 20) && automated){
		std::chrono::milliseconds dura( 100 );
		std::this_thread::sleep_for(dura);
	}
	if(automated){
		armRot->setAbsolutePosition(0);
		while(armRot->getPosition() > 50){
			std::chrono::milliseconds dura( 100 );
			std::this_thread::sleep_for(dura);
		}
	}
	while(boom->getFeedBack() > -345 && automated){
		boom->setVelocity(800);
	}
	automated = false;
	setManualParameters();
}

void ArmClass::goToRetrievalPosition(){

	while(boom->getFeedBack() <= -200 && automated){
		boom->setVelocity(-1000);
		cout << "boom loop" << boom->getFeedBack() <<  endl;
		std::chrono::milliseconds dura( 100 );
		std::this_thread::sleep_for(dura);
	}
	armRot->setAbsolutePosition(14000);
	while(armRot->getPosition() < 6000 && automated){
		armRot->setAbsolutePosition(14000);
		cout << "arm loop" << endl;
		std::chrono::milliseconds dura( 100 );
		std::this_thread::sleep_for(dura);
	}
	
	servo->closeS(thumbCH);
	armRot->setAbsolutePosition(14000);
	claw->setPosition(500);
	stick->setPosition(500);

	automated = false;
	setManualParameters();
}

void ArmClass::resetArmRotCounter(){
	armRot->resetCounter();
}

void ArmClass::setAutomatedParameters(){
	servo->setVelocity(servo->getMaxVel(thumbCH),thumbCH);
	armRot->setAccel(armRotAccelAutomated);
	armRot->setDecel(armRotDecelAutomated);
	armRot->setMaxPositionModeVelocity(armRotMaxVelAutomated);
}

void ArmClass::setManualParameters(){
	armRot->setAccel(armRotAccelManual);
	armRot->setDecel(armRotDecelManual);
	armRot->setMaxVelocity(armRotMaxVelManual);
}
