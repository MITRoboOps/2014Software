#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "../../include/RobotechMotor.h"

using namespace std;

int main(){
    bool testSpeed = true;
    bool testPosition = false;
    bool testMemcpy = false;
    bool testRecievedMsg = true;
    cout << "successful run" << endl;
    RobotechMotor *robo = new RobotechMotor(100, 100,1000,1000,400,true,"/dev/ttyUSB7" );
    cout << "successful initialization" << endl;

    if(robo->connectToDevice()){
        cout << "successful connect" << endl;
    }

    robo->setPGain(110);
    robo->setIGain(3);
    robo->setDGain(3);

    //robo->resetCounter();
    if(testPosition){
        robo->setAbsolutePosition(14000);
    }

    if(testSpeed){
       for(int i =1; i < 6000; i++){
               //
               robo->setVelocity(-150);
              //  cout << "set Vel" << endl;
                if(i% 5 == 0) {
                    //robo->getPosition();
                   // robo->getFeedBack();
                }
                usleep(50*1000);
            }

            sleep(1);
            cout << "after sleep" << endl;
    }

    if(testMemcpy){
        char* msg = "T123adj";
        char* newMsg = (char*) malloc(sizeof(char)*3);
        char* id = (char*) malloc(sizeof(char)*3);

        memcpy(id, msg + 1, 3);
        cout << id << endl;
        memcpy(newMsg,msg+4,3);
        msg = newMsg;
        char sendBack[5] = {' ',' ',' ',' ',' '};

        memcpy(sendBack + 2, id, 3);
        cout << msg << endl;
       // cout << sendBack[2] << id[0] << endl;
        //sendBack[2] = id[1];
        //sendBack[3] = id[1];
        //sendBack[4] = id[2];
        cout << sendBack << endl;
    }

    if(testRecievedMsg){
        robo->statusMessages.push_front("a");
        if(robo->recievedMessage("adshjf")) cout << "successful compare" << endl;

    }

}
