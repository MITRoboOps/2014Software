#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firgelli.h"
#include <MotorController.cpp>
#include <exception>
using namespace std;


class wheelMotor: public MotorController {
	private:
		int rank = 1;
		Firgelli firgelli;
	public:
		void setRank(int newRank){
			rank = newRank;
		}

        bool connectToDevice(){
			try{
				firgelli.Open(rank);
				return true;
			}
            catch (exception& e){
                cout << e.what() << '\n';
                return false;
            }

        bool setPosition(double pos){
                firgelli.WriteCode()
        }

}

