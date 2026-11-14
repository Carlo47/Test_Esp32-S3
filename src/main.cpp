/**
 * Program    Test_Esp32-S3
 *
 * Author     2026-11-12 Charles Geiser (www.dodeka.ch)
 *
 * Purpose    Tests the RGB LED by displaying the colors R, G, B, WHITE,
 *            and BLACK in sequence, or by cycling through hue values
 *            from 0 to 65536 in the HSV color model in selectable increments.
 *            The WaitTimer functor from ToolkitCore is used to ensure
 *            a non-blocking process.
 *
 *            The Wi-Fi connection is verified by establishing a connection
 *            to the router—either using predefined login credentials or
 *            using login credentials entered on the cell phone via the
 *            “AutoConnect” access point launched by the ESP32.
 *
 *            The local time is then retrieved from an NTP server and
 *            displayed on the monitor.
 */

#include <Arduino.h>
#include "ToolkitCore.h"
#include "ToolkitWiFi.h"
#include <Adafruit_NeoPixel.h>

const gpio_num_t pinRGB    = GPIO_NUM_48;
const uint8_t    nbrPixels = 1;

Adafruit_NeoPixel    pixel(nbrPixels, pinRGB, NEO_GRB + NEO_KHZ800);
CoreUtils::WaitTimer itsTimeForNextColor(1000);
CoreUtils::WaitTimer itsTimeForNextHue(10);
CoreUtils::WaitTimer itsTimeToShowTime(15'000);

// Set this variable to `false`to test only the RGB primary colors.
bool testHue = true;

void cycleRGBWBlack()
{ //  Red         Green         Blue        White       Black
  static const uint32_t color[]   = {0x00FF0000, 0x0000FF00, 0x000000FF, 0x00FFFFFF, 0x00000000};
  static const int      nbrColors = sizeof(color) / sizeof(color[0]);
  static int            i         = 0;
  pixel.setPixelColor(0, color[i++]);
  pixel.show();
  i %= nbrColors;
}

void cycleHue(uint16_t step)
{
  static uint16_t hue = 0;
  uint32_t        col = Adafruit_NeoPixel::ColorHSV(hue);
  pixel.setPixelColor(0, col);
  pixel.show();
  hue += step;
}

void setup()
{
  Serial.begin(MONITOR_SPEED);
  CoreUtils::printSystemInfo();

  // Set the next condition to “true” to use the specified login credentials,
  // or to ‘false’ to have your mobile device provide the login credentials
  // via the “autoConnect” access point and save them permanently in the
  // ESP32's preferences.
  if (false)
    WiFiUtils::connectToWiFi("SSID", "PASSWORD", "esp32-s3");
  else
    WiFiUtils::autoConnectToWiFi("esp32-s3");

  WiFiUtils::printNearbyNetworks();
  WiFiUtils::printConnectionDetails();

  RTCUtils::initRTC(RTCUtils::MEZ_MESZ, "ch.pool.ntp.org");

  pixel.begin();

  if (testHue)
    itsTimeForNextHue.begin();
  else
    itsTimeForNextColor.begin();

  // Wait until the seconds are a multiple of 15 to start showing the time
  while (time(NULL) % 15 != 0)
    vTaskDelay(pdMS_TO_TICKS(5));
  itsTimeToShowTime.begin();
}

void loop()
{
  if (itsTimeToShowTime())
    RTCUtils::printDateTime(3);

  if (testHue)
  {
    if (itsTimeForNextHue())
      cycleHue(64);
  }
  else
  {
    if (itsTimeForNextColor())
      cycleRGBWBlack();
  }
}
