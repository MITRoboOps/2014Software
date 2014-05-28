#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Actuator.h"

using namespace std;

int main()
{
    cout << "successful run" << endl;
    Actuator *myAct1 = new Actuator(1,200,1023,10000,100);
    //Actuator *myAct2 = new Actuator(2,200,1000,10000,100);
    cout << "successful initialization" << endl;
    myAct1->setRank(1);
   // myAct2->setRank(2);
    cout << "setRank Successful" << endl;

    myAct1->connectToDevice();
   // myAct2->connectToDevice();
    cout << "connect Successful" << endl;
    int k;
    for(k=0;k<40;k++)
    {
        myAct1->setVelocity(800);
        usleep(1000*50);
    }
    myAct1->setVelocity(0);
   // myAct2->setPosition(700);
    cout << "set position Successful" << endl;
    //myAct2->setVelocity(-50);
    //myAct1->setPosition(1000);

    //usleep(6*1000000);

    //myAct1->setVelocity(-1000);
    // myAct1->setVelocity(-1000);

   // usleep(0.5*1000000);

    return 0;

}

