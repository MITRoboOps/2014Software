#include "ArmClass.h"
#define LOOPRATE 10
#define PUBLISHRATE 1

bool testing = false;
bool stow = false;
bool drop = false;
bool rdy = false;

//Claw params
int clawRank = 1;
int clawMinPos = 0;
int clawMaxPos = 1000;
int clawMaxVel = 1000;

//Stick params
int stickRank = 3;
int stickMinPos = 0;
int stickMaxPos = 1000;
int stickMaxVel = 1000;

//Thumb params
int thumbMaxClosedPosition = 8200;
int thumbMaxOpenedPosition = 5000;
int thumbMaxVel = 550;
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
int boomMaxVel = 1500;
int boomAccel = 15000;
int boomDecel = 15000;
int boomMaxAmps = 300;
std::string boomPath = "/dev/ttyUSB4";

//Arm Rotation Params
int armRotMaxVelM = 50;
int armRotAccelM = 1000;
int armRotDecelM = 1000;
int armRotMaxVelA = 16;
int armRotAccelA = 1000;
int armRotDecelA = 1000;
int armRotMaxAmps = 4;
int armPgain = 110;
int armIgain = 3;
int armDgain = 3;

std::string armRotPath = "/dev/ttyUSB5";

//Universal WatchDog Time
int watchDog = 500;
int servoWatchDog = 200;

int main( int argc, char **argv){

	ros::init(argc,argv,"Arm");
	ros::NodeHandle n;
	ros::Rate loop_rate(LOOPRATE);
	std::cout << "init successful" << std::endl;
	ArmClass *AC = new ArmClass( clawRank, clawMinPos, clawMaxPos, clawMaxVel, stickRank, stickMinPos, stickMaxPos, stickMaxVel, servoMaxOpenedPositions, servoMaxClosedPositions, servoMaxVels, thumbChannel, panChannel, tiltChannel, servoPath, boomMaxVel, boomAccel, boomDecel, boomMaxAmps, boomPath, armRotMaxVelM, armRotAccelM, armRotDecelM, armRotMaxVelA, armRotAccelA, armRotDecelA, armRotMaxAmps, armRotPath,  watchDog, servoWatchDog, testing);
	AC->armRot->setPGain(armPgain);
	AC->armRot->setIGain(armIgain);
	AC->armRot->setDGain(armDgain);
	int counter = 0;
	if(!testing){
		while(ros::ok()){
			if(counter%(PUBLISHRATE*LOOPRATE) == 0){
				AC->publishStatusMessage();
				counter = 0;
			}
			counter++;
			ros::spinOnce();
			loop_rate.sleep();
		}
		return 0;
	}
	else if(stow){
		AC->startAutomatedAction(0);
	} else if(drop){
		AC->startAutomatedAction(2);
	} else if(rdy){
		AC->startAutomatedAction(1);
	}
}

