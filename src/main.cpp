#include <Arduino.h>
#include <ArduinoOTA.h>

#include "board_config.h"
#include "config.h"
#include "secrets.h"

// Modular components
#include "psram.h"
#include "tft_display.h"
#include "buttons.h"
#include "neopixel.h"
#include "network.h"
#include "web_server.h"
#include "ui_state.h"

// Global UI state (declared in ui_state.cpp)
extern UiState::State g_uiState;



void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n[MAIN] Starting...");
  
  // Initialize basic components
  Psram::init();
  Buttons::init();
  TftDisplay::setupDisplay();
  NeoPixel::init();

  // Show WiFi connection progress
  TftDisplay::drawBootScreen("WiFi", 15);
  const bool wifiOk = Network::connectWifiWithFeedback();
  g_uiState.wifiConnected = wifiOk;
  
  // Initialize network services based on WiFi status
  if (wifiOk) {
    TftDisplay::drawBootScreen("WiFi OK", 70);
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setColor(NeoPixel::makeColor(0, 80, 0));  // Green
    #endif

    TftDisplay::drawBootScreen("Starting mDNS", 75);
    g_uiState.mdnsOk = Network::initMdns();
    
    TftDisplay::drawBootScreen("Starting OTA", 80);
    Network::initOta();
    
    TftDisplay::drawBootScreen("Starting web", 85);
    HttpServer::init();
  } else {
    TftDisplay::drawBootScreen("WiFi fail", 70);
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setColor(NeoPixel::makeColor(80, 0, 0));  // Red
    #endif
    
    g_uiState.mdnsOk = false;
    Serial.println("[MAIN] Network services disabled (WiFi connection failed)");
  }

  // Show ready screen
  TftDisplay::drawBootScreen("Ready", 100);
  delay(500);
  TftDisplay::updateMainScreen(true);
  
  Serial.println("[MAIN] Setup complete!");
}

void loop() {
  // Handle network and web requests
  HttpServer::handleClient();
  ArduinoOTA.handle();
  
  // Handle user input
  Buttons::handle();

  // Update UI periodically
  const unsigned long now = millis();
  if (now - g_uiState.lastUiRefresh > 1000) {
    g_uiState.lastUiRefresh = now;
    TftDisplay::updateMainScreen();
    
    // Update LED status
    #if defined(ENV_ESP32S3_N16R8)
    const uint32_t color = Network::isConnected() 
      ? NeoPixel::makeColor(0, 30, 0)    // Green
      : NeoPixel::makeColor(30, 0, 0);   // Red
    NeoPixel::setColor(color);
    #endif
  }
}