#include "Actuator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>




using namespace std;

//cpp

void Actuator::changeVel(int newSpeed)
{
    firgelli.WriteCode(33,newSpeed);
}

Actuator::Actuator(int myRank, int minPos, int maxPos, int maxVelMag, int millisecs)
{
    minPosition = minPos;
    maxPosition = maxPos;
    maxVelocityMagnitude = maxVelMag;
    stallTime = millisecs;
    setRank(myRank);
}


void Actuator::setRank(int newRank)
{
    rank = newRank;
}

bool Actuator::connectToDevice()
{
    try
    {
        firgelli.Open(rank);
        return true;
    }
    catch (exception& e)
    {
        return false;
    }
}

double Actuator::getVelocity()
{
    return velocity;
}

double Actuator::getPosition()
{
    return (double) firgelli.WriteCode(16,0);
}

double Actuator::getCurrent()
{
    //TODO
    return 0.0;
}


void Actuator::setPosition(double pos)
{
    int actualPos = pos;
    if(pos < minPosition) actualPos = minPosition;
    if(pos > maxPosition) actualPos = maxPosition;
    int lastPos = firgelli.WriteCode(32, (int) (actualPos + 0.5));
}

//takes in all ints
void Actuator::setVelocity(double vel)
{
    velocity = vel;
    int actualVel = (int) (vel + 0.5); //this makes it round correctly

    if(actualVel > maxVelocityMagnitude) actualVel = maxVelocityMagnitude;
    if(actualVel < -maxVelocityMagnitude) actualVel = -maxVelocityMagnitude;

    if(vel == 0)
    {
        setPosition(getPosition());
        changeVel(actualVel);
    }
    else if(vel > 0)
    {
	int newPos = (int) getPosition() + vel * ((double)stallTime)/1000.0;
        setPosition(newPos);
        changeVel(actualVel);
    }
    else
    {
	int newPos = (int) getPosition() + vel * ((double)stallTime)/1000.0;
        if(newPos < 0) setPosition(0);
        else setPosition(newPos);
        changeVel(-actualVel);
    }
}

void Actuator::setCurrent(double current)
{
    //TODO
}

bool Actuator::getStatus()
{
    //TODO
    return true;
}

string Actuator::getStatusString()
{
    //TODO
    return "";
}
//header

int Actuator::getMaxVel(){
	return maxVelocityMagnitude;
}



