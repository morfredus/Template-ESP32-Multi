void loop() {
  // Boucle principale vide ou à compléter selon besoins
}
#include <Arduino.h>
#include <ArduinoOTA.h>

#include "board_config.h"
#include "config.h"

// Modular components
#include "psram.h"
#include "tft_display.h"
#include "buttons.h"
#include "neopixel.h"
#include "network.h"
#include "web_server.h"
#include "ui_state.h"
#include "lcd1602.h"

#include "paper154.h"
#include "secrets_wrapper.h"

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

  // Initialisation de l'écran e-paper 1.54'' (Paper154)
  Paper154::begin();
  // Rotation à gauche (quart de tour)
  Paper154::setRotation(0); // 0 = 90° à gauche pour GxEPD2
  Paper154::clear();
  delay(500); // Laisse le temps à l'e-paper de s'initialiser et clear
  Paper154::print("WAIT...", 20, 100); // Affichage test
  delay(1500); // Laisse le temps de voir le message test

  // Initialisation de l'écran LCD 1602 I2C
  Lcd1602::begin(0x27, 16, 2);
  Lcd1602::scrollText("WiFi...", 1, 100);
  // Rien sur e-paper ici, on attend la connexion WiFi pour afficher les infos structurées

  TftDisplay::drawBootScreen("WiFi", 15);
  const bool wifiOk = Network::connectWifiWithFeedback();
  g_uiState.wifiConnected = wifiOk;
  // Affichage structuré sur e-paper après WiFi
  Serial.print("[DEBUG] wifiOk = ");
  Serial.println(wifiOk ? "true" : "false");
  delay(1000); // Laisse le temps à l'e-paper de finir son cycle précédent
  Serial.println("[DEBUG] Avant clear+drawStatusScreen");
  Paper154::clear();
  delay(500); // Laisse le temps au clear de s'afficher
  if (wifiOk) {
    Serial.println("[DEBUG] Appel drawStatusScreen (OK)");
    Serial.print("[DEBUG] projectName: "); Serial.println(projectName());
    Serial.print("[DEBUG] SSID: "); Serial.println(getWifiSsid(0));
    Serial.print("[DEBUG] IP: "); Serial.println(WiFi.localIP());
    Paper154::drawStatusScreen(
      projectName(),
      getWifiSsid(0),
      WiFi.localIP().toString().c_str()
    );
    Serial.println("[DEBUG] Après drawStatusScreen (OK)");
    Paper154::print("TEST", 20, 140); // Affichage test après drawStatusScreen
    Paper154::print("AFTER", 20, 160); // Affichage test supplémentaire
  } else {
    Serial.println("[DEBUG] Appel drawStatusScreen (NO WIFI)");
    Paper154::drawStatusScreen(projectName(), "-", "NO WIFI");
    Serial.println("[DEBUG] Après drawStatusScreen (NO WIFI)");
    Paper154::print("TEST", 20, 140); // Affichage test après drawStatusScreen
    Paper154::print("AFTER", 20, 160); // Affichage test supplémentaire
  }
  delay(1500); // Laisse le temps à l'e-paper de rafraîchir

  if (wifiOk) {
    Lcd1602::print(projectName(), 0, 0);
    Lcd1602::scrollText("WiFi OK", 1, 100);
    TftDisplay::drawBootScreen("WiFi OK", 70);
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setColor(NeoPixel::makeColor(0, 80, 0));  // Green
    #endif

    Lcd1602::print(projectName(), 0, 0);
    Lcd1602::scrollText("mDNS...", 1, 100);
    TftDisplay::drawBootScreen("Starting mDNS", 75);
    g_uiState.mdnsOk = Network::initMdns();

    Lcd1602::print(projectName(), 0, 0);
    Lcd1602::scrollText("OTA...", 1, 100);
    TftDisplay::drawBootScreen("Starting OTA", 80);
    Network::initOta();

    Lcd1602::print(projectName(), 0, 0);
    Lcd1602::scrollText("Web...", 1, 100);
    TftDisplay::drawBootScreen("Starting web", 85);
    HttpServer::init();

    // Affiche le nom du projet et l'adresse IP sur le LCD
    Lcd1602::print(projectName(), 0, 0);
    Lcd1602::scrollText(WiFi.localIP().toString().c_str(), 1, 100);
  } else {
    Lcd1602::print(projectName(), 0, 0);
    Lcd1602::scrollText("WiFi FAIL", 1, 100);
    TftDisplay::drawBootScreen("WiFi fail", 70);
    #if defined(ENV_ESP32S3_N16R8)
    NeoPixel::setColor(NeoPixel::makeColor(80, 0, 0));  // Red
    #endif

    g_uiState.mdnsOk = false;
    Serial.println("[MAIN] Network services disabled (WiFi connection failed)");
  }
// ...fin du else précédent...
}