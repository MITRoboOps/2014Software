#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>


int main(int argc, char **argv){

	ros::init(argc, argv, "publisher");
	ros::NodeHandle n;
	ros::Publisher pub1 = n.advertise<std_msgs::String>("joyOutput", 1000);
	ros::Rate loop_rate(10);

	while(ros::ok()){
		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world";
		msg.data = ss.str();
		ROS_INFO("%s", msg.data.c_str());

		pub1.publish(msg);
		
		ros::spinOnce();
	
		loop_rate.sleep();
	}
	return 0;
}


