#ifndef BUGGY_WIFI_HPP
#define BUGGY_WIFI_HPP

#include "WiFiS3.h"
#include "secrets.h"


/**
 * @class BuggyWifi
 * @brief The Buggy WiFi class manages the WiFi connection for the buggy.
 * @details It handles the setup of the WiFi access point. The class also
 * has methods to performs WiFi status checks which can update the
 * builtin LED, which is used as a status LED.
 */
class BuggyWiFi {
private:
  bool setupDone = false;
  const char* ssid = SECRET_SSID;
  const char* pass = SECRET_PASS;
  int status = WL_IDLE_STATUS;

  const pin_size_t  wifi_led = LED_BUILTIN; // Builtin LED pin 13

public:
  BuggyWiFi() = default;
  /**
   * @brief Sets up the access point. Needs to be called during setup
   */
  void setup_ap();
  /**
   * @brief Will perform the WiFi status check
   */
  void wifi_checks();
  /**
   * @brief Returns if the AP was setup
   */
  bool isSetup();
  /**
   * @brief Is supposed to be used inside the main loop to update the status LED.
   */
  void update();
  /**
   * @brief Log the current WiFi status.
   */
  void printWiFiStatus();
};

#endif //BUGGY_WIFI_HPP
