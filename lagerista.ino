#include <LiquidCrystal.h>

//thermistor variables
#define SERIESRESISTOR 10000 
#define THERMISTORPIN A0
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 100
#define BCOEFFICIENT 4016.4

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
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  lcd.begin(16, 2);

  pinMode(FANPWMPIN, OUTPUT);
  analogWrite(FANPWMPIN, 0);
  
  pinMode(SWITCHUPPIN, INPUT);
  pinMode(SWITCHDOWNPIN, INPUT);

  analogReference(EXTERNAL);

}

void loop() {


  static uint8_t i = 0;
  float sum = 0, average, steinhart;
  static float values[NUMSAMPLES];

  values[i] = analogRead(THERMISTORPIN);
  i++;
  if (i == NUMSAMPLES) i = 0;

  for (uint8_t index = 0; index < NUMSAMPLES; index++) sum += values[index];
  average = sum / NUMSAMPLES;
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C


  
  lcd.setCursor(0, 0);
  lcd.print(getUserTemp());
  
  lcd.setCursor(0, 1);
  lcd.print(average);

}


float getUserTemp() {
  static float set_temp = 20;
  static unsigned long milliseconds;
  static unsigned long sw_timer;
  static bool sw_up = 0, sw_hold_up = 0, sw_down = 0, sw_hold_down = 0;
  milliseconds = millis();
  if (!digitalRead(SWITCHDOWNPIN) && !sw_down) {
      sw_down=true;
      sw_timer=milliseconds;
      set_temp -= 0.1;
  } else if (!digitalRead(SWITCHDOWNPIN) && sw_down) {
    if ((!sw_hold_down) && (milliseconds >= (sw_timer + 1000.0))){
      set_temp -= 0.1;
      sw_hold_down = true;
      sw_timer = milliseconds;
    } else if (sw_hold_down && (milliseconds > (sw_timer + 200.0))) {
      set_temp -= 0.1;
      sw_timer = milliseconds;
    }
  } else if (!digitalRead(SWITCHUPPIN) && !sw_up) {
      sw_up=true;
      sw_timer=milliseconds;
      set_temp += 0.1;
  } else if (!digitalRead(SWITCHUPPIN) && sw_up) {
    if (!sw_hold_up && (milliseconds > sw_timer + 1000)){
      sw_hold_up = true;
      sw_timer = milliseconds;
    } else if (sw_hold_up && (milliseconds > sw_timer + 200)) {
      set_temp += 0.1;
      sw_timer = milliseconds;
    }
  } else {
    sw_down = false;
    sw_hold_down = false;
    sw_up = false;
    sw_hold_up = false;
  }

  return set_temp;
}  

