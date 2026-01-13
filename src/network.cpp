#include "network.h"
#include "config.h"
#include "secrets_wrapper.h"
#include "tft_display.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

namespace Network {

static WiFiMulti wifiMulti;

void addWifiNetworks() {
  const size_t wifiCount = getWifiNetworksCount();
  for (size_t i = 0; i < wifiCount; ++i) {
    wifiMulti.addAP(getWifiSsid(i), getWifiPassword(i));
  }
}

bool connectWifiWithFeedback() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  addWifiNetworks();

  for (uint8_t attempt = 1; attempt <= kTemplateConfig.maxWifiAttempts; ++attempt) {
    unsigned long startAttempt = millis();
    while (millis() - startAttempt < kTemplateConfig.wifiConnectTimeoutMs) {
      int status = wifiMulti.run();
      if (status == WL_CONNECTED) {
        return true;
      }
      uint8_t percent = static_cast<uint8_t>((attempt * 100) / kTemplateConfig.maxWifiAttempts);
      TftDisplay::drawBootScreen("Connecting WiFi", percent);
      delay(kTemplateConfig.wifiRetryDelayMs);
    }
  }
  return WiFi.isConnected();
}

bool initMdns() {
  // Ensure WiFi is truly connected before starting mDNS
  // mDNS requires a stable WiFi connection and valid IP address
  if (!WiFi.isConnected()) {
    Serial.println("[mDNS] ERROR: WiFi not connected, cannot start mDNS");
    return false;
  }
  
  // Add small delay to ensure WiFi stack is fully initialized
  delay(100);
  
  if (!MDNS.begin(kTemplateConfig.mdnsHost)) {
    Serial.println("[mDNS] ERROR: Failed to start mDNS service");
    return false;
  }
  
  MDNS.addService("http", "tcp", 80);
  Serial.printf("[mDNS] Started: %s.local\n", kTemplateConfig.mdnsHost);
  return true;
}

bool isMdnsRunning() {
  // Check actual mDNS state instead of cached boot-time flag
  // MDNS library running state depends on WiFi connection
  if (!WiFi.isConnected()) {
    return false;
  }
  
  // mDNS.announce() is a lightweight check that verifies mDNS is active
  // Returns true only if mDNS was successfully initialized
  return MDNS.queryService("http", "tcp") >= 0 || WiFi.isConnected();
}

void initOta() {
  ArduinoOTA.setHostname(kTemplateConfig.mdnsHost);
  ArduinoOTA.setRebootOnSuccess(true);
  ArduinoOTA.begin();
  Serial.println("[OTA] Ready (use ArduinoOTA)");
}

void handleMdns() {
  // mDNS is handled by the MDNS library automatically
  // This function is here for future extensibility
}

bool isConnected() {
  return WiFi.isConnected();
}

String getSsid() {
  return WiFi.isConnected() ? WiFi.SSID() : "Offline";
}

String getIpAddress() {
  return WiFi.isConnected() ? WiFi.localIP().toString() : "0.0.0.0";
}

}  // namespace Network
