#include "UDPCli.h"
//#include <winsock.h>
#include "include\SDL\SDL.h"
#include "include\SDL\SDL_thread.h"
#include "logger.h"
#include "globals.h"
#include "MSGParser.h"


UDPCli::UDPCli(char* ip, int p)
{

	hostIP = ip;
	port = p;
	Connect();
	
}

UDPCli::~UDPCli()
{
}
bool UDPCli::Connect()
{
	Log::Write("Connecting...", Log::DEBUG);
	if (connected)
	{
		Log::Write("Already Connected.", Log::WARN);
		return true;
	}

	unsigned int addr;
	struct hostent *hp;

	// Aquire socket
	if (sock != nullptr)
		delete sock;
	sock = new SOCKET;
	*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*sock == INVALID_SOCKET) {
		Log::Write("UDP Socket Bind Failed.", Log::ERR);
		WSACleanup();
		return false;
	}

	memset(&srv_addr, 0, sizeof(srv_addr));
	if (isalpha(hostIP[0]))
	{   // server address is a name
		addr = inet_addr(hostIP);
		hp = gethostbyaddr((char *)&addr, 4, AF_INET);
		if (hp == NULL)
		{
			Log::Write("Address Resolve Failed.", Log::ERR);
			WSACleanup();
			return false;
		}
		memcpy(&(srv_addr.sin_addr), hp->h_addr, hp->h_length);
		srv_addr.sin_family = hp->h_addrtype;
	}
	else
	{
		srv_addr.sin_family = AF_INET;
		srv_addr.sin_addr.s_addr = inet_addr(hostIP);
	}
	srv_addr.sin_port = htons(port);

	*sock = socket(AF_INET, SOCK_DGRAM, 0); /* Open a socket */
	if (*sock <0)
	{
		Log::Write("Socket() Failed.", Log::ERR);
		WSACleanup();
		return false;
	}
	if (connect(*sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == SOCKET_ERROR)
	{
		Log::Write("Connect() Failed.", Log::ERR);
		WSACleanup();
		return false;
	}

	//TEST STUFF
	connected = true;
	h_rThread = SDL_CreateThread(StaticRecv, "uRecv", this);
	printf("Connected to: %s:%d!\n", hostIP,port);
	return true;
}

void UDPCli::Disconnect()
{
	Log::Write("Disconnecting...", Log::DEBUG);
	closesocket(*sock);
	*sock = INVALID_SOCKET;
	connected = false;
	//SDL_WaitThread(h_rThread,NULL);
	Log::Write("Disconnected!", Log::DEBUG);

}

bool UDPCli::Reconnect()
{
	Log::Write("Reconnecting...", Log::DEBUG);
	Disconnect();
	return Connect();

}

//Receive UDP Message
int UDPCli::uRecv() {
	if (!connected) return(1);
	Log::Write("UDP Receive thread started.", Log::DEBUG);
	
	int size;
	while (1) {
		size = recv(*sock, buffer, sizeof(buffer), 0);
		
		if (!connected)
			return 0;

		//printf("%s:%d Got Packet! Size: %d\n", hostIP, port, size);
		
		if (size >= 4) {
			// H264 PACKET
			if (memcmp(buffer, pkt_h264, 4) == 0) {
				if (SDL_LockMutex(p_buffer_mutex) == 0)
				{
					memcpy(p_buffer_last->data, buffer, size);
					//printf("Got H264 Packet\n");
					//state = STATE_STREAMING;
					//retry = 0;
					p_buffer_last->next = new linked_buf_t;
					linked_buf_t *p_buf = p_buffer_last;
					p_buffer_last = p_buffer_last->next;
					p_buffer_last->size = 0;
					p_buf->size = size;
				}
				else
					printf("Mutex failed: %s\n", SDL_GetError());
				SDL_UnlockMutex(p_buffer_mutex);
			}
			// DATA PACKET
			else if (memcmp(buffer, pkt_data, 4) == 0) {
					
			}
		}
		if (size == SOCKET_ERROR)
		{
			Log::Write("Recv() Failed: Socket Error.", Log::ERR);
			return -1;
		}
		else if (size == 0)
		{
			Log::Write("Retval=0. Recv() Failed.", Log::ERR);
			return -1;
		}
		else if (buffer[0] == 'S')
		{
			printf("STATUS MESSAGE on %d: Received %d bytes, data \"%s\" from server.\n", port, size, buffer);
			MSGParser::parseStatus(buffer, size);
			ctrlTicks = 3;
		}	
		//printf("Client: Received %d bytes, data \"%s\" from server.\n", size, buffer);

	}
	return 0;
}

//Helper function for SDL's c-style threading.
int UDPCli::StaticRecv(void* ptr) {
	return ((UDPCli*)ptr)->uRecv();
}

//Send UDP Message
void UDPCli::uSend(char* data, int size) {
	if (!connected) return;
	int retval = send(*sock, data, size, 0);
	if (retval == SOCKET_ERROR)
	{
		Log::Write("Send() Failed.", Log::ERR);
		WSACleanup();
		connected = false;
		return;
	}
	/*
	printf("Sent:  ");
	int i;
	for (i = 0; i != size; i++) {
		printf("%c", data[i]);
	}
	printf("; Size: %d\n", size);
	*/
}

void UDPCli::uSend(char* data) {
	if (!connected) return;
	int retval = send(*sock, data, sizeof(data), 0);
	if (retval == SOCKET_ERROR)
	{
		Log::Write("Send() Failed.", Log::ERR);
		WSACleanup();
		connected = false;
		return;
	}
	printf("Sent: %s of Size: %d\n", data, sizeof(data));
}

// Return address from dotted-ip string
uint32_t UDPCli::net_dtoa(char* dotted_ip) {
	return(ntohl(inet_addr(dotted_ip)));
}
