#include "ArmClass.h"



int main( int argc, char **argv){

	ros::init(argc,argv,"arm");
	std::cout << "init successful" << std::endl;
	ArmClass *AC = new ArmClass(2, 0, 1000,10000, 1, 0, 1000, 10000, 8000, 2000, 10000,"/dev/ttyACM0",400,1500,1500,"/dev/ttyUSB4", 150, 1500,1500,"/dev/ttyUSB5", 100);
	ros::spin();
	return 0;
}

