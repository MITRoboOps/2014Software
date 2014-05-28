#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "RobotechMotor.h"

using namespace std;

int main(){
    cout << "successful run" << endl;
    RobotechMotor *robo = new RobotechMotor(400, 100,1500,1500);
    RobotechMotor *robo1 = new RobotechMotor(150,100,1500,1500);
    cout << "successful initialization" << endl;



    if(robo->connectToDevice("/dev/ttyUSB4")){
        cout << "successful connect" << endl;
    }
    robo1->connectToDevice("/dev/ttyUSB1");

    robo->setVelocity(500);
    for(int i =0; i < 200; i++){
        robo->setVelocity(-500);
       // robo1->setVelocity(-500);
        usleep(100*1000);
    }

    sleep(1);
    cout << "after sleep" << endl;
 //   robo->setRelativePosition(100);

}
