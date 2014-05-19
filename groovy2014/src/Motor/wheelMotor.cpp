#include <stdio.h>
#include "Serial.cpp"

class wheelMotor: public MotorController {
    private:
  	Serial serial;
	string deviceID;
    public:
	void setDevID(string newDevID){
		deviceID = newDevID;
	}

 	bool connectToDevice(){
		  //make sure you do try catch and usleep
		  serial = Serial(deviceID, 115200);

		  Serial.open(“/dev/myserialport”);
		  Serial.readTimeout = 500;
	}

	double getVelocity(){

	     //clearbuffer
	     Serial.ReadAll();

	     Serial.writeLine(“!g v”);
	  	 string reply = Serial.readLine();

	     //RoboTeq
	     //sanity check


	     reply = strcat(2,3,reply);
	     return reply;

        }
}
