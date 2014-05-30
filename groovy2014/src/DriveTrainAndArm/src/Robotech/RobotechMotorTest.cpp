#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "../../include/RobotechMotor.h"

using namespace std;

int main(){
    cout << "successful run" << endl;
    RobotechMotor *robo = new RobotechMotor(60, 100,10000,10000,400);
    RobotechMotor *robo1 = new RobotechMotor(150,100,1500,1500,-1);
    cout << "successful initialization" << endl;



    if(robo->connectToDevice("/dev/ttyUSB0")){
        cout << "successful connect" << endl;
    }
   // robo1->connectToDevice("/dev/ttyUSB1");

    robo->setVelocity(500);
    for(int i =1; i < 1000; i++){
        robo->setVelocity(-1000);
        if(i% 5 == 0) robo->getBatteryVoltage();
        usleep(20*1000);
    }

    sleep(1);
    cout << "after sleep" << endl;
 //   robo->setRelativePosition(100);

}
