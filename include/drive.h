#ifndef drive_h
#define drive_h

void go(int L, int R, int interval = 0);
void preg(int speed);
void left();
void right();
void start();
void finish();
void doezd();
void pregSomeTime(unsigned long timeToMove);
void fixPositionAfterTurn();
void driveToObjectOnBlack();
void driveBackToCross();
void turnGCross();
void MoveBankaCross();
void encpid(int V, int K ,int N=0);
void stop();
void pid(int speed, int distance=0);
void pidEnc(int baseSpeed,float k);
void turnEnc(int speed, int angle);
void pidEncSomeTime(unsigned long timeToMove);

void drive2();
void drive3();


#endif