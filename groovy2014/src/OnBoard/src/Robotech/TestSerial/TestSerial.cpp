#include "../../../include/Serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

string IntToStr(int x){
        ostringstream out;
        out << x;
        return out.str();
}

int main(){
bool terminal = false;
bool testInts = false;
bool testGPS = true;

if(terminal){
    char buf[512];
    printf("\n Enter path to Serial : ");
    scanf("%[^\n]",buf);
    getchar();

    string path = buf;
    cout << "Path set to : " << path << "." << endl;

    printf("\n baud rate : ");
    scanf("%[^\n]",buf);
    getchar();

    int baudrate = atoi(buf);
    cout << "baudrate set to : " << baudrate << endl;

    Serial* connection = new Serial("/dev/ttyUSB0", 115200);

    while(1){

        printf("\n Enter Command : ");
        scanf("%[^\n]",buf);
        getchar();
        string msg = buf;
        cout << "message sent : " << msg << "." << endl;
        connection->writeString(msg +"\r");
    }
}
if(testGPS){
Serial* connection = new Serial("/dev/ttyUSB8", 38400);
    while(1){
        cout << connection->readLine() << endl;

    }

}

if(testInts){

    cout << IntToStr(-10) << endl;

}

}
