/**
 * Class        PushButton.cpp
 *
 * Author       2021-10-06 Charles Geiser (https://www.dodeka.ch)
 *
 * Purpose      Debounces a pushbutton and handles the user supplied actions
 *                  - onClick()
 *                  - onLongClick()
 *                  - onDoubleClick()
 *
 * Board        ESP32 DoIt DevKit V1
 *
 * Wiring
 *                                                    GPIO
 *                       .-------.                      ^
 *               GPIO <--o       |                      |
 *                Vcc <--o  [o]  |              _T_     |    ___
 *                GND <--o       |     GND o----o o-----+---|___|----> Vcc
 *                       '-------'                           220
 *
 * Usage        Connect a pushbutton from PIN to GND
 *              Supply the needed callbacks by calling
 *                    addOnClickCB(onClick),
 *                    addOnDoubleClickCB(onDoubleClick),
 *                    addOnLongClickCB(onLongClick)
 *              Call btn.loop() in the main loop of your program
 *
 * References
 */

#include "PushButton.h"

namespace Debounced
{
  void PushButton::addOnClick(Callback cb)
  {
    _onClick = cb;
  };

  void PushButton::addOnLongClick(Callback cb)
  {
    _onLongClick = cb;
  };

  void PushButton::addOnDoubleClick(Callback cb)
  {
    _onDoubleClick = cb;
  };

  void PushButton::loop()
  {
    int prevState = _btnState;
    _btnState     = gpio_get_level(_pin); // query button, pressed is LOW

    if (prevState == HIGH && _btnState == LOW) // button pressed
    {
      _msButtonDown = millis(); // remember time
    }
    else if (prevState == LOW && _btnState == HIGH) // release button ...
    {
      if (millis() - _msButtonDown < _msDebounce) // ... button bounces
      {
        // Prellen ignorieren
      }
      else if (millis() - _msButtonDown > _msLongClick) // time greater than 300 ms
      {
        _onLongClick();
      }
      else
      {
        _clickCount++; // remember time only of 1st clicks
        if (_clickCount == 1)
          _msFirstClick = millis();
      }
    }
    else
    { // this branch is run through if there is nothing to do in the loop
      if (_clickCount == 1 && millis() - _msFirstClick > _msDoubleClickGap) // time after 1st click has expired
      {
        _msFirstClick = 0;
        _clickCount   = 0;
        _onClick();
      }
      else if (_clickCount > 1) // more than one click occured
      {
        _msFirstClick = 0;
        _clickCount   = 0;
        _onDoubleClick();
      }
    }
  }
} // namespace Debounced