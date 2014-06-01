#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "../../include/ServoMotor.h"

using namespace std;

int main(){
    bool testPosition = false;
    bool testVelocity = false;

    cout << "successful run" << endl;
    int closedLim[] = {8200,14000,14000};
    int openedLim[] = {5000,000,000};
    int maxVel[] = {1000,1000,1000};
    ServoMotor *robo = new ServoMotor(closedLim,openedLim,maxVel, 100);
    cout << "successful initialization" << endl;


    if(robo->connectToDevice("/dev/ttyACM0")){
        cout << "successful connect" << endl;
    }

    //robo->closeS(0);
    robo->setAbsolutePosition(7000,0);

    if(testPosition){
        for(int i = 0; i < 14000; i += 100){
            robo->setAbsolutePosition(i,0);
            printf("%i\n",i);
            usleep(20*1000);
        }
    }

    if(testVelocity){
        robo->setAbsolutePosition(5000,1);
        sleep(1);
        for(int i =0; i < 4000; i ++){
            robo->setVelocity(-500,0);
            usleep(40*1000);
        }

       // robo->setVelocity(0);
    }

}
