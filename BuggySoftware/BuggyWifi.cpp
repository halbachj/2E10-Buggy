#include "BuggyWifi.hpp"
#include "Logger.hpp"

using LogLevel = mcu::Logger::LogLevel;

void BuggyWiFi::printWiFiStatus() {
  // print the SSID of the network you're attached to:
  mcu::logger << "SSID: " << WiFi.SSID() << mcu::LeanStreamIO::endl;
  // print your WiFi shield's IP address:
  mcu::logger << "IP Address: " << String("lool").c_str() << mcu::LeanStreamIO::endl;
  Serial.println(WiFi.localIP());
}

void BuggyWiFi::setup_ap() {
  // WiFi.config(IPAddress(192,48,56,2)); // Change the IP
  // print the network name (SSID);
  mcu::logger << "Creating access point named: " << ssid << mcu::LeanStreamIO::endl;
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    mcu::logger << "Creating access point failed" << mcu::LeanStreamIO::endl;
    while (true);
  }
}

void BuggyWiFi::wifi_checks() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    mcu::logger << "Communication with WiFi module failed!" << mcu::LeanStreamIO::endl;
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    mcu::logger << "Please upgrade the firmware" << mcu::LeanStreamIO::endl;
  }
}

void BuggyWiFi::update() {
  // compare the previous status to the current status
  if (this->status != WiFi.status()) {
    // it has changed update the variable
    this->status = WiFi.status();

    if (this->status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      mcu::logger << "Device connected to AP" << mcu::LeanStreamIO::endl;
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      mcu::logger << "Device disconnected from AP" << mcu::LeanStreamIO::endl;
    }
  }
}

bool BuggyWiFi::isSetup() {
  return setupDone;
}

BuggyWiFi::BuggyWiFi() {
  
}
