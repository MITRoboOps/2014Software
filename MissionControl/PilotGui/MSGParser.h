#pragma once
#include <string.h>

namespace MSGParser
{
	extern bool update;
	typedef struct {
		char uptime[10];
		float vBatt;
		float currNetDraw;
		int currConsumed;
		float lat, lon;
		float pitch, roll, yaw;
	} Status;
	extern Status status;
	void parseStatus(char* input, int len);
};