
#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 4444
#define HOSTNAME "192.168.0.13"
// max number of bytes we can get at once
#define MAXDATASIZE 300



int main(int argc, char **argv)
{
	ros::init(argc, argv, "publisher");
	ros::NodeHandle n;
	ros::Publisher pub1 = n.advertise<std_msgs::String>("joyOutput", 1000);
	//ros::Rate loop_rate(10);	
		
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;

	while(ros::ok())
	{
		// connector’s address information
		struct sockaddr_in their_addr;

		// get the host info
		he=gethostbyname(HOSTNAME);
		if(he == NULL)
		{
			perror("gethostbyname()");
			exit(1);
		}
		else
			printf("Client-The remote host is: %s\n", HOSTNAME);

		if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket()");
			exit(1);
		}
		else
			printf("Client-The socket() sockfd is OK...\n");

		// host byte order
		their_addr.sin_family = AF_INET;
		// short, network byte order
		printf("Server-Using %s and port %d...\n", HOSTNAME, PORT);
		their_addr.sin_port = htons(PORT);
		their_addr.sin_addr = *((struct in_addr *)he->h_addr);
		// zero the rest of the struct
		memset(&(their_addr.sin_zero), '\0', 8);

		if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
		{
			perror("connect()");
		}
		else
			printf("Client-The connect() is OK...\n");


		while(ros::ok())
		{

		   std_msgs::String msg;
		   std::stringstream ss;

		   if (recv(sockfd, buf, MAXDATASIZE-1, MSG_PEEK | MSG_DONTWAIT) == 0)
		   {
						close(sockfd);
						break;
		   }
		   else if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
		   {

			   perror("recv()");
			   break;
		   }
		   else
		   {
			   buf[numbytes] = '\0';
			   printf("Client-Received: %s \n", buf);
			   msg.data = buf;
			ROS_INFO("%s", msg.data.c_str());

			pub1.publish(msg);

			ros::spinOnce();

			   //buf will contain messages of the form "XXX,YYY;"
		   }
		   usleep(1*1000);

		}
		usleep(1000*1000);
	}

   printf("Client-Closing sockfd\n");

   close(sockfd);
	return 0;

}
