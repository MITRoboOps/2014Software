#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "RobotechMotor.h"

using namespace std;

int main(){
    cout << "successful run" << endl;
    RobotechMotor *robo = new RobotechMotor();
    cout << "successful initialization" << endl;


    if(robo->connectToDevice("/dev/ttyUSB1")){
        cout << "successful connect" << endl;
    }

    robo->setVelocity(500);
    sleep(1*1000000);
    robo->setVelocity(0);

}
