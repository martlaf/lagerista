#ifndef BTS7960B_H
#define BTS7960B_H
using namespace std;

class BTS7960bHalf
{
  public:
    BTS7960bHalf(uint8_t enPin, uint8_t pwmPin);
    BTS7960bHalf(uint8_t enPin, uint8_t pwmPin, uint8_t isPin);
    void setIntensity(float intensity);
    void setActive();
    void setInactive();
    float getIntensity();
    bool isActive();
    
  protected:
    uint8_t mEnPin, mPwmPin, mIsPin;
    float mIntensity = 0;
    bool mEnable = 0;
};


class BTS7960bHBridge : public BTS7960bHalf
{
  public:
    BTS7960bHBridge(uint8_t r_en, uint8_t r_pwm, uint8_t l_en, uint8_t l_pwm);
    void setIntensity(float intensity);
    float getIntensity();
    
  private:
    void switchSides();
    uint8_t mSpareEnPin, mSparePwmPin, mSpareIsPin;
    bool mIsRightSide = 1;
};


#endif
