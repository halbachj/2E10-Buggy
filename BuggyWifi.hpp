#ifndef BUGGY_WIFI_HPP
#define BUGGY_WIFI_HPP

#include "helpers.hpp"
#include "WiFiS3.h"
#include "secrets.h"


/*
 * BUGGY WIFI CLASS
 * The Buggy WiFi class manages the WiFi connection for the buggy. It handles the setup
 * of the WiFi network, ensuring the buggy connects to the specified network using
 * the provided SSID and password. The class also performs regular WiFi status checks
 * and provides methods for printing the current WiFi status. Additionally, it manages
 * a built-in LED to indicate the connection status.
 */
class BuggyWiFi {
private:
  bool setupDone = false;
  const char* ssid = SECRET_SSID;
  const char* pass = SECRET_PASS;
  int status = WL_IDLE_STATUS;

  PIN_TYPE wifi_led = LED_BUILTIN; // Builtin LED pin 13

public:
  BuggyWiFi();
  void setup_ap();
  void wifi_checks();
  bool isSetup();
  void update();
  void printWiFiStatus();
};

#endif //BUGGY_WIFI_HPP
