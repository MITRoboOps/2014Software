#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <Messages/NavigationMessage.h>
#include <sstream>
#include <thread>
#include <mutex>
#include <iostream>

#define SENDRATE 1 //second(s)
#define PORT 9940
#define BUFLEN 512



int sockfd, newsockfd;

//global connection variables

//Linked List for all incoming messages (rover side)
std::mutex accessStatus;
Messages::NavigationMessage currentStatus;


std::string IntToStr(int x){
        std::ostringstream out;
        out << x;
        return out.str();
}


//takes most recent status messages and concatinates them according to the protocol:
const char* concatMessages(){
	accessStatus.lock();
	std::string outMsg = "S0" + IntToStr(currentStatus.timeUp) + "S1" + IntToStr(currentStatus.BLcurrent) + "S2" + IntToStr(currentStatus.FLcurrent) + "S3" + IntToStr(currentStatus.BRcurrent) + "S4" + IntToStr(currentStatus.FRcurrent) + "S5" + IntToStr(currentStatus.BLvel) + "S6" + IntToStr(currentStatus.FLvel) + "S7" + IntToStr(currentStatus.BRvel) + "S8" + IntToStr(currentStatus.FRvel) + "S9" + IntToStr(currentStatus.chargeUsed) + "Sa" + IntToStr(currentStatus.current) + "Sb" + IntToStr(currentStatus.voltage) + "Sc" + IntToStr(currentStatus.latitude) + "Sd" + IntToStr(currentStatus.longitude) + "Se" + IntToStr(currentStatus.roll) + "Sf" + IntToStr(currentStatus.pitch) + "Sg" + IntToStr(currentStatus.yaw) + "Sh" + IntToStr(currentStatus.boomCurrent) + "Si" + IntToStr(currentStatus.armRotCurrent) + "Sj" + IntToStr(currentStatus.thumbPosition) + "Sk" + IntToStr(currentStatus.clawPosition) + "Sl" + IntToStr(currentStatus.stickPosition) + "Sm" + IntToStr(currentStatus.boomPosition) + "Sn" + IntToStr(currentStatus.armRotPosition) + "So" + IntToStr(currentStatus.panPosition) + "Sp" + IntToStr(currentStatus.tiltPosition) + "Sq" +IntToStr(currentStatus.satilites);
	const char* temp = outMsg.c_str();
	accessStatus.unlock();
	
	return temp;
	
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void sendMessages(){
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
	sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,0);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");
	while(ros::ok()){
		const char* newMsg = concatMessages();
		n = write(newsockfd,newMsg,sizeof(*newMsg));
		if (n < 0){
			close(newsockfd);
			close(sockfd);
			listen(sockfd,0);
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		}
		sleep(SENDRATE);
	}
}

void statusCallBack(const Messages::NavigationMessage::ConstPtr& msg){
	accessStatus.lock();
	currentStatus = *msg;
	accessStatus.unlock();
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "NavigationServer");
	ros::NodeHandle n;

	std::thread outMessagesThread(sendMessages);
	//subscribers	
	ros::Subscriber status = n.subscribe("Navigation",1,statusCallBack);

	while(ros::ok()){
		//have subscribers listen  

	}

	close(newsockfd);
	close(sockfd);
	
	return 0;
}
