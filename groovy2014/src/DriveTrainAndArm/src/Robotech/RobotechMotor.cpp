#include "../../include/RobotechMotor.h"





using namespace std;

//Mode 0 : closed loop speed
//Mode 1 : count position mode (for setting absolute position)
//Mode 2 : relative position mode

RobotechMotor::RobotechMotor(int maxVelocity, int millisecs, int accel, int decel, int maxCurrent){
    mode = -1;
    maxRPM = maxVelocity;
    stallOut = millisecs;
    accelRate = accel;
    decelRate = decel;
    if(maxCurrent < 0) maxAmps = 30;
    else maxAmps = maxCurrent;
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

    setMaxVelocity(maxRPM);
    setMaxCurrent(maxAmps);
    setAccel(accelRate);
    setDecel(decelRate);
    setWatchDogTime(stallOut);
    connection->writeString("^ECHOF 1\r"); //echo off
    connection->writeString("^MVEL 1 10\r"); //position mode velocity default

    thread readValuesThread(&RobotechMotor::readValues,this);
    readValuesThread.detach();


    return true;


}

void RobotechMotor::readValues(){

    while(1){
        string msg = connection->readLine();
        char *msgArray = new char[msg.length() + 1];
        strcpy(msgArray,msg.c_str());
        if(msgArray[0] != '+'){
            getStatus.lock();
            statusMessages.push_back(msgArray);
            getStatus.unlock();
        }
    }

}

char* RobotechMotor::getStatusMessage(char identifier){
    getStatus.lock();
    while(!statusMessages.empty()){
        char* msg = statusMessages.back();
        statusMessages.pop_back();
        if(msg[0] == identifier){
            statusMessages.clear();
            getStatus.unlock();
            return msg;
        }

    }
    getStatus.unlock();
    return "";

}


double RobotechMotor::getPosition(){

return 0.0;
}

double RobotechMotor::getVelocity(){

return 0.0;
}

//Don't know for actuator
double RobotechMotor::getCurrent(){
   // connection->writeString("# C");
    connection->writeString("?A 1");
    char* msg = getStatusMessage('A');
    char* value = new char[sizeof(msg)-2];
    memcpy(value, msg + 2, sizeof(value));

    return atoi(value) / 10.0;
}

double RobotechMotor::getBatteryVoltage(){
    connection->writeString("?V 1");
    char* msg = getStatusMessage('V');
    int start = 0;
    for(int i = 0; i < sizeof(msg); i++){
        if(msg[i] == ':'){
            start = i +1;
            break;

        }
    }
    char* volt = new char[sizeof(msg) - start];
    memcpy(volt, msg + start, sizeof(volt));
    return atoi(volt);
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

void RobotechMotor::setMaxVelocity(int vel){
    connection->writeString("^MXRPM 1 " + IntToStr(vel) + "\r"); //set max speed
    maxRPM = vel;
}
int RobotechMotor::getMaxVelocity(){return maxRPM;}

void RobotechMotor::setMaxCurrent(int cur){
    connection->writeString("^ALIM 1 " + IntToStr(cur * 10) + "\r");
    //connection->writeString("^ALIM 10\r");
    maxAmps = cur;
}
int RobotechMotor::getMaxCurrent(){return maxAmps;}

void RobotechMotor::setAccel(int accel){
    connection->writeString("^MAC 1 "+ IntToStr(accel) + "\r"); //set acceleration
    accelRate = accel;
}
int RobotechMotor::getAccel(){return accelRate;}

void RobotechMotor::setDecel(int decel){
    connection->writeString("^MDEC 1 " + IntToStr(decel) + "\r"); //set deceleration
    decelRate = decel;
}
int RobotechMotor::getDecel(){return decelRate;}

void RobotechMotor::setWatchDogTime(int millisecs) {
    connection->writeString("^RWD " + IntToStr(millisecs) + "\r");
    stallOut = millisecs;
    }
int RobotechMotor::getWatchDogTime() {return stallOut;}
