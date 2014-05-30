
#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "stdlib.h"
#include <iostream>
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

    int fd;
    int* currentPosition;
    int* closedPosLim;
    int* openedPosLim;
    int* velMagLim;
    int stallTime;
public:

    bool velMode;

    /*
    millisecs refers to the amount of milliseconds the servo should remain activated after a velocity command has been issued
    */
    ServoMotor(int * closedPositionLimit, int* openedPositionLimit, int* velocityMagnitudeLimit, int millisecs);

    std::string IntToStr(int x);

    bool connectToDevice(const char* pathToUSB);

    double getPosition(int channel);

    double getVelocity(int channel);

    void setAbsolutePosition(unsigned short pos, int channel);

    void setRelativePosition(double pos, int channel);

    //takes in all ints
    void setVelocity(int vel, int channel);

    int getMaxVel(int channel);
};







#endif
