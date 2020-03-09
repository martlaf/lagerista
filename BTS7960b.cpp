#include <Arduino.h>
#include "BTS7960b.h"

BTS7960bHalf::BTS7960bHalf(uint8_t enPin, uint8_t pwmPin): mEnPin(enPin), mPwmPin(pwmPin){
  pinMode(mEnPin, OUTPUT);
  pinMode(mPwmPin, OUTPUT);
  digitalWrite(mEnPin, LOW);
  digitalWrite(mPwmPin, LOW);
}

BTS7960bHalf::BTS7960bHalf(uint8_t enPin, uint8_t pwmPin, uint8_t isPin): mEnPin(enPin), mPwmPin(pwmPin), mIsPin(isPin){
  pinMode(mEnPin, OUTPUT);
  pinMode(mPwmPin, OUTPUT);
  pinMode(mIsPin, INPUT);
  digitalWrite(mEnPin, LOW);
  digitalWrite(mPwmPin, LOW);
}


void BTS7960bHalf::setIntensity(float intensity){
  mIntensity = min(max(intensity, 0.0), 100.0);    //bound intensity in range 0-100 pct

  analogWrite(mPwmPin, mIntensity*2.55);
}

void BTS7960bHalf::setActive(){
  mEnable = true;
  digitalWrite(mEnPin, HIGH);
}

void BTS7960bHalf::setInactive(){
  mEnable = false;
  digitalWrite(mEnPin, LOW);
}

float BTS7960bHalf::getIntensity(){
  return mIntensity;
}

bool BTS7960bHalf::isActive(){
  return mEnable;
}

BTS7960bHBridge::BTS7960bHBridge(uint8_t r_en, uint8_t r_pwm, uint8_t l_en, uint8_t l_pwm)
      : BTS7960bHalf(r_en, r_pwm), mSpareEnPin(l_en), mSparePwmPin(l_pwm){
  
  pinMode(mSpareEnPin, OUTPUT);
  pinMode(mSparePwmPin, OUTPUT);
  digitalWrite(mSpareEnPin, LOW);
  digitalWrite(mSparePwmPin, LOW);
}


void BTS7960bHBridge::setIntensity(float intensity) {
  intensity = min(max(intensity, -100.0), 100.0); //bound intensity in range -100 to 100 pct
  
  if (intensity >= 0.0 && !mIsRightSide) {
    switchSides();
  } else if (intensity < 0 && mIsRightSide) {
    switchSides();
  }

  this->BTS7960bHalf::setIntensity(abs(intensity));
}

void BTS7960bHBridge::switchSides() {
  
  mIsRightSide != mIsRightSide;

  setInactive();
  
  uint8_t oldEnPin = mEnPin;
  uint8_t oldPwmPin = mPwmPin;
  uint8_t oldIsPin = mIsPin;

  mEnPin = mSpareEnPin;
  mPwmPin = mSparePwmPin;
  mIsPin = mSpareIsPin;

  mSpareEnPin = oldEnPin;
  mSparePwmPin = oldPwmPin;
  mSpareIsPin = oldIsPin;

  setActive();
  
}

float BTS7960bHBridge::getIntensity(){
  if (mIsRightSide) return mIntensity;

  return -mIntensity;
}

