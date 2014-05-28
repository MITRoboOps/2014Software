#include "ArmClass.h"

//Claw params
int clawRank = 2;
int clawMinPos = 0;
int clawMaxPos = 1000;
int clawMaxVel = 1000;

//Stick params
int stickRank = 1;
int stickMinPos = 0;
int stickMaxPos = 1000;
int stickMaxVel = 1000;

//Thumb params
int thumbMaxClosedPosition = 8000;
int thumbMaxOpenedPosition = 3000;
int thumbMaxVel = 20;
const char* thumbPath = "/dev/ttyACM0";

//Boom params
int boomMaxVel = 400;
int boomAccel = 10000;
int boomDecel = 10000;
std::string boomPath = "/dev/ttyUSB4";

//Arm Rotation Params
int armRotMaxVel = 150;
int armRotAccel = 10000;
int armRotDecel = 10000;
std::string armRotPath = "/dev/ttyUSB5";

//Universal WatchDog Time
int watchDogStallTime = 100;


int main( int argc, char **argv){

	ros::init(argc,argv,"arm");
	std::cout << "init successful" << std::endl;
	ArmClass *AC = new ArmClass(clawRank, clawMinPos, clawMaxPos, clawMaxVel, stickRank, stickMinPos, stickMaxPos, stickMaxVel, thumbMaxClosedPosition, thumbMaxOpenedPosition, thumbMaxVel,  thumbPath, boomMaxVel, boomAccel, boomDecel, boomPath, armRotMaxVel, armRotAccel, armRotDecel, armRotPath, watchDogStallTime);
	ros::spin();
	return 0;
}

