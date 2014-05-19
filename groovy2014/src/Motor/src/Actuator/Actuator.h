//header for Actuator
#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "firgelli6/firgelli.h"
#include <string>

//class Actuator;


class Actuator
{
private:
    int rank;
    int velocity;
    Firgelli firgelli;

    void changeVel(int newSpeed);


public:

    Actuator(int myRank);

    void setRank(int newRank);

    bool connectToDevice();

    double getPosition();

    double getVelocity();

    //Don't know for actuator
    double getCurrent();


    void setPosition(double pos);

    //takes in all ints
    void setVelocity(double vel);

    void setCurrent(double current);

    bool getStatus();

    std::string getStatusString();


};

#endif
