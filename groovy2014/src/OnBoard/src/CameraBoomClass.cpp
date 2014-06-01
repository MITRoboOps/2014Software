#include "CameraBoomClass.h"

using namespace std;

CameraBoomClass::CameraBoomClass(int downPosition, int upPosition, int velocity, int rank){

	cameraBoom = new Actuator(rank, 0, 950, velocity, 100);
	cameraBoom->connectToDevice();
	sub = nh.subscribe("CBoom",1, &CameraBoomClass::callBack,this);

	downPos = downPosition;
	upPos = upPosition;

	statusPublisher = nh.advertise<Messages::CameraBoomStatus>("CBoomStatus", 1);

}

void CameraBoomClass::callBack(const Messages::CameraBoomMessage::ConstPtr& msg){
	if(msg->command == 0) cameraBoom->setPosition(downPos);
	else if(msg->command == 1) cameraBoom->setPosition(upPos);
	else if(msg->command == 2) cameraBoom->setPosition(cameraBoom->getPosition() + 5);
	else if(msg->command == 3) cameraBoom->setPosition(cameraBoom->getPosition() - 5);
}

void CameraBoomClass::publishStatusMessage(){
	status.position = cameraBoom->getPosition();
	statusPublisher.publish(status);

}
