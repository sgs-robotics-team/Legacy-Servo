#include <stdio.h>
#include <stdlib.h>
#include <phidget22.h>
#include "Servo.h"

int main(int argc, char **argv){
		
	Servo s;
}

int main(int argc, char **argv) {
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
