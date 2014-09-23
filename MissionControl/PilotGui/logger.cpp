#include "logger.h"
#include <iostream>
#include <chrono>
#include <thread>


namespace Log {
	using namespace std;

	void Log::Write(string msg, int priority) {
		//TODO: Fix possible race condition.
		cout << msg << endl;
		if (priority == ERR)
			this_thread::sleep_for(chrono::milliseconds(5000));
	}
}

