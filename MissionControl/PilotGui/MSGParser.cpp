#include "MSGParser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace MSGParser {
	Status status;

	bool update;

	void parseStatus(char* input, int len)
	{
		
		for (int i = 0; i < len; i++)
		{
			//printf("i = %d\n", i);
			char msgtype;
			char msg[512];
			int val, hour, second, minute;
			if (input[i] == 'S')
			{
				int  k = i+1;
				for (k; k <= len; k++)
				{
					if (input[k] == 'S')
						break;
				}
				//printf("k=%d\n", k, i);
				msgtype = input[i + 1];
				memcpy(msg, input+i+2, k-i-2);
				msg[k-i-2] = '\0';
				val = strtol(msg, nullptr, 10);
				//printf("Parsing msg: %s of type %c\n", msg, msgtype);
				switch(msgtype)
				{
				case '0': //Uptime
					hour = val / 3600;
					second = val % 3600;
					minute = second / 60;
					second %= 60;
					sprintf_s(&*status.uptime, 10,"%.2d:%.2d:%.2d", hour, minute, second);
					
					//printf("uptime %d\n", val);
					break;
				case '1': //Batt Voltage
					status.vBatt = (float)(val/10.0);
					//printf("batvol %d\n", val);
					break;
				case '2': //Current Consumed
					status.currConsumed = val;
					//printf("CurrConsumed %d\n", val);
					break;
				case '3': //lon
					status.lon = (float)(val/10000000.0);
					break;
				case '4': //lat
					status.lat = (float)(val / 10000000.0);
					break;
				case '5': //roll
					status.roll = (float)((val - 1000)/100.0);
					break;
				case '6': //pitch
					status.pitch = (float)((val - 1000)/ 100.0);
					break;
				case '8': //Current Draw
					status.currNetDraw = (float)(val/100.0);
					break;
				}

				i = k-1;
			}
		}
		update = true;
	}
}