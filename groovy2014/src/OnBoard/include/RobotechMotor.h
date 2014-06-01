
#ifndef ROBOTECHMOTOR_H
#define ROBOTECHMOTOR_H
#include "Serial.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "stdlib.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <exception>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <list>
#include <exception>

//class RobotechMotor;

class RobotechMotor {

private:
 //   Serial* connection;
    Serial* connection;

    std::string path;

    int mode, maxRPM, stallOut, accelRate, decelRate, maxAmps;

    bool isOpenLoopVel;


    std::mutex getStatus;

public:
    std::list<char*> statusMessages;

    bool velMode;

    //accel/decel should be on order 1500
    RobotechMotor(int maxVelocity, int millisecs, int accel, int decel, int maxCurrent, bool isOpenLoopVelocity, std::string pathToUsb);

    std::string IntToStr(int x);

    bool connectToDevice();

    int getPosition();

    int getFeedBack();

    int getVelocity();

    //Don't know for actuator
    int getCurrent();

    int getBatteryVoltage();

    int* getTemp();

    void setAbsolutePosition(double pos);

    void setRelativePosition(double pos);

    //takes in all ints
    void setVelocity(double vel);

    //Set Constant Parameters

    void setMaxPositionModeVelocity(int vel);

    void setMaxVelocity(int vel);
    int getMaxVelocity();

    void setMaxCurrent(int cur);
    int getMaxCurrent();

    void setAccel(int accel);
    int getAccel();

    void setDecel(int decel);
    int getDecel();

    void setWatchDogTime(int millisecs);
    int getWatchDogTime();

    void readValues();

    char* getStatusMessage(char* identifier);

    bool recievedMessage(char* msg);


    //leaves motor in open loop speed mode
    void resetCounter();

    void setPGain(int gain); //sets to gain /100
    void setIGain(int gain); //sets to gain /100
    void setDGain(int gain); //sets to gain/100

    void changeToOpenLoopVel();
};







#endif
