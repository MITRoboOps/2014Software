#include "../../include/RobotechMotor.h"





using namespace std;

//Mode 0 : closed loop speed
//Mode 1 : count position mode (for setting absolute position)
//Mode 2 : relative position mode

RobotechMotor::RobotechMotor(int maxVelocity, int millisecs, int accel, int decel, int maxCurrent, bool isOpenLoopVelocity, std::string pathToUsb){
    mode = -1;
    maxRPM = maxVelocity;
    stallOut = millisecs;
    accelRate = accel;
    decelRate = decel;
    if(maxCurrent < 0) maxAmps = 100;
    else maxAmps = maxCurrent;
    isOpenLoopVel = isOpenLoopVelocity;
    path = pathToUsb;
}

std::string RobotechMotor::IntToStr(int x){
        std::ostringstream out;
        out << x;
        return out.str();
}


bool RobotechMotor::connectToDevice(){
    int numTries = 0;
    int numberToTry = 5;
    while(numTries < numberToTry)
    try{
        connection = new Serial(path,115200);
        numTries = numberToTry;
        } catch (exception& e) {
			std::cout << "Connection failed. Retrying " << numberToTry - numTries << " more times." << endl;
			if(numTries == numberToTry) return false;
			numTries++;
			usleep (3000*1000);
		}

    connection->writeString("");
   //connection->writeString("^MMOD 0");
  //  connection->writeString("%EERST 321654987");
    setMaxVelocity(maxRPM);
    setMaxCurrent(maxAmps);
    setAccel(accelRate);
    setDecel(decelRate);
    setWatchDogTime(stallOut);
    connection->writeString("^ECHOF 0"); //echo off

    thread readValuesThread(&RobotechMotor::readValues,this);
    readValuesThread.detach();


    return true;


}

void RobotechMotor::readValues(){

    while(1){
        string msg = connection->readLine();
        char *msgArray = new char[msg.length() + 1];
        strcpy(msgArray,msg.c_str());
        if(msgArray[0] != '+'){
            getStatus.lock();
            statusMessages.push_back(msgArray);
            getStatus.unlock();
        }
    }

}

char* RobotechMotor::getStatusMessage(char* identifier){
    getStatus.lock();
    while(!statusMessages.empty()){
        char* msg = statusMessages.back();
        cout << msg << endl;
        statusMessages.pop_back();
        bool msgMatches = true;
        for(int i =0; i < sizeof(*identifier); i++){
            cout << msg[i] << " " <<  identifier[i] << endl;
            if(msg[i] != identifier[i]) msgMatches = false;
        }
        if(msgMatches){
            statusMessages.clear();
            getStatus.unlock();
            return msg;
        }
    }
    getStatus.unlock();
    return "";

}

bool RobotechMotor::recievedMessage(char* msgToCheck){
    getStatus.lock();
    for (std::list<char*>::iterator it=statusMessages.begin(); it != statusMessages.end(); ++it){
        try{
           // cout << "itr : " << *it << " msg : " << msgToCheck << endl;
            bool compare = 0;
          // cout << "compare : " << memcmp(msgToCheck, *it, sizeof(*msgToCheck)) << endl;
            if(memcmp(msgToCheck, *it, sizeof(msgToCheck)) == 0){
                compare = 1;
            }
            if(compare){
                getStatus.unlock();
                return true;
            }
        } catch(exception e){}
    }
    getStatus.unlock();
    return false;

}


//returns count
int RobotechMotor::getPosition(){
    connection->writeString("?C 1");
    usleep(50*1000);
    char* msg = getStatusMessage("C");
   // cout << msg << endl;
    if(msg[2] == '-'){
        char* value = new char[sizeof(msg)-3];
        memcpy(value, msg + 3, sizeof(value));
        cout << "Position " << -atoi(value) << endl;
        return -atoi(value);
    }
    else{
        char* value = new char[sizeof(msg)-2];
        memcpy(value, msg + 2, sizeof(value));
      //  cout << "Position " << atoi(value) << endl;
        return atoi(value);
    }

}

int RobotechMotor::getFeedBack(){
    connection->writeString("?F");
    usleep(50*1000);
    //if(!recievedMessage("?F"));

    char* msg = getStatusMessage("F");
    if(sizeof(msg) < 3) return getFeedBack();
  //  cout << msg << " " << msg[2] <<  endl;
    if(msg[2] == '-'){
        char value[strlen(msg) -3];
        memcpy(value, msg + 3, sizeof(value));
      //  cout << "minus reckonized" << endl;
      //  cout << value << endl;
      //  cout << "FeedBack " << -atoi(value) << endl;
        return -atoi(value);
    }
    else{
        char* value = new char[sizeof(msg)-2];
        memcpy(value, msg + 2, sizeof(value));
      //  cout << "Feedback " << atoi(value) << endl;
        return atoi(value);
    };
}



//returns RPMS
int RobotechMotor::getVelocity(){
    connection->writeString("?BS 1");
    char* msg = getStatusMessage("BS");
    char* value = new char[sizeof(msg)-3];
    memcpy(value, msg + 3, sizeof(value));

    return atoi(value);
}

//NOTE: This value is 10* the current
int RobotechMotor::getCurrent(){
   // connection->writeString("# C");
    connection->writeString("?A 1");
    char* msg = getStatusMessage("A");
    char* value = new char[sizeof(msg)-2];
    memcpy(value, msg + 2, sizeof(value));

    return atoi(value);
}


int* RobotechMotor::getTemp(){
    connection->writeString("?T");
    char* msg = getStatusMessage("T");
    char* value = new char[sizeof(msg)-2];
    memcpy(value, msg + 2, sizeof(value));

    return NULL;
}

int RobotechMotor::getBatteryVoltage(){
    connection->writeString("?V 1");
    char* msg = getStatusMessage("V");
    int start = 0;
    for(int i = 0; i < sizeof(msg); i++){
        if(msg[i] == ':'){
            start = i +1;
            break;

        }
    }
    char* volt = new char[sizeof(msg) - start];
    memcpy(volt, msg + start, sizeof(volt));
    return atoi(volt);
}


void RobotechMotor::setAbsolutePosition(double pos){
    if( mode != 1){
        cout << "MMOD 3" << endl;
        connection->writeString("^MMOD 0");
        usleep(10*1000);
        connection->writeString("^MMOD 3");//position count mode
        usleep(50*1000);
        if(!recievedMessage("^MMOD 3")) setAbsolutePosition(pos);
        mode = 1;
    }

    int actualPos = (int) (pos + 0.5);
    string toWrite = "!p " + IntToStr(actualPos);
    connection->writeString(toWrite);
    usleep(50*1000);
    //if(!recievedMessage((char*)toWrite.c_str())) setAbsolutePosition(pos);

}

void RobotechMotor::setRelativePosition(double pos){
    if(mode != 2){
        cout << "MMOD 2" << endl;
        connection->writeString("^MMOD 1 2");//position count mode
        mode = 2;
    }

    int actualPos = (int) (pos + 0.5);
    string toString = IntToStr(actualPos);
    connection->writeString("!g 1 "+toString);

}

//takes in all ints
void RobotechMotor::setVelocity(double vel){
    if(mode != 0){
        mode = 0;
        if(isOpenLoopVel) {
            connection->writeString("^MMOD 0");//open loop
            cout << " r  MMOD 0" <<  endl;
                    }
        else {
            connection->writeString("^MMOD 1");
            cout << "MMOD 1" << endl;
        }
    }
    int actualVel = (int) (vel + 0.5);
    //cout << actualVel << endl;
    string toString = IntToStr(actualVel);
    connection->writeString("!g 1 "+toString );
}

void RobotechMotor::setMaxVelocity(int vel){
    connection->writeString("^MXRPM 1 " + IntToStr(vel) ); //set max speed
    maxRPM = vel;
}
int RobotechMotor::getMaxVelocity(){return maxRPM;}

void RobotechMotor::setMaxCurrent(int cur){
    connection->writeString("^ALIM 1 " + IntToStr(cur * 10) );
    //connection->writeString("^ALIM 10");
    maxAmps = cur;
}
int RobotechMotor::getMaxCurrent(){return maxAmps;}

void RobotechMotor::setAccel(int accel){
    connection->writeString("^MAC 1 "+ IntToStr(accel) ); //set acceleration
    accelRate = accel;
}
int RobotechMotor::getAccel(){return accelRate;}

void RobotechMotor::setDecel(int decel){
    connection->writeString("^MDEC 1 " + IntToStr(decel) ); //set deceleration
    decelRate = decel;
}
int RobotechMotor::getDecel(){return decelRate;}

void RobotechMotor::setWatchDogTime(int millisecs) {
    connection->writeString("^RWD " + IntToStr(millisecs) );
    stallOut = millisecs;
    }
int RobotechMotor::getWatchDogTime() {return stallOut;}

void RobotechMotor::resetCounter(){
    connection->writeString("^MMOD 0");
    connection->writeString("!c 0");

}

void RobotechMotor::setPGain(int gain){
    connection->writeString("^KP " + IntToStr(gain));
}
void RobotechMotor::setIGain(int gain){
    connection->writeString("^KI " + IntToStr(gain));
}
void RobotechMotor::setDGain(int gain){
    connection->writeString("^KD " + IntToStr(gain));
}

void RobotechMotor::changeToOpenLoopVel(){
    isOpenLoopVel =true;
}

void RobotechMotor::setMaxPositionModeVelocity(int vel){
    connection->writeString("^MVEL 1 " + IntToStr(vel)); //position mode velocity default
}
