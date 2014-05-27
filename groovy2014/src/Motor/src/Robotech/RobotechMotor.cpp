#include "RobotechMotor.h"





using namespace std;

//Mode 0 : closed loop speed
//Mode 1 : count position mode (for setting absolute position)
//Mode 2 : relative position mode



RobotechMotor::RobotechMotor(int maxVelocity, int millisecs, int accel, int decel){
    mode = -1;
    maxRPM = maxVelocity;
    stallOut = millisecs;
    accelRate = accel;
    decelRate = decel;
}

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

    connection->writeString("^RWD " + IntToStr(stallOut) + "\r"); //set watchdog timeout
    connection->writeString("^ECHOF 1\r"); //echo off
    connection->writeString("^MXRPM 1 " + IntToStr(maxRPM) + "\r"); //set max speed
    connection->writeString("^MAC 1 "+ IntToStr(accelRate) + "\r"); //set acceleration
    connection->writeString("^MDEC 1 " + IntToStr(decelRate) + "\r"); //set deceleration
    connection->writeString("^MVEL 1 10\r"); //position mode velocity default

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
    //string answer = connection->writeString("?A1");
    //cout << connection->readLine() << endl;
    return 0.0;
}


void RobotechMotor::setAbsolutePosition(double pos){
    if(! mode == 1){
        connection->writeString("^MMOD 1 3\r");//position count mode
        mode = 1;
    }

    int actualPos = (int) (pos + 0.5);
    cout << actualPos << endl;
    string toString = IntToStr(actualPos);
    connection->writeString("!s 100\r");
    connection->writeString("!p 1 "+toString+"\r");

}

void RobotechMotor::setRelativePosition(double pos){
    if(! mode == 2){
        connection->writeString("^MMOD 1 2\r");//position count mode
        mode = 2;
    }

    int actualPos = (int) (pos + 0.5);
    cout << actualPos << endl;
    string toString = IntToStr(actualPos);
    connection->writeString("!s 1 1\r");
    connection->writeString("!g 1 "+toString+"\r");

}

//takes in all ints
void RobotechMotor::setVelocity(double vel){
    if(! mode == 0){
        connection->writeString("^MMOD 1 0\r");//open loop
        mode = 0;
    }
    int actualVel = (int) (vel + 0.5);
    string toString = IntToStr(actualVel);
    cout << toString << endl;
    connection->writeString("!g 1 "+toString+"\r");
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
