#include <LiquidCrystal.h>
#include <PID_v1.h>  //Thanks to Brett Beauregard
#include "Thermistor.h"
#include "ThreePinFanPWM.h"
#include "BTS7960b.h"

//switch variables
#define SWITCHDOWNPIN 0
#define SWITCHUPPIN 1

double errorTemp, controlVal, setTemp;
double kP = 400.0, kI = 200.0, kD = 10.0;
PID pid(&errorTemp, &controlVal, &setTemp, kP, kI, kD, DIRECT);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Thermistor probe(14);
ThreePinFanPWM fan(6);
BTS7960bHBridge peltier(7,9,8,10);

void setup() {
  lcd.begin(16, 2);
  peltier.setActive();
  pid.SetMode(AUTOMATIC);
  
  pinMode(SWITCHUPPIN, INPUT_PULLUP);
  pinMode(SWITCHDOWNPIN, INPUT_PULLUP);

  analogReference(EXTERNAL);

}

  
void loop() {
  
  probe.update();
  pid.Compute();
  
  fan.setSpeed(100.0);
  
  if (probe.isValid()) {
    
    
    // printing value to LCD
    lcd.setCursor(0, 1);
    String strTemp = probe.strTemp(1);
    lcd.print(strTemp);
    for (int i = 0; i < 6 - strTemp.length(); i++) lcd.print(" ");
  } else {
    lcd.print("      ");
  }

  lcd.setCursor(0, 0);
  lcd.print(String(getUserTemp(), 1));

  
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


