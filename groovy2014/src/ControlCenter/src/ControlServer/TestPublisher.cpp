#include "ros/ros.h"
#include "std_msgs/String.h"
#include <Messages/DriveTrainMessage.h>

#include <sstream>


int main(int argc, char **argv){

	ros::init(argc, argv, "publisher");
	ros::NodeHandle n;
	ros::Publisher pub1 = n.advertise<std_msgs::String>("leftDriveTrain", 1);
	ros::Publisher pub2 = n.advertise<std_msgs::String>("rightDriveTrain", 1);
	ros::Publisher pub3 = n.advertise<Messages::DriveTrainMessage>("DriveTrain",1);
	ros::Rate loop_rate(10);

	int left = 0;
	int right = 1000;

	while(ros::ok()){
		std_msgs::String msgLeft;
		std_msgs::String msgRight;
		std::stringstream ssLeft;
		std::stringstream ssRight;

		Messages::DriveTrainMessage msg;

		msg.leftVelocity = 300;
		msg.rightVelocity = 400;		

		ssLeft << "300";
		ssRight << "300";
		msgLeft.data = ssLeft.str();
		msgRight.data = ssRight.str();

		left++;
		right--;		

		pub1.publish(msgLeft);
		pub2.publish(msgRight);
		pub3.publish(msg);
		
		ros::spinOnce();
	
		loop_rate.sleep();
	}
	return 0;
}

