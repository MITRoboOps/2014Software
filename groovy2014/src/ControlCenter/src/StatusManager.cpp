#include <iostream>
#include <ctime>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <thread>
#include <mutex>

//global time
std::clock_t initTime = std::clock();


double getRunTime(){
	return (std::clock() - initTime)/(double) CLOCKS_PER_SEC;
}

int main(int argc, char **argv){

	ros::init(argc, argv, "StatusManager");
	ros::NodeHandle n;

	//subscribers
	//ros::Subscriber
}
