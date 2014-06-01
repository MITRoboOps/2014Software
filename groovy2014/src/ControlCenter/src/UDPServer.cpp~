#include "ros/ros.h"
#include "std_msgs/String.h"
#include <Messages/DriveTrainMessage.h>
#include <Messages/ArmMessage.h>
#include <Messages/PilotMessage.h>
#include <Messages/CameraBoomMessage.h>
#include <sstream>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <iostream>
#define BUFLEN 512
#define PORT 9930



//global connection variables
char param[4];
int sockfd;
struct sockaddr_in my_addr, cli_addr;

//Linked List for all incoming messages (rover side)
std::list<char*> messagesToClient;
std::list<int> messagesToClientSize;
std::mutex getClientMessage;

//Trusted Msgs
bool trustedIDs[1000];
std::mutex getIDs;

//Ros message types
Messages::DriveTrainMessage DTmsg;
Messages::ArmMessage Amsg;
Messages::CameraBoomMessage CBmsg;

void err(char *str)
{
    perror(str);
    exit(1);
}

void parseMessage(char* msg, int length){
	if(msg[0] == 'C') {
		for(int i = 1; i < length; i+= 5){
			memcpy(&param, msg + i + 1, 4);
			if(msg[i] == '0') DTmsg.leftVelocity = atoi(param);
			if(msg[i] == '1') DTmsg.rightVelocity = atoi(param);
			if(msg[i] == '2') Amsg.armRot = atoi(param);
			if(msg[i] == '3') Amsg.boom = atoi(param);
			if(msg[i] == '4') Amsg.stick = atoi(param);
			if(msg[i] == '5') Amsg.claw = atoi(param);
			if(msg[i] == '6') Amsg.thumb = atoi(param);
			if(msg[i] == '7') Amsg.pan = atoi(param);
			if(msg[i] == '8') Amsg.tilt = atoi(param);
			Amsg.command = -1;
		}
	}
	else if(msg[0] == 'A'){
		Amsg.command = atoi(msg[1]);
	}
	else if(msg[0] == 'B'){
		CBmsg.command = atoi(msg[1]);
	}

}

void resetMessages(){
	DTmsg.leftVelocity = 1000;
	DTmsg.rightVelocity = 1000;
	Amsg.armRot = 1000;
	Amsg.boom = 1000;
	Amsg.stick = 1000;
	Amsg.claw = 1000;
	Amsg.thumb = 1000;
	Amsg.pan = 1000;
	Amsg.tilt = 1000;
	Amsg.command = -1;
	CBmsg.command = -1;
}

void zeroBoolArray(bool* array, int arrayLength, int start, int placesToZero){
	getIDs.lock();
	for(int i = start; i < start + placesToZero; i++){
		array[i%arrayLength] = 0;
	}
	getIDs.unlock();
}

void checkTrusted(char* msg, int length){
	if(msg[0] != 'T') return;
	char* newMsg = (char*) malloc(sizeof(char)*3);
        char* id = (char*) malloc(sizeof(char)*3);
	char sendBack[5] = {'T','R',' ',' ',' '};
	memcpy(id, msg + 1, 3);
	getIDs.lock();
	if(trustedIDs[atoi(id)] == 0){
		trustedIDs[atoi(id)] = 1;
		memcpy(newMsg,msg+4,3);
        	msg = newMsg;
		length -=4;
		getIDs.unlock();
		zeroBoolArray(trustedIDs, 1000, atoi(id) + 500, 1);
	}
	else length = 0;
	memcpy(sendBack + 2, id, 3);

	getClientMessage.lock();
	messagesToClient.push_back(sendBack);
	messagesToClientSize.push_back(5);
	getClientMessage.unlock();
}



void outMessages(ros::NodeHandle n){

	std::cout << "new thread" << std::endl;
	//publishers
	ros::Publisher DTpub = n.advertise<Messages::DriveTrainMessage>("DriveTrain", 1);
	ros::Publisher Apub = n.advertise<Messages::ArmMessage>("Arm", 1);
	ros::Publisher CBpub = n.advertise<Messages::CameraBoomMessage>("CBoom",1);

	
	int i;
	socklen_t slen=sizeof(cli_addr);
	char buf[BUFLEN];
	int len =0;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		err("socket");
	else
		printf("Server : Socket() successful\n");

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1)
		err("bind");
	else
		printf("Server : bind() successful\n");
	
	while(ros::ok()){
		len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen);
		if (len==-1) err("recvfrom()");
		printf("Received packet from %s:%d\nData: %s Length : %i\n\n",
		       inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),buf, len);
		checkTrusted(buf,len);
		parseMessage(buf, len);
		DTpub.publish(DTmsg);
		Apub.publish(Amsg);
		CBpub.publish(CBmsg);
		ros::spinOnce();
		resetMessages();
	}
}

void addMessageToClient(char* msg, int size){
	getClientMessage.lock();
	messagesToClient.push_back(msg);
	messagesToClientSize.push_back(size);
	getClientMessage.unlock();
}

std::string IntToStr(int x){
        std::ostringstream out;
        out << x;
        return out.str();
}

void statusCallBack(const Messages::PilotMessage::ConstPtr& msg){
	std::string msgToSend = "S0" + IntToStr(msg->timeUp) + "S1" + IntToStr(msg->batteryVoltage) + "S2" + IntToStr(msg->chargeUsed) + "S3" + IntToStr(msg->latitude) + "S4" + IntToStr(msg->longitude) + "S5" + IntToStr(msg->roll + 1000) + "S6" + IntToStr(msg->pitch + 1000) + "S7" + IntToStr(msg->yaw + 1000);
	addMessageToClient(msgToSend.c_str(),msgToSend.length());

}

int main(int argc, char **argv){

	zeroBoolArray(trustedIDs, 1000, 0, 1000);

	ros::init(argc, argv, "PilotServer");
	ros::NodeHandle n;

	ros::Subscriber status = n.subscribe("Pilot",1,statusCallBack);

	std::thread outMessagesThread(outMessages,n);
	while(ros::ok()){
		getClientMessage.lock();
		if(messagesToClient.empty()){
			getClientMessage.unlock();
			std::chrono::milliseconds dura( 100 );
			std::this_thread::sleep_for(dura); //100 millisecond sleep
		}
		else{
			char* msg = messagesToClient.front();
			messagesToClient.pop_front();
			int size = messagesToClientSize.front();
			messagesToClientSize.pop_front();
			sendto(sockfd, msg , size, 0, (struct sockaddr*)&cli_addr, sizeof(cli_addr));
			getClientMessage.unlock();
		}
	}

	close(sockfd);
	return 0;
}




