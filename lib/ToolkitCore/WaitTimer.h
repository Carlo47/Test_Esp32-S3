#include <Arduino.h>

/**
 * Author       2026-07-22 Charles Geiser and Copilot
 *
 * Purpose      Implements a WaitTimer by means of a functor.
 *              A functor is a function object that overloads the
 *              call operator `operator()` and preserves the state
 *              of its variables between calls.
 *
 * Usage        Call the timer functor in the main loop()
 *
 * Examples     CoreUtils::WaitTimer everySecond; // default constructor, ms to wait = 1000, doit agai and again
 *              everySecond.begin();
 *              if (doItContinuously()) doSomething();
 *
 *              CoreUtils::WaitTimer doItManyTimes(2000, 15); // ms to wait = 2000, do it 15 times
 *              doIt15Times.begin();
 *              if (doIt15Times()) doSomething();
 *
 *              doItManyTimes.reset(10000, 3); reset timer to 10000 ms and number of runs to 3
 */
namespace CoreUtils
{
  // Functor
  struct WaitTimer
  {
      uint32_t _msStart = millis();
      uint32_t msToWait{1000}; // default 1 second
      uint32_t count{0};
      int      remainingRuns{0}; // default 0 = run continuously
      bool     done{false};
      bool     _autoReload{remainingRuns <= 0}; // run continuously if remainingRuns <= 0
      WaitTimer() = default;
      WaitTimer(uint32_t msWaitTime, int runs = 0) : _msStart(millis()), msToWait(msWaitTime), remainingRuns(runs)
      {
      }

      bool operator()()
      {
        auto _msEllapsed = millis() - _msStart;
        if (!done)
        {
          if (_msEllapsed >= msToWait) // time has exspired
          {
            if (_autoReload)
            {
              _msStart = millis(); // reload the timer
              count++;
              return true; // return true to signal time has expired
            }
            if (remainingRuns > 0) // decrement if > 0
            {
              remainingRuns--;
              _msStart = millis(); // reload the timer to do another run
              count++;
              return true;
            }
            else
            {
              done = true; // all runs hve done
            }
          }
        }
        return false;
      }

      // reset _msStart to current millis()
      void begin()
      {
        _msStart = millis();
      }

      // reset timer to new values and set _msStart to current millis()
      void reset(uint32_t msWaitTime, int runs = 0)
      {
        msToWait      = msWaitTime;
        remainingRuns = runs;
        count         = 0;
        done          = false;
        _msStart      = millis();
      }
  };
} // namespace CoreUtils