#include "../../include/Actuator.h"





using namespace std;

//cpp

void Actuator::changeVel(int newSpeed)
{
    try{
        firgelli.WriteCode(33,newSpeed);
    }catch (exception& e){
            cout << "connection failed. trying again" << endl;
            sleep(3);
            connectToDevice();
        }
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

void Actuator::connectToDevice()
{
    int count = 0;
    int numberToTry = 5;
    while(count < numberToTry){
        try{
        firgelli.Open(rank);
        break;
        } catch (exception& e){
            std::cout << "connection failed, trying again" << std::endl;
            count++;
            continue;
        }
    }
}

int Actuator::getVelocity()
{
    return velocity;
}

int Actuator::getPosition()
{
    int toReturn = 0;
    try{
        toReturn = firgelli.WriteCode(16,0);
    } catch (exception& e){
            connectToDevice();
        }
    return toReturn;
}

void Actuator::setPosition(double pos)
{
    int actualPos = pos;
    if(pos < minPosition) actualPos = minPosition;
    if(pos > maxPosition) actualPos = maxPosition;
    try{
        changeVel(maxVelocityMagnitude);
        firgelli.WriteCode(32, (int) (actualPos + 0.5));
    } catch (exception& e){
            connectToDevice();
        }
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



