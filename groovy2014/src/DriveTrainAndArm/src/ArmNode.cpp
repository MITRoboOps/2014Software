#include "ArmClass.h"

//Claw params
int clawRank = 3;
int clawMinPos = 0;
int clawMaxPos = 1000;
int clawMaxVel = 1000;

//Stick params
int stickRank = 1;
int stickMinPos = 0;
int stickMaxPos = 1000;
int stickMaxVel = 1000;

//Thumb params
int thumbMaxClosedPosition = 14000;
int thumbMaxOpenedPosition = 3000;
int thumbMaxVel = 450;
int thumbChannel = 0;

//Pan params
int panMaxClosedPosition = 10000;
int panMaxOpenedPosition = 0;
int panMaxVel = 450;
int panChannel = 1;

//Tilt params
int tiltMaxClosedPosition = 8000;
int tiltMaxOpenedPosition = 0;
int tiltMaxVel = 450;
int tiltChannel = 2;

//Servo params
const char* servoPath = "/dev/ttyACM0";
int servoMaxOpenedPositions[] = {thumbMaxOpenedPosition,panMaxOpenedPosition,tiltMaxOpenedPosition};
int servoMaxClosedPositions[] = {thumbMaxClosedPosition,panMaxClosedPosition,tiltMaxClosedPosition};
int servoMaxVels[] ={thumbMaxVel,panMaxVel,tiltMaxVel};

//Boom params
int boomMaxVel = 400;
int boomAccel = 10000;
int boomDecel = 10000;
int boomMaxAmps = 150;
std::string boomPath = "/dev/ttyUSB4";

//Arm Rotation Params
int armRotMaxVel = 150;
int armRotAccel = 10000;
int armRotDecel = 10000;
int armRotMaxAmps = 150;
std::string armRotPath = "/dev/ttyUSB5";

//Universal WatchDog Time
int watchDog = 300;
int servoWatchDog = 100;

int main( int argc, char **argv){

	ros::init(argc,argv,"arm");
	std::cout << "init successful" << std::endl;
	ArmClass *AC = new ArmClass( clawRank, clawMinPos, clawMaxPos, clawMaxVel, stickRank, stickMinPos, stickMaxPos, stickMaxVel, servoMaxOpenedPositions, servoMaxClosedPositions, servoMaxVels, thumbChannel, panChannel, tiltChannel, servoPath, boomMaxVel, boomAccel, boomDecel, boomMaxAmps, boomPath, armRotMaxVel, armRotAccel, armRotDecel, armRotMaxAmps, armRotPath,  watchDog, servoWatchDog);
	ros::spin();
	return 0;
}

