#include <stdio.h>
#include <stdlib.h>

#include <phidget22.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

static void CCONV ssleep(int);

static void CCONV
onAttachHandler(PhidgetHandle phid, void *ctx) {
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

static void CCONV
onDetachHandler(PhidgetHandle phid, void *ctx) {
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

static void CCONV
errorHandler(PhidgetHandle phid, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString) {

	fprintf(stderr, "Error: %s (%d)\n", errorString, errorCode);
}

static void CCONV
onPositionChangeHandler(PhidgetRCServoHandle ch, void *ctx, double position) {

	printf("Position Changed: %.3g\n", position);
}

static void CCONV
onVelocityChangeHandler(PhidgetRCServoHandle ch, void *ctx, double velocity) {

	printf("Velocity Changed: %.3g\n", velocity);
}

static void CCONV
onTargetPositionReachedHandler(PhidgetRCServoHandle ch, void *ctx, double position) {

	printf("Target Position Reached: %.3g\n", position);
}

/*
* Creates and initializes the channel.
*/
static PhidgetReturnCode CCONV
initChannel(PhidgetHandle ch) {
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

	/*
	* Please review the Phidget22 channel matching documentation for details on the device
	* and class architecture of Phidget22, and how channels are matched to device features.
	*/

	/*
	* Specifies the serial number of the device to attach to.
	* For VINT devices, this is the hub serial number.
	*
	* The default is any device.
	*/
	// Phidget_setDeviceSerialNumber(ch, <YOUR DEVICE SERIAL NUMBER>);

	/*
	* For VINT devices, this specifies the port the VINT device must be plugged into.
	*
	* The default is any port.
	*/
	// Phidget_setHubPort(ch, 0);

	/*
	* Specifies that the channel should only match a VINT hub port.
	* The only valid channel id is 0.
	*
	* The default is 0 (false), meaning VINT hub ports will never match
	*/
	// Phidget_setIsHubPortDevice(ch, 1);

	/*
	* Specifies which channel to attach to.  It is important that the channel of
	* the device is the same class as the channel that is being opened.
	*
	* The default is any channel.
	*/
	// Phidget_setChannel(ch, 0);

	/*
	* In order to attach to a network Phidget, the program must connect to a Phidget22 Network Server.
	* In a normal environment this can be done automatically by enabling server discovery, which
	* will cause the client to discovery and connect to available servers.
	*
	* To force the channel to only match a network Phidget, set remote to 1.
	*/
	// PhidgetNet_enableServerDiscovery(PHIDGETSERVER_DEVICE);
	// Phidget_setIsRemote(ch, 1);

	return (EPHIDGET_OK);
}

int
main(int argc, char **argv) {
	PhidgetRCServoHandle ch;
	PhidgetReturnCode res;
	const char *errs;

	/*
	* Enable logging to stdout
	*/
	PhidgetLog_enable(PHIDGET_LOG_INFO, NULL);

	res = PhidgetRCServo_create(&ch);
	if (res != EPHIDGET_OK) {
		fprintf(stderr, "failed to create rc servo channel\n");
		exit(1);
	}

	res = initChannel((PhidgetHandle)ch);
	if (res != EPHIDGET_OK) {
		Phidget_getErrorDescription(res, &errs);
		fprintf(stderr, "failed to initialize channel:%s\n", errs);
		exit(1);
	}

	res = PhidgetRCServo_setOnPositionChangeHandler(ch, onPositionChangeHandler, NULL);
	if (res != EPHIDGET_OK) {
		Phidget_getErrorDescription(res, &errs);
		fprintf(stderr, "failed to set position change handler: %s\n", errs);
		goto done;
	}

	res = PhidgetRCServo_setOnVelocityChangeHandler(ch, onVelocityChangeHandler, NULL);
	if (res != EPHIDGET_OK) {
		Phidget_getErrorDescription(res, &errs);
		fprintf(stderr, "failed to set velocity change handler: %s\n", errs);
		goto done;
	}

	res = PhidgetRCServo_setOnTargetPositionReachedHandler(ch, onTargetPositionReachedHandler, NULL);
	if (res != EPHIDGET_OK) {
		Phidget_getErrorDescription(res, &errs);
		fprintf(stderr, "failed to set target position reached handler: %s\n", errs);
		goto done;
	}

	/*
	* Open the channel synchronously: waiting a maximum of 5 seconds.
	*/
	res = Phidget_openWaitForAttachment((PhidgetHandle)ch, 5000);
	if (res != EPHIDGET_OK) {
		if (res == EPHIDGET_TIMEOUT) {
			printf("Channel did not attach after 5 seconds: please check that the device is attached\n");
		} else {
			Phidget_getErrorDescription(res, &errs);
			fprintf(stderr, "failed to open channel:%s\n", errs);
		}
		goto done;
	}

	ssleep(3);

	printf("Setting target position to 90\n");
	res = PhidgetRCServo_setTargetPosition(ch, 90);
	if (res != EPHIDGET_OK) {
		printf("failed to set target position\n");
	}

	printf("Setting engaged\n");
	res = PhidgetRCServo_setEngaged(ch, 1);
	if (res != EPHIDGET_OK) {
		printf("failed to set engaged\n");
	}

	printf("Setting Target Position to 180 for 5 seconds...\n");
	PhidgetRCServo_setTargetPosition(ch, 180);
	ssleep(5);

	printf("Setting Target Position to 0 for 5 seconds...\n");
	PhidgetRCServo_setTargetPosition(ch, 0);
	ssleep(5);

	printf("Setting Target Position to 90 for 5 seconds...\n");
	PhidgetRCServo_setTargetPosition(ch, 90);
	ssleep(5);

done:

	Phidget_close((PhidgetHandle)ch);
	PhidgetRCServo_delete(&ch);

	exit(res);
}

static void CCONV
ssleep(int tm) {
#ifdef _WIN32
	Sleep(tm * 1000);
#else
	sleep(tm);
#endif
}
