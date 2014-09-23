#include "OutputHandler.h"
#include <stdio.h>

#include "globals.h"

OutputHandler::OutputHandler()
{
}


OutputHandler::~OutputHandler()
{
}

void OutputHandler::sendChannels()
{
	p_out[0] = 'C';
	int left, right;
	switch (mode)
	{
	case 0: // Tank Drive
		len_p_out = 11;
		p_out[1] = '0';
		iToFChar(applyInputCurving(joyData->axis[1]), p_out + 2);
		p_out[6] = '1';
		iToFChar(applyInputCurving(joyData->axis[5]), p_out + 7);
		break;
	case 1: // Arcade + PT
		len_p_out = 21;
		left = joyData->axis[4];
		right = joyData->axis[5];
		arcadeAlgo(&left, &right);
		p_out[1] = '0';
		iToFChar(applyInputCurving(left), p_out + 2);
		p_out[6] = '1';
		iToFChar(applyInputCurving(right), p_out + 7);
		p_out[11] = '7'; //Pan
		iToFChar(applyInputCurving(joyData->axis[0]), p_out + 12);
		p_out[16] = '8'; //Tilt
		iToFChar(applyInputCurving(joyData->axis[1]), p_out + 17);
		break;
	case 2: // Arcade + Arm
		len_p_out = 41;
		//Arcase
		left = joyData->axis[4];
		right = joyData->axis[5];
		arcadeAlgo(&left, &right);
		p_out[1] = '0';
		iToFChar(applyInputCurving(left), p_out + 2);
		p_out[6] = '1';
		iToFChar(applyInputCurving(right), p_out + 7);
		//Arm
		p_out[11] = '2'; // Rotation
		iToFChar(applyInputCurving(joyData->axis[0]), p_out + 12);
		p_out[16] = '3'; // Boom
		iToFChar(applyInputCurving(joyData->axis[1]), p_out + 17);
		p_out[21] = '4'; //Stick
		iToFChar(applyInputCurving(joyData->axis[2]), p_out + 22);
		p_out[26] = '5'; // Claw
		iToFChar(applyInputCurving(joyData->axis[6]), p_out + 27);
		//PT HAT
		p_out[31] = '7'; //Pan
		iToFChar(applyInputCurving((int)(joyData->axis[8] * .4)), p_out + 32);
		p_out[36] = '8'; //Tilt
		iToFChar(applyInputCurving((int)(joyData->axis[9] * .4)), p_out + 37);
		break;
	case 3: // ARM ISO
		len_p_out = 46;
		//ARM
		p_out[1] = '2'; // Rotation
		iToFChar(applyInputCurving(joyData->axis[0]), p_out + 2);
		p_out[6] = '3'; // Boom
		iToFChar(applyInputCurving(joyData->axis[1]), p_out + 7);
		p_out[11] = '4'; //Stick
		iToFChar(applyInputCurving(joyData->axis[5]), p_out + 12);
		p_out[16] = '5'; // Claw
		iToFChar(applyInputCurving(joyData->axis[4]), p_out + 17);
		p_out[21] = '6'; //Thumb
		if (!thumbLock)
			iToFChar(applyInputCurving(joyData->axis[2]), p_out + 22);
		else
			iToFChar(applyInputCurving(0), p_out + 22);
		//PT HAT
		p_out[26] = '7'; //Pan
		iToFChar(applyInputCurving((int)(joyData->axis[8] * .4)), p_out + 27);
		p_out[31] = '8'; //Tilt
		iToFChar(applyInputCurving((int)(joyData->axis[9] * .4)), p_out + 32);
		//DT HAT
		p_out[36] = '0'; //
		iToFChar(applyInputCurving((int)(joyData->axis[10] * -.3)), p_out + 37);
		p_out[41] = '1'; //
		iToFChar(applyInputCurving((int)(joyData->axis[11] * -.3)), p_out + 42);
		break;
	}
	/*
	printf("ChData:  ");
	int i;
	for (i = 0; i != len_p_out; i++) {
		printf("%c", p_out[i]);
	}
	printf("; Size: %d\n", len_p_out);
	*/

	ctrlCli->uSend(p_out, len_p_out);
}

void OutputHandler::iToFChar(int i, char* out)
{
	char buf[5];
	if (i<0 || i>2000)
		i = 9999;
	sprintf_s(buf, "%04d", i);
	memcpy(out, buf, 4);
}
int OutputHandler::applyInputCurving(int i)
{
	//int in = i;
	float expo = (float)(-0.0045*joyData->axis[7] + 5.5); //Expo Level 1-10
	float sens = (float)(-0.0005*joyData->axis[3] + 0.525); //Sensitivity 5-100%
	sens = min(1, sens);

	i = (int)(i*sens);
	i = (int)(1000*((pow((float)i/1000, 3)*(expo-1) + (float)i/1000) / expo));

	i += 1000;

	//printf("EXPO: %f, Sens: %f, Input: %d. Output: %d\n", expo, sens, in, i);
	return i;
}

void  OutputHandler::arcadeAlgo(int* x, int* y)
{
	const int TURN_RATE = 95; //pick # 0-100
	int left, right;

	//printf("Arcade: XIN=%d, XOUT=%d, ", *x, *y);
	*x = (*x*TURN_RATE) / 100;
	left = (*y - *x);
	right = (*y + *x);
	left = min(left, 1000);
	left = max(left, -1000);
	right = min(right, 1000);
	right = max(right, -1000);
	*x = left;
	*y = right;
	//printf("LEFT=%d, RIGHT=%d\n", *x, *y);
}
