// Définition unique des méthodes et de l'objet display
#include "paper154.h"
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include "board_config.h"
#include "secrets_wrapper.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

namespace Paper154 {
    void drawStatusScreen(const char* project, const char* ssid, const char* ip) {
        clear();
        Serial.println("[PAPER154] drawStatusScreen exécuté");
        display.setFont(&FreeMonoBold9pt7b);
        // Cadre principal
        display.drawRoundRect(2, 2, 196, 196, 8, GxEPD_BLACK);
        // Titre centré
        int16_t x1, y1; uint16_t w, h;
        display.getTextBounds(project ? project : "-", 0, 0, &x1, &y1, &w, &h);
        display.setCursor((200-w)/2, 28);
        display.print(project ? project : "-");
        // Ligne de séparation
        display.drawLine(10, 36, 190, 36, GxEPD_BLACK);
        // SSID
        display.setCursor(14, 60);
        display.print("SSID : ");
        display.print(ssid ? ssid : "-");
        // IP
        display.setCursor(14, 90);
        display.print("IP   : ");
        display.print(ip ? ip : "-");
        // Cadre info
        display.drawRect(8, 45, 184, 55, GxEPD_BLACK);
        // Décor bas
        display.drawLine(10, 170, 190, 170, GxEPD_BLACK);
        display.setFont();
        display.setCursor(20, 190);
        display.print("E-Paper 1.54'' 200x200");
        display.display(true); // Rafraîchissement complet
    }

    void begin() {
        display.init();
        display.setRotation(1); // Paysage
        display.setTextColor(GxEPD_BLACK);
        clear();
    }

    void setRotation(uint8_t rot) {
        display.setRotation(rot);
    }

    void clear() {
        display.fillScreen(GxEPD_WHITE);
        display.display(true); // Rafraîchissement complet
    }

    void printCenter(const char* text) {
        display.setFont(&FreeMonoBold9pt7b);
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
        int16_t x = (200 - w) / 2;
        int16_t y = (200 + h) / 2;
        display.setCursor(x, y);
        display.print(text);
        update();
    }

    void print(const char* text, int16_t x, int16_t y) {
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(x, y);
        display.print(text);
        update();
    }

    void update() {
        display.display(false); // partiel
    }
}
