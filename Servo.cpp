#include <stdio.h>
#include <stdlib.h>
#include <phidget22.h>
#include <iostream>
#include "Servo.h"

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

static void CCONV ssleep(int);

static void CCONV onAttachHandler(PhidgetHandle phid, void *ctx) {
	PhidgetReturnCode res;
	int hubPort;
	int channel;
	int serial;

	res = Phidget_getDeviceSerialNumber(phid, &serial);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to get device serial number\n");
		return;
	}

	res = Phidget_getChannel(phid, &channel);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to get channel number\n");
		return;
	}

	res = Phidget_getHubPort(phid, &hubPort);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to get hub port\n");
		hubPort = -1;
	}

	if (hubPort == -1)
		printf("channel %d on device %d attached\n", channel, serial);
	else
		printf("channel %d on device %d hub port %d attached\n", channel, serial, hubPort);
}

static void CCONV onDetachHandler(PhidgetHandle phid, void *ctx) {
	PhidgetReturnCode res;
	int hubPort;
	int channel;
	int serial;

	res = Phidget_getDeviceSerialNumber(phid, &serial);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to get device serial number\n");
		return;
	}

	res = Phidget_getChannel(phid, &channel);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to get channel number\n");
		return;
	}

	res = Phidget_getHubPort(phid, &hubPort);
	if (res != EPHIDGET_OK)
		hubPort = -1;

	if (hubPort != -1)
		printf("channel %d on device %d detached\n", channel, serial);
	else
		printf("channel %d on device %d hub port %d detached\n", channel, hubPort, serial);
}

static void CCONV errorHandler(PhidgetHandle phid, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString) {

	fprintf(stderr, "Error: %s (%d)\n", errorString, errorCode);
}

static void CCONV onPositionChangeHandler(PhidgetRCServoHandle ch, void *ctx, double position) {

	printf("Position Changed: %.3g\n", position);
}

static void CCONV onVelocityChangeHandler(PhidgetRCServoHandle ch, void *ctx, double velocity) {

	printf("Velocity Changed: %.3g\n", velocity);
}

static void CCONV onTargetPositionReachedHandler(PhidgetRCServoHandle ch, void *ctx, double position) {

	printf("Target Position Reached: %.3g\n", position);
}

/*
* Creates and initializes the channel.
*/
static PhidgetReturnCode CCONV initChannel(PhidgetHandle ch) {
	PhidgetReturnCode res;

	res = Phidget_setOnAttachHandler(ch, onAttachHandler, NULL);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to assign on attach handler\n");
		return (res);
	}

	res = Phidget_setOnDetachHandler(ch, onDetachHandler, NULL);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to assign on detach handler\n");
		return (res);
	}

	res = Phidget_setOnErrorHandler(ch, errorHandler, NULL);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to assign on error handler\n");
		return (res);
	}

	return (EPHIDGET_OK);

}

static void CCONV ssleep(int tm) {
#ifdef _WIN32
	Sleep(tm * 1000);
#else
	sleep(tm);
#endif
}

Servo::Servo(){



}

int Servo::calESC(){

	//To be implemented
	//Return 1 for Failure, 0 for Success

	return 0;

}

Servo::~Servo(){

	Phidget_close((PhidgetHandle)ch);
	PhidgetRCServo_delete(&ch);
	exit(res);

}
