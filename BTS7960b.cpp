/**********************************************************************************************
 * Arduino BTS7960b Library
 * Author: Martin Lafrance
 *         lafrance.martin@gmail.com
 *         
 * This library supports both the chip itself and the full H-Bridge board
 * The Half-bridge object could be used for any controller that has an Enable pin with a PWM pin
 * The Full H-bridge object could be used the same with two-sided controllers
 **********************************************************************************************/
 
#include <Arduino.h>
#include "BTS7960b.h"

// Single-sided, no sensing
BTS7960bHalf::BTS7960bHalf(uint8_t enPin, uint8_t pwmPin)
      : mEnPin(enPin), mPwmPin(pwmPin){
  pinMode(mEnPin, OUTPUT);
  pinMode(mPwmPin, OUTPUT);
  digitalWrite(mEnPin, LOW);
  digitalWrite(mPwmPin, LOW);
}

// Single-sided, with sensing
BTS7960bHalf::BTS7960bHalf(uint8_t enPin, uint8_t pwmPin, uint8_t isPin)
      : mEnPin(enPin), mPwmPin(pwmPin), mIsPin(isPin){
  pinMode(mEnPin, OUTPUT);
  pinMode(mPwmPin, OUTPUT);
  pinMode(mIsPin, INPUT);
  digitalWrite(mEnPin, LOW);
  digitalWrite(mPwmPin, LOW);
}

// set PWM intensity in percentage
void BTS7960bHalf::setIntensity(float intensity){
  mIntensity = min(max(intensity, 0.0), 100.0);    //bound intensity in range 0-100 pct

  analogWrite(mPwmPin, mIntensity*2.55);
}

// set Enable pin active
void BTS7960bHalf::setActive(){
  mEnable = true;
  digitalWrite(mEnPin, HIGH);
}

// set Enable pin inactive
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

// get voltage at IS pin
float BTS7960bHalf::getCurrent(){
  float currentSense = analogRead(mIsPin);
  return currentSense;
}

// Full H-bridge, no sensing
BTS7960bHBridge::BTS7960bHBridge(uint8_t r_en, uint8_t r_pwm, uint8_t l_en, uint8_t l_pwm)
      : BTS7960bHalf(r_en, r_pwm), mSpareEnPin(l_en), mSparePwmPin(l_pwm){
  
  pinMode(mSpareEnPin, OUTPUT);
  pinMode(mSparePwmPin, OUTPUT);
  digitalWrite(mSpareEnPin, HIGH);
  digitalWrite(mSparePwmPin, LOW);
}


void BTS7960bHBridge::setIntensity(float intensity) {
  intensity = min(max(intensity, -100.0), 100.0); //bound intensity in range -100 to 100 pct
  
  if (intensity >= 0.0 && !mIsRightSide) {
    switchSides();
  } else if (intensity < 0 && mIsRightSide) {
    switchSides();
  }
  
  //set high side to desired voltage
  BTS7960bHalf::setIntensity(abs(intensity));
}

void BTS7960bHBridge::switchSides() {
  
  mIsRightSide = !mIsRightSide;
  
  //set low side to ground
  BTS7960bHalf::setIntensity(0.0);
  
  uint8_t oldEnPin = mEnPin;
  uint8_t oldPwmPin = mPwmPin;
  uint8_t oldIsPin = mIsPin;

  mEnPin = mSpareEnPin;
  mPwmPin = mSparePwmPin;
  mIsPin = mSpareIsPin;

  mSpareEnPin = oldEnPin;
  mSparePwmPin = oldPwmPin;
  mSpareIsPin = oldIsPin;

}

float BTS7960bHBridge::getIntensity(){
  if (mIsRightSide) return mIntensity;
  return -mIntensity;
}


