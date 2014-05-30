
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

//class RobotechMotor;

class RobotechMotor {

private:
 //   Serial* connection;
    Serial* connection;

    int mode, maxRPM, stallOut, accelRate, decelRate, maxAmps;

    std::list<char*> statusMessages;
    std::mutex getStatus;

public:

    bool velMode;

    //accel/decel should be on order 1500
    RobotechMotor(int maxVelocity, int millisecs, int accel, int decel, int maxCurrent);

    std::string IntToStr(int x);

    bool connectToDevice(std::string pathToUSB);

    double getPosition();

    double getVelocity();

    //Don't know for actuator
    double getCurrent();

    double getBatteryVoltage();

    void setAbsolutePosition(double pos);

    void setRelativePosition(double pos);

    //takes in all ints
    void setVelocity(double vel);

    //Set Constant Parameters

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

    char* getStatusMessage(char identifier);

};







#endif
