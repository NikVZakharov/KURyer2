#include <Arduino.h>
#include <header.h>
void go(int L, int R) {
  if (L > 0) {
    digitalWrite(2, 1);
    analogWrite(3, L);
  }
  else {
    digitalWrite(2, 0);
    analogWrite(3, abs(L));
  }
  if (R > 0) {
    digitalWrite(4, 1);
    analogWrite(5, R);
  }
  else {
    digitalWrite(4, 0);
    analogWrite(5, abs(R));
  }
}

void right() {
  go(V, -V); delay(1000);
  while (analogRead(A1) > 500) go(V, -V);

  go(0,0);
}

void left() {
  go(-V, V); delay(1000);
  while (analogRead(A0) < 500) go(-V, V);

  go(0,0);
}
