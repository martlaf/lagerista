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
  
  float measuredTemp = getMeasuredTemp();

  printMeasuredTemp(measuredTemp);
  
  lcd.setCursor(0, 0);
  lcd.print(getUserTemp());

}

void printMeasuredTemp(float temp) {
  lcd.setCursor(0,1);
  if (temp < -30.0) {
    lcd.print("      ");
  } else {
    lcd.print(temp);
  }
  
}

float getMeasuredTemp() {

  static float values[NUMSAMPLES];
  static uint8_t iteration = 0;
  
  float sum = 0, average, steinhart;

  values[iteration] = analogRead(THERMISTORPIN);
  iteration++;
  if (iteration == NUMSAMPLES) iteration = 0;

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

  return steinhart;
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

