#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "../../include/Actuator.h"

using namespace std;

int main()
{
    cout << "successful run" << endl;
    Actuator *myAct1 = new Actuator(1,0,1023,1000,100);
    //Actuator *myAct2 = new Actuator(2,200,1000,10000,100);
    cout << "successful initialization" << endl;
    myAct1->setRank(3);
   // myAct2->setRank(2);
    cout << "setRank Successful" << endl;

    myAct1->connectToDevice();
   // myAct2->connectToDevice();
    cout << "connect Successful" << endl;
   // myAct1->setPosition(700);
   // sleep(10);
   // myAct1->setPosition(0);
    int k;
    for(k=0;k<5000;k++)
    {
        myAct1->setVelocity(1000);
        usleep(1000*50);
    }
  //  myAct1->setVelocity(0);
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

