#pragma once
#include <Arduino.h>
#include <WIFI.h>
#include "ESP32AutoConnect.h"
#include <ESPmDNS.h>
#include "mdns.h"

void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

namespace WiFiUtils
{
  void   autoConnectToWiFi(const char hostname[]);
  void   connectToWiFi(const char ssid[], const char password[], const char hostname[]);
  void   printConnectionDetails();
  void   printNearbyNetworks();
  bool   initESP32mDNS(const char hostname[], int port);
  String makeHostname(const char prefix[]);
  String getRealMDNSName();
} // namespace WiFiUtils