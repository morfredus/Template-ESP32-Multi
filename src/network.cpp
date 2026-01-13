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
  if (!MDNS.begin(kTemplateConfig.mdnsHost)) {
    return false;
  }
  MDNS.addService("http", "tcp", 80);
  return true;
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
