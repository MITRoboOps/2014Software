#include "DriveTrainClass.h"



int main( int argc, char **argv){

	ros::init(argc,argv,"driveTrain");
	std::cout << "init successful" << std::endl;
	DriveTrainClass *DTC = new DriveTrainClass("/dev/ttyUSB0","/dev/ttyUSB3","/dev/ttyUSB1","/dev/ttyUSB2");
	ros::spin();
	return 0;
}

