#include <ros/ros.h>
#include <ros/node_handle.h>
#include "std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sensor_msgs/Joy.h>
#include "Actuator.h"



class ArmClass {

private:

    // Node Handler
    ros::NodeHandle nh;

    // Subscribers
    ros::Subscriber leftSub;
    ros::Subscriber rightSub;

    // Publishers
    ros::Publisher pointPub;

public:

    ArmClass()

};
