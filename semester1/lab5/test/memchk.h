#pragma once

#include "gtest/gtest.h"

/* Testing namespace */
namespace testing
{
  /* Memory leak checker class */
  class MemoryLeakDetector : public EmptyTestEventListener
  {
#ifdef _DEBUG
  public:
    virtual void OnTestStart(const TestInfo&)
    {
      _CrtMemCheckpoint(&memState_);
    }

    virtual void OnTestEnd(const TestInfo& test_info){
      if(test_info.result()->Passed())
      {
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
        {
          FAIL() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";
        }
      }
    }

  private:
    _CrtMemState memState_;
#endif // _DEBUG
  }; /* End of 'MemoryLeakDetector ' class */
} /* End of 'testing' namespace */
