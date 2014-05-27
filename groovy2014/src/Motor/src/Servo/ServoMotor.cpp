#include "ServoMotor.h"





using namespace std;

//Mode 0 : closed loop speed
//Mode 1 : count position mode (for setting absolute position)
//Mode 2 : relative position mode



ServoMotor::ServoMotor(int closedPositionLimit, int openedPositionLimit, int velocityMagnitudeLimit, int millisecs){
    closedPosLim = closedPositionLimit;
    openedPosLim = openedPositionLimit;
    velMagLim = velocityMagnitudeLimit;
    stallTime = millisecs;
}

std::string ServoMotor::IntToStr(int x){
        std::ostringstream out;
        out << x;
        return out.str();
}


bool ServoMotor::connectToDevice(const char* pathToUSB){
    int numTries = 0;
    int numberToTry = 5;
    while(numTries < numberToTry)
    try{
        fd = open(pathToUSB, O_RDWR | O_NOCTTY);
        numTries = numberToTry;
        } catch (exception& e) {
			std::cout << "Connection failed. Retrying " << numberToTry - numTries << " more times." << endl;
			if(numTries == numberToTry) return false;
			numTries++;
			usleep (3000*1000);
		}

    return true;


}

double ServoMotor::getPosition(){

return 0.0;
}


void ServoMotor::setAbsolutePosition(unsigned short pos){

  unsigned short actualPos = pos;

  if(pos < openedPosLim){
    actualPos = openedPosLim;
  }
  if(pos > closedPosLim){
    actualPos = closedPosLim;
  }

  unsigned char command[] = {0x84, 0, actualPos & 0x7F, actualPos >> 7 & 0x7F};


  if (write(fd, command, sizeof(command)) == -1)
  {
    printf("could not set this position");
  }
  else currentPosition = pos;
}

void ServoMotor::setRelativePosition(double pos){
}

//positive velocity means closing, negative means opening
void ServoMotor::setVelocity(int vel){
    if((vel < -velMagLim || vel > velMagLim)){
        printf("velocity outside limits");
        return;
    }
    unsigned short velToController =0;
    if(vel < 0){
        velToController = -vel;
    }
    else velToController = vel;

    int distToTravel = vel * 10* stallTime;
    int newPos = currentPosition + distToTravel;
    unsigned short newActualPos = newPos;
    if(newPos < 0) newActualPos = 0;

    unsigned char speedCommand[] = {0x87, 0, velToController & 0x7F, velToController >> 7 & 0x7F};
        if (write(fd, speedCommand, sizeof(speedCommand)) == -1){
            printf("could not set this velocity");
        }

    setAbsolutePosition(newActualPos);


}

bool ServoMotor::getStatus(){
//TODO
    return true;
}

std::string ServoMotor::getStatusString(){
    //TODO
    return "";
}
