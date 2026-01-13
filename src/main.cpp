#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_NeoPixel.h>
#include <esp32-hal-psram.h>

#include "board_config.h"
#include "config.h"
#include "secrets.h"

namespace {

WiFiMulti wifiMulti;
WebServer server(80);

// ST7789 1.54" - constructeur simple (CS, DC, RST)
// Les broches SPI sont gérées via SPI.begin()
Adafruit_ST7789 tft(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN);
#if defined(ENV_ESP32S3_N16R8)
Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
#endif

struct UiState {
  bool wifiConnected{false};
  String currentSsid;
  IPAddress currentIp;
  bool mdnsOk{false};
  unsigned long lastUiRefresh{0};
  bool rebootPrompt{false};
  unsigned long rebootHoldStart{0};
};

UiState ui;
constexpr uint8_t kBootBarMargin = 20;
constexpr uint8_t kLedcChannel = 5;
constexpr uint8_t kLedcResolution = 8;
constexpr uint16_t kTextMarginPx = 10;  // left/right padding for LCD text

uint16_t printWrap(const String& text, uint8_t textSize, uint16_t x, uint16_t yStart, uint16_t maxWidthPx, uint8_t maxLines = 2) {
  const uint16_t charWidth = 6 * textSize;
  const uint16_t charHeight = 8 * textSize;
  const uint16_t maxChars = maxWidthPx / charWidth;
  if (maxChars == 0 || maxLines == 0) return yStart;

  String remaining = text;
  uint16_t y = yStart;
  for (uint8_t line = 0; line < maxLines && remaining.length() > 0; ++line) {
    String slice;
    if (remaining.length() <= maxChars) {
      slice = remaining;
      remaining = "";
    } else {
      slice = remaining.substring(0, maxChars);
      remaining = remaining.substring(maxChars);
      // If this is the last allowed line and there is still text, add ellipsis
      if (line + 1 == maxLines) {
        if (slice.length() >= 3) {
          slice = slice.substring(0, slice.length() - 3) + "...";
        }
        remaining = "";
      }
    }
    tft.setCursor(x, y);
    tft.print(slice);
    y += charHeight;
  }
  return y;
}

// Truncate text so it fits within the given pixel width at the chosen text size.
String fitText(const String& text, uint8_t textSize, uint16_t maxWidthPx) {
  const uint16_t charWidth = 6 * textSize;  // default Adafruit_GFX font width
  const uint16_t maxChars = maxWidthPx / charWidth;
  if (maxChars == 0) return "";
  if (text.length() <= maxChars) return text;
  if (maxChars <= 3) return text.substring(0, maxChars);  // not enough room for ellipsis
  return text.substring(0, maxChars - 3) + "...";  // ASCII ellipsis to avoid unsupported glyphs
}

void printFit(uint16_t x, uint16_t y, const String& text, uint8_t textSize, uint16_t maxWidthPx) {
  tft.setTextSize(textSize);
  tft.setCursor(x, y);
  tft.print(fitText(text, textSize, maxWidthPx));
}

void initBacklight() {
  if (DISPLAY_BL_PIN == 255) {
    return;
  }
  ledcSetup(kLedcChannel, 5000, kLedcResolution);
  ledcAttachPin(DISPLAY_BL_PIN, kLedcChannel);
  ledcWrite(kLedcChannel, kTemplateConfig.backlightLevel);
}

void initPsram() {
  #if defined(ENV_ESP32S3_N16R8)
  if (!psramInit()) {
    Serial.println("[PSRAM] Init failed or PSRAM not detected.");
    return;
  }
  const size_t total = ESP.getPsramSize();
  const size_t free = ESP.getFreePsram();
  Serial.printf("[PSRAM] Enabled: %u / %u bytes free.\n", static_cast<unsigned>(free), static_cast<unsigned>(total));
  #endif
}

void drawBootScreen(const char* message, uint8_t percent) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  const uint16_t lineWidth = DISPLAY_WIDTH - (2 * kTextMarginPx);
  printFit(kTextMarginPx, 20, projectName(), 2, lineWidth);
  tft.setTextSize(2);
  tft.setCursor(kTextMarginPx, 50);
  tft.print("v");
  tft.print(projectVersion());
  printFit(kTextMarginPx, 90, message, 2, lineWidth);
  if (kTemplateConfig.enableBootBar) {
    uint16_t barWidth = DISPLAY_WIDTH - (2 * kBootBarMargin);
    uint16_t filled = (percent * barWidth) / 100;
    uint16_t y = DISPLAY_HEIGHT - 60;
    tft.drawRect(kBootBarMargin, y, barWidth, 20, ST77XX_WHITE);
    tft.fillRect(kBootBarMargin + 2, y + 2, filled > 4 ? filled - 4 : 0, 16, ST77XX_GREEN);
  }
}

void updateMainScreen(bool force = false) {
  const bool wifiNow = WiFi.isConnected();
  const String ssidNow = wifiNow ? WiFi.SSID() : "Offline";
  const IPAddress ipNow = wifiNow ? WiFi.localIP() : IPAddress();
  if (!force && wifiNow == ui.wifiConnected && ssidNow == ui.currentSsid && ipNow == ui.currentIp) {
    return;
  }
  ui.wifiConnected = wifiNow;
  ui.currentSsid = ssidNow;
  ui.currentIp = ipNow;

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  const uint16_t lineWidth = DISPLAY_WIDTH - (2 * kTextMarginPx);

  // Header
  printFit(kTextMarginPx, 20, projectName(), 2, lineWidth);
  tft.setTextSize(2);
  tft.setCursor(kTextMarginPx, 40);
  tft.print("v");
  tft.print(projectVersion());

  // Details (smaller font, wrapped)
  uint16_t y = 80;
  tft.setTextSize(1);
  tft.setCursor(kTextMarginPx, y);
  tft.print("SSID:");
  y += 12;
  y = printWrap(ssidNow, 1, kTextMarginPx, y, lineWidth);
  y += 4;

  tft.setCursor(kTextMarginPx, y);
  tft.print("IP:");
  y += 12;
  y = printWrap(wifiNow ? ipNow.toString() : String("0.0.0.0"), 1, kTextMarginPx, y, lineWidth);
  y += 4;

  tft.setCursor(kTextMarginPx, y);
  tft.print("mDNS:");
  y += 12;
  y = printWrap(ui.mdnsOk ? String(kTemplateConfig.mdnsHost) : String("disabled"), 1, kTextMarginPx, y, lineWidth);
}

void drawRebootPrompt(uint8_t percent) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("Reboot ?");
  uint16_t barWidth = DISPLAY_WIDTH - (2 * kBootBarMargin);
  uint16_t filled = (percent * barWidth) / 100;
  uint16_t y = DISPLAY_HEIGHT / 2;
  tft.drawRect(kBootBarMargin, y, barWidth, 20, ST77XX_WHITE);
  tft.fillRect(kBootBarMargin + 2, y + 2, filled > 4 ? filled - 4 : 0, 16, ST77XX_ORANGE);
}

void animateNeoPixel(uint32_t color) {
  #if defined(ENV_ESP32S3_N16R8)
  pixel.setBrightness(NEOPIXEL_BRIGHTNESS);
  for (uint16_t i = 0; i < NEOPIXEL_COUNT; ++i) {
    pixel.setPixelColor(i, color);
  }
  pixel.show();
  #endif
}

void addWifiNetworks() {
  constexpr size_t wifiCount = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
  for (size_t i = 0; i < wifiCount; ++i) {
    wifiMulti.addAP(WIFI_NETWORKS[i][0], WIFI_NETWORKS[i][1]);
  }
}

String buildOtaPage() {
  String html;
  html.reserve(2600);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>OTA Update - ";
  html += projectName();
  html += "</title>";
  html += "<style>body{font-family:Inter,Arial,sans-serif;background:#0b1220;color:#e2e8f0;margin:0;padding:24px;}";
  html += ".card{max-width:520px;margin:0 auto;background:#111827;border:1px solid #1f2937;border-radius:12px;padding:20px;box-shadow:0 12px 30px rgba(0,0,0,0.35);}";
  html += "button,input[type=submit]{background:#2563eb;color:white;border:none;border-radius:10px;padding:12px 16px;font-size:15px;cursor:pointer;}";
  html += "button:hover,input[type=submit]:hover{background:#1d4ed8;}";
  html += ".progress{width:100%;background:#1f2937;border-radius:10px;overflow:hidden;margin:12px 0;height:14px;}";
  html += ".bar{width:0;height:100%;background:#10b981;transition:width .15s ease;}";
  html += "#status{font-size:14px;margin-top:8px;color:#cbd5e1;}";
  html += "</style></head><body><div class=\"card\">";
  html += "<h2>OTA Update</h2><p>Select firmware (.bin) then click Upload.</p>";
  html += "<form id=\"otaForm\" method=\"POST\" action=\"/update\" enctype=\"multipart/form-data\">";
  html += "<input type=\"file\" name=\"firmware\" accept=\".bin\" required><br><br>";
  html += "<input type=\"submit\" value=\"Upload\">";
  html += "<div class=\"progress\"><div class=\"bar\" id=\"bar\"></div></div>";
  html += "<div id=\"status\">Idle</div>";
  html += "</form>";
  html += "<p><a href=\"/\"><button type=\"button\">Back to dashboard</button></a></p>";
  html += "</div><script>const f=document.getElementById('otaForm');const bar=document.getElementById('bar');const st=document.getElementById('status');f.addEventListener('submit',e=>{e.preventDefault();const fd=new FormData(f);const xhr=new XMLHttpRequest();xhr.open('POST','/update');xhr.upload.onprogress=e=>{if(e.lengthComputable){const p=Math.round((e.loaded/e.total)*100);bar.style.width=p+'%';st.textContent='Uploading '+p+'%';}};xhr.onload=()=>{if(xhr.status==200){bar.style.width='100%';st.textContent='Upload OK, rebooting...';setTimeout(()=>location.href='/',4000);}else{st.textContent='Error '+xhr.status;}};xhr.onerror=()=>st.textContent='Upload failed';xhr.send(fd);});</script></body></html>";
  return html;
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
      drawBootScreen("Connecting WiFi", percent);
      delay(kTemplateConfig.wifiRetryDelayMs);
    }
  }
  return WiFi.isConnected();
}

String buildStatusJson() {
  const bool connected = WiFi.isConnected();
  const size_t psramTotal = ESP.getPsramSize();
  const size_t psramFree = ESP.getFreePsram();
  String json = "{";
  json += "\"project\":\"" + String(projectName()) + "\",";
  json += "\"version\":\"" + String(projectVersion()) + "\",";
  json += "\"chip\":\"" + String(ESP.getChipModel()) + "\",";
  json += "\"cpu_mhz\":" + String(getCpuFrequencyMhz()) + ",";
  json += "\"heap_free\":" + String(ESP.getFreeHeap()) + ",";
  json += "\"heap_total\":" + String(ESP.getHeapSize()) + ",";
  json += "\"psram_free\":" + String(psramFree) + ",";
  json += "\"psram_total\":" + String(psramTotal) + ",";
  json += "\"ssid\":\"" + (connected ? String(WiFi.SSID()) : String("")) + "\",";
  json += "\"ip\":\"" + (connected ? WiFi.localIP().toString() : String("")) + "\"";
  json += "}";
  return json;
}

String buildRootPage() {
  const bool connected = WiFi.isConnected();
  const String ip = connected ? WiFi.localIP().toString() : "Not connected";
  const String ssid = connected ? WiFi.SSID() : "Not connected";
  String html;
  html.reserve(2048);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>" + String(projectName()) + "</title>";
  html += "<style>body{font-family:Inter,Arial,sans-serif;background:linear-gradient(135deg,#0f172a,#1e293b);color:#e2e8f0;margin:0;padding:24px;}";
  html += ".shell{max-width:960px;margin:0 auto;}h1{margin:0 0 12px;}";
  html += ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:12px;}";
  html += ".card{background:#0b1220;border:1px solid #243148;border-radius:12px;padding:16px;box-shadow:0 10px 30px rgba(0,0,0,0.35);}";
  html += ".badge{display:inline-block;padding:4px 8px;border-radius:999px;background:#10b981;color:#042f2e;font-weight:700;font-size:12px;}";
  html += "</style></head><body><div class=\"shell\">";
  html += "<h1>" + String(projectName()) + " <span class=\"badge\">v" + String(projectVersion()) + "</span></h1>";
  html += "<div class=\"grid\">";
  html += "<div class=\"card\"><strong>Processor</strong><div>" + String(ESP.getChipModel()) + "</div></div>";
  html += "<div class=\"card\"><strong>CPU</strong><div>" + String(getCpuFrequencyMhz()) + " MHz</div></div>";
  html += "<div class=\"card\"><strong>Heap</strong><div>" + String(ESP.getFreeHeap()) + " / " + String(ESP.getHeapSize()) + " bytes</div></div>";
  html += "<div class=\"card\"><strong>PSRAM</strong><div>" + String(ESP.getFreePsram()) + " / " + String(ESP.getPsramSize()) + " bytes</div></div>";
  html += "<div class=\"card\"><strong>SSID</strong><div>" + ssid + "</div></div>";
  html += "<div class=\"card\"><strong>IP</strong><div>" + ip + "</div></div>";
  html += "<div class=\"card\"><strong>OTA</strong><div><a href=\"/update\"><button>Open OTA Page</button></a></div></div>";
  html += "</div></div></body></html>";
  return html;
}

void setupWebServer() {
  if (!kTemplateConfig.enableWebUi) {
    return;
  }
  server.on("/", []() { server.send(200, "text/html", buildRootPage()); });
  server.on("/update", HTTP_GET, []() { server.send(200, "text/html", buildOtaPage()); });
  server.on(
    "/update", HTTP_POST,
    []() {
      const bool ok = !Update.hasError();
      server.send(ok ? 200 : 500, "text/plain", ok ? "OK" : "FAIL");
      if (ok) {
        delay(200);
        ESP.restart();
      }
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("[OTA] Update start: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("[OTA] Update Success: %u bytes\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
  server.on("/status", []() { server.send(200, "application/json", buildStatusJson()); });
  server.begin();
}

void handleButtons() {
  const bool bootPressed = digitalRead(BUTTON_BOOT_PIN) == LOW;
  if (bootPressed && !ui.rebootPrompt) {
    ui.rebootPrompt = true;
    ui.rebootHoldStart = millis();
    drawRebootPrompt(0);
  }

  if (ui.rebootPrompt) {
    const uint32_t elapsed = millis() - ui.rebootHoldStart;
    const uint32_t requiredMs = static_cast<uint32_t>(kTemplateConfig.rebootHoldSeconds) * 1000UL;
    uint8_t percent = elapsed >= requiredMs ? 100 : static_cast<uint8_t>((elapsed * 100) / requiredMs);
    drawRebootPrompt(percent);
    if (!bootPressed) {
      ui.rebootPrompt = false;
      updateMainScreen(true);
      return;
    }
    if (percent >= 100) {
      #if defined(ENV_ESP32S3_N16R8)
      animateNeoPixel(pixel.Color(0, 0, 255));
      #endif
      ESP.restart();
    }
  }
}

void setupDisplay() {
  Serial.println("[DISPLAY] Starting SPI with low frequency...");
  // Utiliser une fréquence basse au départ (20 MHz au lieu de 27 MHz)
  SPI.begin(DISPLAY_SCK_PIN, DISPLAY_MISO_PIN, DISPLAY_MOSI_PIN);
  SPI.setFrequency(20000000);  // 20 MHz
  
  Serial.println("[DISPLAY] Initializing ST7789 1.54\"...");
  delay(100);
  tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  delay(100);
  
  Serial.println("[DISPLAY] Setting rotation...");
  tft.setRotation(kTemplateConfig.lcdRotation);
  delay(100);
  
  Serial.println("[DISPLAY] Initializing backlight...");
  initBacklight();
  
  // Test simple avec délai
  Serial.println("[DISPLAY] Test: filling RED...");
  tft.fillScreen(ST77XX_RED);
  delay(1500);
  
  Serial.println("[DISPLAY] Drawing boot screen...");
  drawBootScreen("Boot", 5);
  Serial.println("[DISPLAY] Setup complete!");
}

void setupNeoPixel() {
  #if defined(ENV_ESP32S3_N16R8)
  pixel.begin();
  pixel.clear();
  pixel.setBrightness(NEOPIXEL_BRIGHTNESS);
  animateNeoPixel(pixel.Color(0, 10, 50));
  #endif
}

void setupMdns() {
  if (!MDNS.begin(kTemplateConfig.mdnsHost)) {
    ui.mdnsOk = false;
    return;
  }
  MDNS.addService("http", "tcp", 80);
  ui.mdnsOk = true;
}

void setupOta() {
  ArduinoOTA.setHostname(kTemplateConfig.mdnsHost);
  ArduinoOTA.setRebootOnSuccess(true);
  ArduinoOTA.begin();
  Serial.println("[OTA] Ready (use ArduinoOTA)");
}

void setupButtons() {
  pinMode(BUTTON_BOOT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_USER1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_USER2_PIN, INPUT_PULLUP);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  initPsram();
  setupButtons();
  setupDisplay();
  setupNeoPixel();

  drawBootScreen("WiFi", 15);
  const bool wifiOk = connectWifiWithFeedback();
  ui.wifiConnected = wifiOk;
  if (wifiOk) {
    drawBootScreen("WiFi OK", 70);
    #if defined(ENV_ESP32S3_N16R8)
    animateNeoPixel(pixel.Color(0, 80, 0));
    #endif
  } else {
    drawBootScreen("WiFi fail", 70);
    #if defined(ENV_ESP32S3_N16R8)
    animateNeoPixel(pixel.Color(80, 0, 0));
    #endif
  }

  setupMdns();
  setupOta();
  drawBootScreen("Starting web", 85);
  setupWebServer();

  drawBootScreen("Ready", 100);
  updateMainScreen(true);
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleButtons();

  const unsigned long now = millis();
  if (now - ui.lastUiRefresh > 1000) {
    ui.lastUiRefresh = now;
    updateMainScreen();
    #if defined(ENV_ESP32S3_N16R8)
    const uint32_t color = WiFi.isConnected() ? pixel.Color(0, 30, 0) : pixel.Color(30, 0, 0);
    animateNeoPixel(color);
    #endif
  }
}