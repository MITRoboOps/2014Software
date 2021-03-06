#include <ros/ros.h>
#include <ros/node_handle.h>
#include "std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <Messages/DriveTrainMessage.h>
#include <Messages/DriveTrainStatusMessage.h>
#include "RobotechMotor.h"

class DriveTrainClass {

private:
    // Node Handler
    ros::NodeHandle nh;

    // Subscribers
    ros::Subscriber leftSub;
    ros::Subscriber rightSub;
    ros::Subscriber sub;

    // Publishers
    ros::Publisher statusPublisher;

    Messages::DriveTrainStatusMessage status;


public:

    DriveTrainClass(std::string BLPath, std::string BRPath, std::string FLPath, std::string FRPath);


    RobotechMotor *BR;
    RobotechMotor *BL;
    RobotechMotor *FR;
    RobotechMotor *FL;

// Callbacks
    void callBack(const Messages::DriveTrainMessage::ConstPtr& msg);
	
    void publishStatusMessage();

    void resetStatusMessage();

};
