#ifndef middleware_h
#define middleware_h


int getIRSensorValue(int sensor);
bool isOnCross();
bool isOnBlack(int sensor);
bool checkBanka();
int getIRError();
int getWallError();
int getError();
int getEncoderError();
float getMedianWithSort(const float *arr, int size);
float getMedianValue( float currentValue, float *pastValues);
void crossCalc();

#endif