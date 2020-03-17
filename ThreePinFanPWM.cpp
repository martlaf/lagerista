#include <Arduino.h>
#include "ThreePinFanPWM.h"

ThreePinFanPWM::ThreePinFanPWM(uint8_t pinOut): mPinOut(pinOut) {
  pinMode(mPinOut, OUTPUT);
  ThreePinFanPWM::setSpeed(0);
}

ThreePinFanPWM::ThreePinFanPWM(uint8_t pinOut, uint8_t pinIn): mPinOut(pinOut), mPinIn(pinIn) {
  pinMode(mPinIn, INPUT);
  pinMode(mPinOut, OUTPUT);
  ThreePinFanPWM::setSpeed(0);
}

void ThreePinFanPWM::setSpeed(float percentage) {
  analogWrite(mPinOut, (percentage * 2.55));
}


