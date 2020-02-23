#include "Arduino.h"
#include "Thermistor.h"

Thermistor::Thermistor(int pin)
{
  pinMode(pin, INPUT);
  mPin = pin;
}


void Thermistor::update() {
  mValues[mIndex] = analogRead(mPin);
  mIndex++;
  if (mIndex == NUMSAMPLES) mIndex = 0;
}


float Thermistor::getTemp() {
  
  float sum = 0, average, steinhart;

  for (uint8_t i = 0; i < NUMSAMPLES; i++) sum += mValues[i];
  average = sum / NUMSAMPLES;
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  mTemp = average / THERMISTORNOMINAL;     // (R/Ro)
  mTemp = log(mTemp);                  // ln(R/Ro)
  mTemp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  mTemp += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  mTemp = 1.0 / mTemp;                 // Invert
  mTemp -= 273.15;                         // convert to C

  return mTemp;
}

String Thermistor::strTemp(int precision) {
  return String(mTemp,1);
}

bool Thermistor::isValid() {
  if (mTemp < 100.0 && mTemp > 0.0) {
    return true;
  } else {
    return false;
  }
}


