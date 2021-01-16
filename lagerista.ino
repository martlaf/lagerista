#include <LiquidCrystal.h>
#include <PID_v1.h>  //Thanks to Brett Beauregard
#include "Thermistor.h"
#include "ThreePinFanPWM.h"
#include "BTS7960b.h"

//switch variables
#define SWITCHDOWNPIN 0
#define SWITCHUPPIN 1
#define BACKLIGHTPIN A5

double errorTemp, controlVal, setTemp;
double kP = 400.0, kI = 200.0, kD = 10.0;
PID pid(&errorTemp, &controlVal, &setTemp, kP, kI, kD, DIRECT);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Thermistor probe(14);
ThreePinFanPWM fan(6);
BTS7960bHBridge peltier(8, 10, 7, 9);

void setup() {
  lcd.begin(16, 2);
  peltier.setActive();
  pid.SetMode(AUTOMATIC);

  pinMode(SWITCHUPPIN, INPUT_PULLUP);
  pinMode(SWITCHDOWNPIN, INPUT_PULLUP);
  pinMode(BACKLIGHTPIN, OUTPUT);
  digitalWrite(BACKLIGHTPIN, LOW);

  analogReference(EXTERNAL);

}


void loop() {

  probe.update();
  pid.Compute();

  static float fanSpeed = 50.0;
  fan.setSpeed(fanSpeed);

  peltier.setIntensity(-100.0);
  //Serial.begin(9600);


  if (probe.isValid()) {
    //get user temp
    lcd.setCursor(0, 0);
    lcd.print("Set=");
    lcd.print(String(getUserTemp(), 1));

    //print power percentage sent to Peltier device
    lcd.setCursor(10, 0);
    lcd.print("P=");
    float power = peltier.getIntensity();
    if (power > 0.) lcd.print(" ");
    if (abs(power) < 100.) lcd.print(" ");
    lcd.print(String(power, 0));

    //print measured temp
    lcd.setCursor(0, 1);
    lcd.print("Cur=");
    String strTemp = probe.strTemp(1);
    lcd.print(strTemp);
    for (int i = 0; i < 6 - strTemp.length(); i++) lcd.print(" ");

    //print fan speen in percentage
    lcd.setCursor(10,1);
    lcd.print("F= ");
    if (fanSpeed < 100.) lcd.print(" ");
    lcd.print(String(fanSpeed, 0));

  } else {
    lcd.setCursor(0, 0);
    lcd.print("               ");
    lcd.setCursor(0, 1);
    lcd.print("               ");
  }


}




float getUserTemp() {
  static float set_temp = 20.0;
  static unsigned long sw_timer;
  static bool sw_up = 0, sw_hold_up = 0, sw_down = 0, sw_hold_down = 0;

  unsigned long currentMillis = millis();

  if (!digitalRead(SWITCHDOWNPIN) && !sw_down) {
    digitalWrite(BACKLIGHTPIN, HIGH);
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
    digitalWrite(BACKLIGHTPIN, HIGH);
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
    if (millis() > sw_timer + 10000) digitalWrite(BACKLIGHTPIN, LOW);
  }

  return set_temp;
}
