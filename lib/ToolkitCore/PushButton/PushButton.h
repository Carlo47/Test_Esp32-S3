/**
 * Header       PushButton.h
 *
 * Author       2021-10-06 Charles Geiser (https://www.dodeka.ch)
 *
 * Purpose      Declaration of the class PushButton
 *
 * Constructor
 * arguments    pin   Input pin to which the pushbutton is conneted
 *              mode  GPIO_PULLUP_ONLY   --> button pressed == LOW
 *                    GPIO_PULLDOWN_ONLY --> button pressed == HIGH 
 */

#pragma once
#include <Arduino.h>

namespace Debounced
{
  class PushButton
  {
    public:
      using Callback = std::function<void()>;

      PushButton(gpio_num_t pin, gpio_pull_mode_t mode = GPIO_PULLUP_ONLY) : _pin(pin), _mode(mode)
      {
        gpio_reset_pin(_pin);
        gpio_set_direction(_pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(_pin, _mode);
      }

      void addOnClick(Callback cb);
      void addOnLongClick(Callback cb);
      void addOnDoubleClick(Callback cb);

      void loop();

    private:
      // Functor
      struct Nop
      {
          void operator()() const
          {
          }
      };
      static constexpr Nop nop{};
      const gpio_num_t     _pin;
      gpio_pull_mode_t     _mode;
      Callback             _onClick{nop};
      Callback             _onLongClick{nop};
      Callback             _onDoubleClick{nop};
      uint8_t              _clickCount{0};
      int                  _btnState{_mode==GPIO_PULLUP_ONLY ? HIGH : LOW};
      uint32_t             _msDebounce  = 30;  // After 30ms the button should have reached a steady state
      uint32_t             _msLongClick = 300; // A button that is held longer than 300ms is considered as longClick
      uint32_t             _msDoubleClickGap = 250; // Two button clicks within 250ms count as a doubleClick
      uint32_t             _msButtonDown{0};
      uint32_t             _msFirstClick{0};
  };
} // namespace Debounced