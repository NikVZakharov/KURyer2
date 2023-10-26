#include <Arduino.h>
#include <header.h>
int uzdF() {
  digitalWrite(UZF_TRIGGER_PIN, 0);
  delayMicroseconds(2);
  digitalWrite(UZF_TRIGGER_PIN, 1);
  delayMicroseconds(10);
  digitalWrite(UZF_TRIGGER_PIN, 0);
  return 0.01723 * pulseIn(UZF_ECHO_PIN, 1);
}
