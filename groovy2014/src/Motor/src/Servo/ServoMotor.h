
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
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

//class RobotechMotor;

class ServoMotor {

private:
 //   Serial* connection;

    int fd;
    int currentPosition;
    int closedPosLim, openedPosLim, velMagLim;
    int stallTime;
public:

    bool velMode;

    /*
    millisecs refers to the amount of milliseconds the servo should remain activated after a velocity command has been issued
    */
    ServoMotor(int closedPositionLimit, int openedPositionLimit, int velocityMagnitudeLimit, int millisecs);

    std::string IntToStr(int x);

    bool connectToDevice(const char* pathToUSB);

    double getPosition();

    double getVelocity();

    void setAbsolutePosition(unsigned short pos);

    void setRelativePosition(double pos);

    //takes in all ints
    void setVelocity(int vel);

    bool getStatus();

    std::string getStatusString();
};







#endif
