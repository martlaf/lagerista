#include "DiscretePID.h"


double DiscretePID::compute(double error)
{
  double mPTerm = mKp * error;

  mLastI += error;
  double mITerm = mKi * mLastI;

  mDTerm = mKd * (mLastD - error);
  mLastD = error;
  
}


