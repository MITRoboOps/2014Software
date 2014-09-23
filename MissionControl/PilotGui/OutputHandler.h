#pragma once
class OutputHandler
{
public:
	OutputHandler();
	~OutputHandler();
	int mode = 0;
	char* p_out = new char[512];
	int len_p_out;

	void sendChannels();


private:


	void iToFChar(int i, char* out);
	int applyInputCurving(int i);
	void arcadeAlgo(int* x, int* y);
};

