#ifndef THREEPINFAN_H
#define THREEPINFAN_H

class ThreePinFanPWM
{
  public:
    ThreePinFanPWM(uint8_t pinOut);
    ThreePinFanPWM(uint8_t pinOut, uint8_t pinIn);
    void setSpeed(float percentage);
    void controlSpeed(float rpm);
    
  private:
    uint8_t mPinOut, mPinIn;
    float mPercentage = 0;
    float mRpm, mLastRpm = 0;
};

#endif

