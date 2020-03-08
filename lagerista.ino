#include <LiquidCrystal.h>
#include "Thermistor.h"
#include "DiscretePID.h"
#include "ThreePinFanPWM.h"

//drive variables
#define HEAT_PWM_PIN 10
#define COOL_PWM_PIN 11
#define HEAT_EN_PIN 12
#define COOL_EN_PIN 13

//fan variables
//#define FANPWMPIN 6

//switch variables
#define SWITCHDOWNPIN 0
#define SWITCHUPPIN 1

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Thermistor probe(14);
DiscretePID pid(400, 200, 10);
ThreePinFanPWM fan(6);


void setup() {
  lcd.begin(16, 2);

  pinMode(SWITCHUPPIN, INPUT_PULLUP);
  pinMode(SWITCHDOWNPIN, INPUT_PULLUP);

  analogReference(EXTERNAL);

}

  
void loop() {
  
  probe.update();
  
  lcd.setCursor(0, 1);
  
  String strTemp = probe.strTemp(1);
  
  if (probe.isValid()) {
    lcd.print(strTemp);
    for (int i = 0; i < 6 - strTemp.length(); i++) lcd.print(" ");
  } else {
    lcd.print("      ");
  }

  lcd.setCursor(0, 0);
  lcd.print(String(getUserTemp(), 1));

  fan.setSpeed(100);
  
}


float getUserTemp() {
  static float set_temp = 20.0;
  static unsigned long sw_timer;
  static bool sw_up = 0, sw_hold_up = 0, sw_down = 0, sw_hold_down = 0;

  unsigned long currentMillis = millis();

  if (!digitalRead(SWITCHDOWNPIN) && !sw_down) {
    sw_down = true;
    sw_timer = currentMillis;
    set_temp -= 0.1;
  } else if (!digitalRead(SWITCHDOWNPIN) && sw_down) {
    if ((!sw_hold_down) && (currentMillis >= sw_timer + 1000.0)) {
      sw_hold_down = true;
      sw_timer = currentMillis;
      set_temp -= 0.1;
    } else if (sw_hold_down && (currentMillis > sw_timer + 200.0)) {
      sw_timer = currentMillis;
      set_temp -= 0.1;
    }
  } else if (!digitalRead(SWITCHUPPIN) && !sw_up) {
    sw_up = true;
    sw_timer = currentMillis;
    set_temp += 0.1;
  } else if (!digitalRead(SWITCHUPPIN) && sw_up) {
    if (!sw_hold_up && (currentMillis > sw_timer + 1000)) {
      sw_hold_up = true;
      sw_timer = currentMillis;
      set_temp += 0.1;
    } else if (sw_hold_up && (currentMillis > sw_timer + 200)) {
      sw_timer = currentMillis;
      set_temp += 0.1;
    }
  } else {
    sw_down = false;
    sw_hold_down = false;
    sw_up = false;
    sw_hold_up = false;
  }

  return set_temp;
}


