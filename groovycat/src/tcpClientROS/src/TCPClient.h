#include <ros/ros.h>
#include <ros/node_handle.h>
#include <sensor_msgs/Joy.h>

class TCPClient {

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

    JoyToPoint(ros::NodeHandle nh_);
    ~JoyToPoint(){}
	
	
    // Callbacks
    void joyCallback( const sensor_msgs::Joy::ConstPtr& j );

};
