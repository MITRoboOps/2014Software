
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

//class RobotechMotor;

class RobotechMotor {

private:
 //   Serial* connection;
    Serial* connection;

    int mode, maxRPM, stallOut, accelRate, decelRate;

public:

    bool velMode;

    //accel/decel should be on order 1500
    RobotechMotor(int maxVelocity, int millisecs, int accel, int decel);

    std::string IntToStr(int x);

    bool connectToDevice(std::string pathToUSB);

    double getPosition();

    double getVelocity();

    //Don't know for actuator
    double getCurrent();


    void setAbsolutePosition(double pos);

    void setRelativePosition(double pos);

    //takes in all ints
    void setVelocity(double vel);

    void setCurrent(double current);

    bool getStatus();

    std::string getStatusString();
};







#endif
