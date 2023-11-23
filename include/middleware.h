#ifndef middleware_h
#define middleware_h

void preg(int speed);
int getIRSensorValue(int sensor);
bool isOnCross();
bool isOnBlack(int sensor);
bool checkBanka();
int currentError();
void fixPositionAfterTurn();
#endif