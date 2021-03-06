#include <ros/ros.h>
#include <ros/node_handle.h>
#include <sensor_msgs/Joy.h>
#include "Serial.cpp"

class JoyToPoint {

private:
    // Node Handler
    ros::NodeHandle nh;

    // Subscribers
    ros::Subscriber joySub;

    // Publishers
    ros::Publisher pointPub;

    // Messages
    sensor_msgs::Joy joy;

public:
	Serial *serialfl;
	Serial *serialfr;
	Serial *serialbl;
	Serial *serialbr;

    JoyToPoint(ros::NodeHandle nh_);
    ~JoyToPoint(){}
	
	
    // Callbacks
    void joyCallback( const std_msgs::String::ConstPtr& msg );

};
