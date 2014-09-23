#pragma once
#include <stdint.h>
#include <winsock2.h>
#include "include\SDL\SDL.h"
#include "include\SDL\SDL_thread.h"

class UDPCli
{
private:

	char* hostIP;
	int port;
	
	
	SOCKET *sock;        
	SOCKADDR_IN srv_addr;
	SOCKADDR_IN cli_addr;
	char cli_hname[256];					/* Host name of this computer */
	char buffer[8192];
	int  packetSize;
	SDL_Thread *h_rThread;

	uint32_t net_dtoa(char* dotted_ip);

	int uRecv();

public:
	bool connected = false;

	bool Connect();
	void Disconnect();
	bool Reconnect();
	void uSend(char* data, int size);
	void uSend(char* data);
	UDPCli(char* ip, int port);
	~UDPCli();

protected:
	static int StaticRecv(void* ptr);
};

