#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "timezones.h"

namespace RTCUtils
{
  void initRTC(const char timeZone[], const char ntpServerPool[], bool disconnect = false);
  void printDateTime(int format);
} // namespace RTCUtils