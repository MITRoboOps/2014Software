#include "ros/ros.h"
#include "std_msgs/String.h"


void callBack(const std_msgs::String::ConstPtr& msg){
	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main( int argc, char **argv){
	ros::init(argc,argv,"subscriber");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("joyOutput", 1000, callBack);
	ros::spin();
	return 0;
}

