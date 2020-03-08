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


BTS7960bHBridge::BTS7960bHBridge(uint8_t r_en, uint8_t r_pwm, uint8_t l_en, uint8_t l_pwm) {
  
  //mEnPin = r_en;
  //mPwmPin = r_pwm;
  pinMode(mSpareEnPin, OUTPUT);
  pinMode(mSparePwmPin, OUTPUT);
  digitalWrite(mSpareEnPin, LOW);
  digitalWrite(mSparePwmPin, LOW);
}
/*

void BTS7960b::setIntensity(float intensity){
  mIntensity = min(max(intensity, -100.0), 100.0); //bound intensity in range -100 to 100 pct
  
  if (mIntensity >= 0.0) {
    mIsRightSide = true;
    
    //RightSide.setIntensity(mIntensity);
    //LeftSide.setIntensity(0.0);
    
  } else if (mIntensity < 0) {
    mIsRightSide = false;
    
    //RightSide.setIntensity(0.0);
    //LeftSide.setIntensity(mIntensity);
  }
}
*/
