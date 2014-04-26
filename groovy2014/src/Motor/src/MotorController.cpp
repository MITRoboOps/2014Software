#include <stdio.h>
#include <iostream>
  
using namespace std;

class MotorController {
	protected:
    	bool status = true;
        string deviceID;
  	public:
 		//returns true if successfully connected
  		virtual bool connectToDevice(string devID) =0;
  		//getter methods
		virtual double getVelocity() =0;
  		virtual double getPosition() =0;
  		virtual double getCurrent() =0;
  		//setter methods :
  		//		return true if successfully set parameter
  		//		return false otherwise
  		virtual void setVelocity(double vel) =0;
  		virtual void setPosition(double pos) =0;
  		virtual void setCurrent(double current) =0;
  		//returns status
  		virtual bool getStatusBool() =0;
  		//returns error message or empty string
  		virtual string getStatusString() =0;
  		
}