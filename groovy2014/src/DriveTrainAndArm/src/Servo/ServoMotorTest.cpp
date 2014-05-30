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
    int closedLim[] = {8000,8000,8000};
    int openedLim[] = {3000,3000,3000};
    int maxVel[] = {500,500,500};
    ServoMotor *robo = new ServoMotor(closedLim,openedLim,maxVel, 40);
    cout << "successful initialization" << endl;


    if(robo->connectToDevice("/dev/ttyACM0")){
        cout << "successful connect" << endl;
    }

    robo->setAbsolutePosition(6000,0);

    if(testPosition){
        for(int i = 2500; i < 8000; i += 100){
            robo->setAbsolutePosition(i,0);
            printf("%i\n",i);
            usleep(100*1000);
        }
    }

    if(testVelocity){
        robo->setAbsolutePosition(5000,1);
        sleep(1);
        for(int i =0; i < 4000; i ++){
            robo->setVelocity(150,1);
            usleep(40*1000);
        }

       // robo->setVelocity(0);
    }

}
