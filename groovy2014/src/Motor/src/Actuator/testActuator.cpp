#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Actuator.h"

using namespace std;

int main()
{
    cout << "successful run" << endl;
    Actuator *myAct1 = new Actuator(1);
    cout << "successful initialization" << endl;
    myAct1->setRank(1);
    // myAct2->setRank(2);
    cout << "setRank Successful" << endl;

    myAct1->connectToDevice();
    cout << "connect Successful" << endl;
    myAct1->setPosition(1000);
    //myAct1->setPosition(1000);

    usleep(6*1000000);

    myAct1->setVelocity(-1000);
    // myAct1->setVelocity(-1000);

    usleep(0.5*1000000);

    return 0;

}

