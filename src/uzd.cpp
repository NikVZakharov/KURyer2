#include <Arduino.h>
#include <header.h>


float uzd(int trigerPin, int echoPin,float& pastValue) {
  digitalWrite(trigerPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigerPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigerPin, 0);
  
  float s= 0.01723 * pulseIn(echoPin, 1);
  float u=s*0.3+pastValue*0.7;
  pastValue=s;
  return u;
}

float getDistance(int trigerPin, int echoPin,float& pastValue) {
  return uzd( trigerPin,  echoPin, pastValue);

}



