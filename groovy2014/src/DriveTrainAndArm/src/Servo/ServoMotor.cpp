#include "../../include/ServoMotor.h"





using namespace std;

//Mode 0 : closed loop speed
//Mode 1 : count position mode (for setting absolute position)
//Mode 2 : relative position mode



ServoMotor::ServoMotor(int* closedPositionLimit, int* openedPositionLimit, int* velocityMagnitudeLimit, int millisecs){
    closedPosLim = closedPositionLimit;
    openedPosLim = openedPositionLimit;
    velMagLim = velocityMagnitudeLimit;
    stallTime = millisecs;
    currentPosition = (int*)malloc(sizeof(closedPositionLimit));
    for(int i =0; i < sizeof(currentPosition)/sizeof(currentPosition[0]) + 1;i++){
        currentPosition[i] = 5000;
        cout << "init " << i << " " << closedPosLim[i] << " " << openedPosLim[i] << endl;
    }
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

double ServoMotor::getPosition(int channel){

return 0.0;
}


void ServoMotor::setAbsolutePosition(unsigned short pos,int channel){

  unsigned short actualPos = max((int) pos,openedPosLim[channel]);
  actualPos = min((int) actualPos, closedPosLim[channel]);

  unsigned char command[] = {0x84, channel, actualPos & 0x7F, actualPos >> 7 & 0x7F};


  if (write(fd, command, sizeof(command)) == -1)
  {
    printf("could not set this position");
  }
  else currentPosition[channel] = actualPos;
}

void ServoMotor::setRelativePosition(double pos,int channel){
}

//positive velocity means closing, negative means opening
void ServoMotor::setVelocity(int vel, int channel){
    if((vel < -velMagLim[channel] || vel > velMagLim[channel])){
        printf("velocity outside limits");
        return;
    }
    unsigned short velToController =1;
    if(vel < 0){
        velToController = -vel;
    }
    else velToController = vel;

    int distToTravel = (int) (vel * 10* ((double)stallTime)/1000.0);
    int newPos = currentPosition[channel] + distToTravel;
    unsigned short newActualPos = newPos;
    if(newPos < 0) newActualPos = 0;

    printf("Channel: is %i, Velocity is: %i, Distance To Travel is : %i, New Actual Position is : %i, CurrentPos : %i\n",channel, velToController,distToTravel,newPos,currentPosition[channel]);


    unsigned char speedCommand[] = {0x87, channel, velToController & 0x7F, velToController >> 7 & 0x7F};
        if (write(fd, speedCommand, sizeof(speedCommand)) == -1){
            printf("could not set this velocity");
        }

    setAbsolutePosition(newActualPos,channel);
}


int ServoMotor::getMaxVel(int channel){
    return velMagLim[channel];
     }
