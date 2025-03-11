#include "BuggyWifi.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

void BuggyWiFi::printWiFiStatus() {
  // print the SSID of the network you're attached to:
  logger << logLevel::INFO << "SSID: " << WiFi.SSID() << EmbeddedLogger::endl;
  // print your WiFi shield's IP address:
  logger << logLevel::INFO << "IP Address: " << String("lool").c_str() << EmbeddedLogger::endl;
  Serial.println(WiFi.localIP());
}

void BuggyWiFi::setup_ap() {
  // WiFi.config(IPAddress(192,48,56,2)); // Change the IP
  // print the network name (SSID);
  logger << logLevel::INFO << "Creating access point named: " << ssid << EmbeddedLogger::endl;
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    logger << logLevel::ERROR << "Creating access point failed" << EmbeddedLogger::endl;
    while (true);
  }
}

void BuggyWiFi::wifi_checks() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    logger << logLevel::ERROR << "Communication with WiFi module failed!" << EmbeddedLogger::endl;
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    logger << logLevel::WARNING << "Please upgrade the firmware" << EmbeddedLogger::endl;
  }
}

void BuggyWiFi::update() {
  // compare the previous status to the current status
  if (this->status != WiFi.status()) {
    // it has changed update the variable
    this->status = WiFi.status();

    if (this->status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      logger << logLevel::INFO << "Device connected to AP" << EmbeddedLogger::endl;
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      logger << logLevel::INFO << "Device disconnected from AP" << EmbeddedLogger::endl;
    }
  }
}

bool BuggyWiFi::isSetup() {
  return setupDone;
}
