#ifndef drive_h
#define drive_h

void go(int L, int R);
void left();
void right();
void start();
void finish();
void doezd();
void driveSomeTime(unsigned long timeToMove);
void fixPositionAfterTurn();
void driveToObjectOnBlack();
void driveBackToCross();
void stop();
void pid(int speed);
void turnEnc(int speed, int angle);
void driveSomeDistance(int distance);
int arrMove(int *starshiy);
void avoidObstacle();
//void cubeReverse(int n, const int arr[], int out[]);




#endif