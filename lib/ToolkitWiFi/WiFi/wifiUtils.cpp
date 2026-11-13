#include "wifiUtils.h"

/**
 * Asynchronous WiFi-Eventhandler for connectToWiFi()
 */
void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
  const unsigned long RECONNECT_INTERVAL = 12345; // wait ~12 sec before retrying to connect
  uint8_t             reason;
  switch (event)
  {
    case ARDUINO_EVENT_WIFI_STA_START:
      log_i("event: %d WiFi start", event);
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      log_i("event %d: Connected!", event);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      reason = info.wifi_sta_disconnected.reason;
      log_i("event %d: Disconnected! Reason: %d: ", event, reason);
      vTaskDelay(pdMS_TO_TICKS(RECONNECT_INTERVAL));
      log_i("Try to establish the connection");
      WiFi.begin();
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      log_i("event %d: Got IP: %s", event, WiFi.localIP().toString().c_str());
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      log_i("event %d: IP lost", event);
      vTaskDelay(pdMS_TO_TICKS(500));
      WiFi.begin();
      break;
    case ARDUINO_EVENT_WIFI_OFF:
      log_i("event %d: WiFi off", event);
      break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
      log_i("event %d: WiFi stop", event);
      break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      log_i("event %d: Scan done", event);
      break;
    default:
      log_i("unhandled event: %d", event);
      break;
  }
}

namespace WiFiUtils
{
  /**
   * Connect to the WiFi with given credentials and set the hostname.
   * The function terminates when waitForConnectionResult() returns.
   */
  void connectToWiFi(const char ssid[], const char password[], const char hostname[])
  {
    uint8_t result;

    log_i("Try to connect to %s with %s\n", ssid, password);
    if (WiFi.status() == WL_CONNECTED)
      WiFi.disconnect(true, true);
    vTaskDelay(pdMS_TO_TICKS(500));
    WiFi.mode(WIFI_OFF);
    WiFi.persistent(false);
    WiFi.setSleep(false); // crucial for AsyncWebServer
    WiFi.setAutoReconnect(false);
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(onWiFiEvent); // Set the callback to capture WiFi events
    WiFi.setHostname(hostname);
    vTaskDelay(pdMS_TO_TICKS(500));
    WiFi.begin(ssid, password);
    result = WiFi.waitForConnectResult();
    log_i("==> done with result = %d", result);
  }

  void autoConnectToWiFi(const char hostname[])
  {
    ESP32AutoConnect ac(hostname);
    // Activate next line to remove stored credentials, run the code
    // and then recompile again with the line commented.
    // Next time you start, you have to reconnect to AutoConnectAP

    // ac.clearCredentials();
    ac.autoConnect();
  }

  /**
   * Use a raw string literal to print a formatted
   * string of WiFi connection details
   */
  void printConnectionDetails()
  {
    Serial.printf(R"(
Connection Details
------------------
SSID         %s
Hostname     %s
IP-Address   %s
MAC-Address  %s
RSSI         %d (received signal strength indicator)

)",
                  WiFi.SSID().c_str(),
                  // WiFi.hostname().c_str(),  // ESP8266
                  WiFi.getHostname(), // ESP32
                  WiFi.localIP().toString().c_str(),
                  WiFi.macAddress().c_str(),
                  WiFi.RSSI());
  }

  /**
   * Print nearby WiFi networks with SSID und RSSI
   */
  void printNearbyNetworks()
  {
    int n = WiFi.scanNetworks();
    printf(R"(
Nearby WiFi networks:
--------------------
)");
    for (int i = 0; i < n; i++)
    {
      printf("%s\t%d\r\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
  }

  String getRealMDNSName()
  {
    char mDNShostname[MDNS_NAME_BUF_LEN];
    if (mdns_hostname_get(mDNShostname) == ESP_OK)
    {
      return String(mDNShostname) + ".local";
    }
    return String("mDNShostname unknown");
  }

  bool initESP32mDNS(const char hostname[], int port)
  {
    // Start mDNS, set the hostname. mDNS adds ".local"
    if (!MDNS.begin(hostname))
    {
      log_e("starting mDNS failed");
      return false;
    }
    if (!MDNS.addService("http", "tcp", port))
    {
      log_e("adding mDNS service failed");
      return false;
    }
    log_i("mDNS started, hostname: %s", getRealMDNSName().c_str());
    return true;
  }

  /**
   * Creates a unique hostname using a given prefix and
   * part of the MAC address extracted from the chip ID
   */
  String makeHostname(const char prefix[])
  {
    char     hostname[32];
    uint64_t chipid = ESP.getEfuseMac();
    snprintf(hostname, 32, "%s%04X", prefix, (uint16_t) (chipid >> 32));
    log_i("==> done: %s", hostname);
    return String(hostname);
  }
} // namespace WiFiUtils