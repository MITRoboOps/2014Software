#include "DriveTrainClass.h"
#define LOOPRATE 10
#define PUBLISHRATE 1


int main( int argc, char **argv){

	ros::init(argc,argv,"DriveTrain");
	std::cout << "init successful" << std::endl;
	DriveTrainClass *DTC = new DriveTrainClass("/dev/ttyUSB0","/dev/ttyUSB3","/dev/ttyUSB1","/dev/ttyUSB2");
	ros::Rate loop_rate(LOOPRATE);
	int counter = 0;
	while(ros::ok()){
		if(counter%(PUBLISHRATE*LOOPRATE) == 0){
			DTC->publishStatusMessage();
			counter = 0;
		}
		counter++;
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}

