/*
 * Serial.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: roboops
 */


#include "../../include/Serial.h"

using namespace std;

Serial::Serial(std::string port, unsigned int baud_rate)
: io(), serial(io,port) {

    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}


void Serial::writeString(string s)
{
    char * toWrite = (char*) malloc(sizeof(char)*(s.length()+1));
    memcpy(toWrite,s.c_str(),s.length());
    toWrite[s.length()] = '\r';
   // cout << toWrite << " blah" <<  endl;

    boost::asio::write(serial,boost::asio::buffer(toWrite,s.size()+1));
}

string Serial::readLine() {
    //Reading data char by char, code is optimized for simplicity, not speed
    using namespace boost;
    char c;
    std::string result;
    for(;;)
    {
        asio::read(serial,asio::buffer(&c,1));
        switch(c)
        {
            case '\r':
                cout << result << endl;
                return result;
            default:
                result+=c;
        }
    }
}
