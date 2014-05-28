#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ServoMotor.h"

using namespace std;

int main(){
    bool testPosition = false;
    bool testVelocity = true;

    cout << "successful run" << endl;
    ServoMotor *robo = new ServoMotor(8000,3000,500, 100);
    cout << "successful initialization" << endl;


    if(robo->connectToDevice("/dev/ttyACM0")){
        cout << "successful connect" << endl;
    }


    if(testPosition){
        for(int i = 2500; i < 8000; i += 100){
            robo->setAbsolutePosition(i);
            printf("%i\n",i);
            usleep(100*1000);
        }
    }

    if(testVelocity){
        robo->setAbsolutePosition(5000);
        sleep(1);
        for(int i =0; i < 4000; i ++){
            robo->setVelocity(20);
            usleep(20*1000);
        }

        robo->setVelocity(0);
    }

}
