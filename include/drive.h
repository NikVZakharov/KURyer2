#ifndef drive_h
#define drive_h

void go(int L, int R, int interval = 0,bool fixMotor=true);
void preg(int speed);
void left();
void right();
void start();
void finish();
void doezd();
void pregSomeTime(unsigned long timeToMove);
void fixPositionAfterTurn();

#endif