/**
 * Class         DebouncedRotaryEncoder declaration
 *
 * Author        2024-11-12 Charles Geiser (https://www.dodeka.ch)
 *
 * Purpose       Debounces a rotary encoder without the need of delays
 *
 * Constructor
 * arguments    pinClock    clock pin of the rotary encoder
 *              pinData     data pin of the rotary encoder
 *              onCW        reference to the function to be executed when the
 *                            rotary shaft is turned clockwise
 *              onCCW       reference to the function to be executed when the
 *                            rotary shaft is turned counter clockwise
 *
 * Remarks      Principle of operation
 *                    ______          ______
 *              ____||      ||______||         Clk
 *                  :       :       :
 *                 ______   :      ______
 *              _|| :    ||______|| :    ||_   Data
 *                  :       :       :
 *                  :_______:       :_______
 *              ____|       |_______|          Cleaned clock (copy stable data value on jittering clock edge)
 *
 *              Clockwise rotation if the previous clean clock state was 0 and the
 *                current clock state is 1.
 *              Counterclockwise rotation if the previous clean clock state was 1 and
 *                the current clock state is 1.
 *              The clock states are logically linked with each other in order to react
 *                to only one edge during a rotation step.
 *
 * References   http://www.technoblogy.com/show?1YHJ
 */

#pragma once
#include <Arduino.h>

// using Action = void (&)();

namespace Debounced
{

  class RotaryEncoder
  {
    public:
      using Callback = std::function<void()>;

      RotaryEncoder(gpio_num_t pinClock, gpio_num_t pinData, Callback onCW, Callback onCCW)
          : _pinClock(pinClock), _pinData(pinData), _onCW(onCW), _onCCW(onCCW)
      {
        gpio_reset_pin(pinClock);
        gpio_set_direction(pinClock, GPIO_MODE_INPUT);
        gpio_set_pull_mode(pinClock, GPIO_PULLUP_ONLY);
        gpio_reset_pin(pinData);
        gpio_set_direction(pinData, GPIO_MODE_INPUT);
        gpio_set_pull_mode(pinData, GPIO_PULLUP_ONLY);
      }

      void loop();

    private:
      int        _clkState;
      int        _prevClkState;
      int        _cleanedClkState;
      int        _prevCleanedClkState;
      int        _dataState;
      gpio_num_t _pinClock;
      gpio_num_t _pinData;
      Callback   _onCW;
      Callback   _onCCW;
  };
} // namespace Debounced
