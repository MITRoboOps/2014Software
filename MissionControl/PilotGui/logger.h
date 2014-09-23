#pragma once
#include <string>
using namespace std;

namespace Log
{
	enum Priority
	{
		VERBOSE_DEBUG = 0, DEBUG = 1, INFO = 2, NOTICE = 3,
		P_NOTICE = 4, SUCCESS_AUDIT = 5, FAILURE_AUDIT = 6,
		WARN = 8, P_WARN = 9, ERR = 10
	};

	void Write(string msg, int priority);
};

