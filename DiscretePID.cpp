#include "DiscretePID.h"

DiscretePID::DiscretePID(float Kp, float Ki, float Kd)
{
  float mKp = Kp;
  float mKi = Ki;
  float mKd = Kd;
}

double DiscretePID::compute(double error)
{
  double mPTerm = mKp * error;

  mLastI += error;
  double mITerm = mKi * mLastI;

  mDTerm = mKd * (mLastD - error);
  mLastD = error;
  
}

