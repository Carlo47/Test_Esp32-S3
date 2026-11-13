#pragma once
#include "esp_chip_info.h"
#include <Arduino.h>

namespace CoreUtils
{
  void printSystemInfo();
  void printPartitions();
} // namespace CoreUtils