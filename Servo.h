#include <stdio.h>
#include <stdlib.h>
#include <phidget22.h>

#ifndef STEPPER_H
#define STEPPER_H

class Servo{

	PhidgetRCServoHandle ch;
	PhidgetReturnCode res;
	const char *errs;

private:

	int currentspeed;

public:

	Servo();
	int getSpeed(){ return currentspeed; }
	void setTargetSpeed(int);
	int calESC();
	~Servo();


};

#endif
