#include "RobotechMotor.h"





using namespace std;

Serial* connection;


RobotechMotor::RobotechMotor(){}

std::string RobotechMotor::IntToStr(int x){
        std::ostringstream out;
        out << x;
        return out.str();
}


bool RobotechMotor::connectToDevice(std::string pathToUSB){
    int numTries = 0;
    int numberToTry = 5;
    while(numTries < numberToTry)
    try{
        connection = new Serial(pathToUSB,115200);
        numTries = numberToTry;
        } catch (exception& e) {
			std::cout << "Connection failed. Retrying " << numberToTry - numTries << " more times." << endl;
			if(numTries == numberToTry) return false;
			numTries++;
			usleep (3000*1000);
		}

    connection->writeString("^ECHOF 1\r"); //echo off
    return true;


}

double RobotechMotor::getPosition(){

return 0.0;
}

double RobotechMotor::getVelocity(){

return 0.0;
}

//Don't know for actuator
double RobotechMotor::getCurrent(){

return 0.0;
}


void RobotechMotor::setPosition(double pos){

}

//takes in all ints
void RobotechMotor::setVelocity(double vel){
    int actualVel = (int) (vel + 0.5);
    string toString = IntToStr(actualVel);
    connection->writeString("!s 1 "+toString+"\r");
}

void RobotechMotor::setCurrent(double current){

}

bool RobotechMotor::getStatus(){
//TODO
    return true;
}

std::string RobotechMotor::getStatusString(){
    //TODO
    return "";
}
