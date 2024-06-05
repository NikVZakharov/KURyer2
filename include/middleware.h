#ifndef middleware_h
#define middleware_h


int getIRSensorValue(int sensor);
bool isOnCross();
bool isOnBlack(int sensor);
bool checkBanka();
int getIRError();
int getWallError();
int getError(int distance);
int getEncoderError();

#endif