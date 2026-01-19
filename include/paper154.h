#pragma once
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Arduino.h>

namespace Paper154 {
    // Initialisation de l'écran e-paper 1.54" 200x200 (ex: GDEH0154D67)
    void begin();
    void setRotation(uint8_t rot);
    void clear();
    void printCenter(const char* text);
    void print(const char* text, int16_t x, int16_t y);
    void update();

    // Affichage structuré et esthétique des infos système
    void drawStatusScreen(const char* project, const char* ssid, const char* ip);
}
