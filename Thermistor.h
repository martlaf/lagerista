#ifndef THERMISTOR_H
#define THERMISTOR_H

#define NUMSAMPLES 100
#define SERIESRESISTOR 10000 
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 100
#define BCOEFFICIENT 4016.4

class Thermistor
{
  public:
    Thermistor(int pin);
    float getTemp();
    String strTemp(int precision);
    void update();
    void print();
    bool isValid();

  private:
    float mValues [NUMSAMPLES];
    uint8_t mIndex = 0;
    int mPin;
    float mTemp;
};

#endif
