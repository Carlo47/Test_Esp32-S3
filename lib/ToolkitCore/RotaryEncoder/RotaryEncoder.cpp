/**
 * Class      DebouncedRotaryEncoder implementation
 *
 * Author     2024-11-12 Charles Geiser (https://www.dodeka.ch)
 *
 * Purpose    Debounces a rotary encoder without the need of delays
 *
 * Remarks    Principle of operation
 *                    ______          ______
 *            ____||      ||______||         Clk
 *                :       :       :
 *               ______   :      ______
 *            _|| :    ||______|| :    ||_   Data
 *                :       :       :
 *                :_______:       :_______
 *            ____|       |_______|          Cleaned clock (copy stable data value on jittering clock edge)
 *
 *            Clockwise rotation if the previous clean clock state was 0 and the
 *                current clock state is 1.
 *            Counterclockwise rotation if the previous clean clock state was 1 and
 *                the current clock state is 1.
 *            The clock states are logically linked with each other in order to react
 *                to only one edge during a rotation step.
 *
 */

// #include <Arduino.h>
#include "RotaryEncoder.h"

// extern Action onCW();   // function that is executed when turning clockwise
// extern Action onCCW();  // function that is executed when turning counterclockwise
namespace Debounced
{

  void RotaryEncoder::loop()
  {
    _clkState  = gpio_get_level(_pinClock);
    _dataState = gpio_get_level(_pinData);

    // Debouncing rotary encoder
    if (_prevClkState != _clkState) // Clock change detected (even bouncing)
    {
      _prevClkState    = _clkState;
      _cleanedClkState = _dataState; // Copy data state to get clean clock
      // Serial.println("bouncing");

      if (_prevCleanedClkState != _cleanedClkState) // Clock change detected
      {
        /*
        char s[60];
        sprintf(s, "clean clock changed from %d to %d with clk %d", _prevCleanedClkState, _cleanedClkState, _clkState);
        Serial.println(s);
        */

        if (_prevCleanedClkState == LOW && _clkState == 1)
          _onCW();
        if (_prevCleanedClkState == HIGH && _clkState == 1)
          _onCCW();
      }
      _prevCleanedClkState = _cleanedClkState;
    }
  }
} // namespace Debounced
