#include "ros/ros.h"
#include "std_msgs/String.h"
#include <Messages/DriveTrainMessage.h>
#include <Messages/ArmMessage.h>
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
char incomingMessage[BUFLEN];
char param[4];
int sockfd;
struct sockaddr_in my_addr, cli_addr;

//Linked List for all incoming messages (rover side)
std::list<char*> messagesToClient;
std::mutex getClientMessage;

//Ros message types
Messages::DriveTrainMessage DTmsg;
Messages::ArmMessage Amsg;

void err(char *str)
{
    perror(str);
    exit(1);
}

void parseMessage(char* msg, int length){
	if(msg[0] != 'C') return;
	for(int i = 1; i < length; i+= 5){
		memcpy(&param, msg + i + 1, 4);
		if(msg[i] == '0') DTmsg.leftVelocity = atoi(param);
		if(msg[i] == '1') DTmsg.rightVelocity = atoi(param);
		if(msg[i] == '2') Amsg.armRot = atoi(param);
		if(msg[i] == '3') Amsg.boom = atoi(param);
		if(msg[i] == '4') Amsg.stick = atoi(param);
		if(msg[i] == '5') Amsg.claw = atoi(param);
		if(msg[i] == '6') Amsg.thumb = atoi(param);
		if(msg[i] == '7') {
			Amsg.pan = atoi(param);
			std::cout << "pan " << atoi(param) << std::endl;
}
		if(msg[i] == '8') Amsg.tilt = atoi(param);
			
		printf("%i from %s which goes to %i\n",DTmsg.leftVelocity,param,atoi(param));
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
}

void outMessages(){
	std::cout << "new thread" << std::endl;
	while(1){
		getClientMessage.lock();
		if(messagesToClient.empty()){
			getClientMessage.unlock();
			std::chrono::milliseconds dura( 20 );
			std::this_thread::sleep_for(dura); //20 millisecond sleep
		}
		else{
			char* msg = messagesToClient.front();
			messagesToClient.pop_front();
			sendto(sockfd, msg , sizeof(msg), 0, (struct sockaddr*)&cli_addr, sizeof(cli_addr));
			getClientMessage.unlock();
		}
	}
}


int main(int argc, char **argv){

	ros::init(argc, argv, "publisher");
	ros::NodeHandle n;



	//publishers
	ros::Publisher DTpub = n.advertise<Messages::DriveTrainMessage>("DriveTrain", 1);
	ros::Publisher Apub = n.advertise<Messages::ArmMessage>("Arm", 1);

	
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

	std::thread outMessagesThread(outMessages);
	
	while(ros::ok()){
		len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen);
		if (len==-1) err("recvfrom()");
		printf("Received packet from %s:%d\nData: %s Length : %i\n\n",
		       inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),buf, len);
		/*getClientMessage.lock();
		messagesToClient.push_back(buf);
		getClientMessage.unlock();*/ //sends message back to client
		parseMessage(buf, len);
		DTpub.publish(DTmsg);
		Apub.publish(Amsg);
		ros::spinOnce();
		resetMessages();
		
		

	}

	close(sockfd);
	return 0;
}




