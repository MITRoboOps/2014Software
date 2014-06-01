#include "CameraBoomClass.h"
#define LOOPRATE 10
#define PUBLISHRATE 1

//Params
int downPos = 0;
int upPos = 920;
int maxVel = 1000;
int rank = 2;


int main( int argc, char **argv){

	ros::init(argc,argv,"CameraBoom");
	ros::NodeHandle n;
	ros::Rate loop_rate(LOOPRATE);
	std::cout << "init successful" << std::endl;
	CameraBoomClass *cboom = new CameraBoomClass(downPos,upPos,maxVel,rank);
	int counter = 0;
	while(ros::ok()){
		if(counter%(PUBLISHRATE*LOOPRATE) == 0){
			cboom->publishStatusMessage();
			counter = 0;
		}
		counter++;
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
