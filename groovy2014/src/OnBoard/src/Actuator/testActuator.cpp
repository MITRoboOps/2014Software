#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "../../include/Actuator.h"

using namespace std;

int main()
{

    bool testVelocity = false;
    bool testPosition = true;
    bool getValues = true;

    cout << "successful run" << endl;
    Actuator *myAct1 = new Actuator(1,0,1023,1000,100);
    cout << "successful initialization" << endl;
    myAct1->setRank(3);
    cout << "setRank Successful" << endl;

    myAct1->connectToDevice();

    cout << "connect Successful" << endl;

    if(testVelocity){
        int k;
        for(k=0;k<5000;k++)
        {
            myAct1->setVelocity(1000);
            usleep(1000*50);
        }
    }
    if(testPosition){
        myAct1->setPosition(1000);
    }
    if(getValues){
        cout << myAct1->getPosition() << endl;
    }
//922
    return 0;

}

