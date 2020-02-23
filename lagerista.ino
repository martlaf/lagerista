#include <LiquidCrystal.h>
#include "Thermistor.h"


//drive variables
#define HEAT_PWM_PIN 10
#define COOL_PWM_PIN 11
#define HEAT_EN_PIN 12
#define COOL_EN_PIN 13

//fan variables
#define FANPWMPIN 6

//switch variables
#define SWITCHDOWNPIN 0
#define SWITCHUPPIN 1


// LCD pins
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Thermistor temperatureProbe(14);

void setup() {
  lcd.begin(16, 2);

  pinMode(FANPWMPIN, OUTPUT);
  analogWrite(FANPWMPIN, 0);
  
  pinMode(SWITCHUPPIN, INPUT);
  pinMode(SWITCHDOWNPIN, INPUT);

  analogReference(EXTERNAL);

}

void loop() {
  temperatureProbe.update();
  
  lcd.setCursor(0,1);

  
  float temp = temperatureProbe.getTemp();
  String strTemp = String(temp,1);
  
  if (temperatureProbe.isValid()) {
    lcd.print(String(temp,1));
    for (int i=0; i < 6 - strTemp.length(); i++) lcd.print(" ");
  } else {
    lcd.print("      ");
  }
  
  //printMeasuredTemp(temperatureProbe.getTemp());
  
  lcd.setCursor(0, 0);
  lcd.print(String(getUserTemp(),1));

}

void printMeasuredTemp(float temp) {
  char string [16];
  int character;
  if (temp < -30.0) {
    lcd.print("       ");
  } else {
    //character = 
    //lcd.print(temp2str(temp,3));
    //sprintf(string,"%.1f",temp);
    //lcd.print(std::setprecision(1)<<temp);
  }
  
}

float getUserTemp() {
  static float set_temp = 20.0;
  static unsigned long sw_timer;
  static bool sw_up = 0, sw_hold_up = 0, sw_down = 0, sw_hold_down = 0;

  unsigned long currentMillis = millis();
  
  if (!digitalRead(SWITCHDOWNPIN) && !sw_down) {
      sw_down=true;
      sw_timer=currentMillis;
      set_temp -= 0.1;
  } else if (!digitalRead(SWITCHDOWNPIN) && sw_down) {
    if ((!sw_hold_down) && (currentMillis >= sw_timer + 1000.0)){
      sw_hold_down = true;
      sw_timer = currentMillis;
      set_temp -= 0.1;
    } else if (sw_hold_down && (currentMillis > sw_timer + 200.0)) {
      sw_timer = currentMillis;
      set_temp -= 0.1;
    }
  } else if (!digitalRead(SWITCHUPPIN) && !sw_up) {
      sw_up=true;
      sw_timer=currentMillis;
      set_temp += 0.1;
  } else if (!digitalRead(SWITCHUPPIN) && sw_up) {
    if (!sw_hold_up && (currentMillis > sw_timer + 1000)){
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

