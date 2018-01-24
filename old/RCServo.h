#include <stdio.h>
#include <stdlib.h>

#include <phidget22.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

class RCServo_Example{
public:
    int main(int argc, char **argv);
    ssleep(int);
    onAttachHandler(PhidgetHandle phid, void *ctx);
    onDetachHandler(PhidgetHandle phid, void *ctx);
    errorHandler(PhidgetHandle phid, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString);
    onPositionChangeHandler(PhidgetRCServoHandle ch, void *ctx, double position);
    onVelocityChangeHandler(PhidgetRCServoHandle ch, void *ctx, double velocity);
    onTargetPositionReachedHandler(PhidgetRCServoHandle ch, void *ctx, double position);
    PhidgetReturnCode initChannel(PhidgetHandle ch);
};
