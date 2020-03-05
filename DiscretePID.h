#ifndef DISCRETEPID_H
#define DISCRETEPID_H

class DiscretePID
{
  public:
    DiscretePID(float Kp, float Ki, float Kd): mKp(Kp), mKi(Ki), mKd(Kd) {};
    double compute(double error);

  private:
    float mKp, mKi, mKd;
    double mPTerm, mITerm, mDTerm;
    double mLastD = 0, mLastI = 0;
};

#endif

