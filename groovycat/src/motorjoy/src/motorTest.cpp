#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/Joy.h>
#include "motorTest.h"
#include "stdlib.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <exception>


const int TURN_RATE = 75; //pick # 0-100
const int EXPO = 5; //exponential
const int PEAK = 650; //max power

using namespace std;

template <typename ROFL>
string IntToStr(ROFL tmp)
{
    std::ostringstream out;
    out << tmp;
    return out.str();
}

JoyToPoint::JoyToPoint(ros::NodeHandle nh_) : nh(nh_) {

	std::cout << "Opening serial ports..." << endl;
	 //open serial ports
	bool connected = false;
	while (!connected)
	{
		try {
			
			serialfl = new Serial("/dev/ttyUSB0",115200);
			serialfr = new Serial("/dev/ttyUSB1",115200);
			serialbl = new Serial("/dev/ttyUSB2",115200);
			serialbr = new Serial("/dev/ttyUSB3",115200);
			connected = true;
		} catch (exception& e) {
			std::cout << "Connection failed. Retrying..." << endl;
			usleep (3000*1000);
		}
	}

	std::cout << "Setting controller modes..." << endl;

	//Set up motor in correct mode
	serialbl->writeString("^ECHOF 1\r"); //echo off
	serialbl->writeString("^MMOD 1 0\r");//open loop
	serialbr->writeString("^ECHOF 1\r"); //echo off
	serialbr->writeString("^MMOD 1 0\r");//open loop
	serialfl->writeString("^ECHOF 1\r"); //echo off
	serialfl->writeString("^MMOD 1 0\r");//open loop
	serialfr->writeString("^ECHOF 1\r"); //echo off
	serialfr->writeString("^MMOD 1 0\r");//open loop
	
	std::cout << "Subscribing to ROS topic..." << endl;


	// Subscribe to input topics.
	joySub = nh.subscribe("joyOutput", 1, &JoyToPoint::joyCallback, this);

	// Get Parameters
	ros::NodeHandle local_nh("~");
	
	std::cout << "\r\r Motor Joy running." << endl;

}

void JoyToPoint::joyCallback( const std_msgs::String::ConstPtr& msg ) {
    // Convert Joystick message to a virtual point
  float spd, dir, left, right;

  std::istringstream ss(msg->data.c_str());
  char delim[2] = {',',';'};

  ss >> dir >> delim[0];
  ss >> spd >> delim[1];
  dir/=-100;
  spd/=100;
 // spd = j->axes[1];
  //dir = j->axes[0];
  spd = ((pow(spd,3) * (EXPO-1) + spd ) / EXPO);
  dir = ((pow(dir,3) * (EXPO-1) + dir ) / EXPO) * (TURN_RATE/100.0);
  
  std::cout << "spd: " << spd << "dir: " << dir << "\n";  

  left = (spd - dir);
  right = (spd + dir);
  if(abs(left)>1)
	left/=(abs(spd)+abs(dir));
  else if(abs(right)>1)
	right/=(abs(spd)+abs(dir));

  left*=PEAK;
  right*=PEAK;

  std::cout << "Left: " << IntToStr(left) << " Right: " << IntToStr(right) << "\n";
  
  //if(j->buttons[0] == 1){
	//std::cout << "pew pew" << "\n";
 // }
 
  serialfl->writeString("!g 1 "+IntToStr(left)+"\r");
  serialfr->writeString("!g 1 "+IntToStr(right)+"\r");
  serialbl->writeString("!g 1 "+IntToStr(left)+"\r");
  serialbr->writeString("!g 1 "+IntToStr(right)+"\r"); 
  
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_to_point");
  ros::NodeHandle nh;

  JoyToPoint point(nh);

  ros::spin();
  return 0;
}
