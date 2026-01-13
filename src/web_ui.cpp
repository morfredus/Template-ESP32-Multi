#include "web_ui.h"
#include "web_styles.h"
#include "config.h"
#include <WiFi.h>
#include <esp_system.h>

namespace WebUI {

String buildRootPage() {
  const bool connected = WiFi.isConnected();
  const String ip = connected ? WiFi.localIP().toString() : "Not connected";
  const String ssid = connected ? WiFi.SSID() : "Not connected";
  
  String html;
  html.reserve(2048);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>" + String(projectName()) + "</title>";
  html += "<style>";
  html += WebStyles::getDashboardStyles();
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

String buildOtaPage() {
  String html;
  html.reserve(2600);
  html += "<!doctype html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>OTA Update - ";
  html += projectName();
  html += "</title>";
  html += "<style>";
  html += WebStyles::getOtaStyles();
  html += "</style></head><body><div class=\"card\">";
  html += "<h2>OTA Update</h2><p>Select firmware (.bin) then click Upload.</p>";
  html += "<form id=\"otaForm\" method=\"POST\" action=\"/update\" enctype=\"multipart/form-data\">";
  html += "<input type=\"file\" name=\"firmware\" accept=\".bin\" required><br><br>";
  html += "<input type=\"submit\" value=\"Upload\">";
  html += "<div class=\"progress\"><div class=\"bar\" id=\"bar\"></div></div>";
  html += "<div id=\"status\">Idle</div>";
  html += "</form>";
  html += "<p><a href=\"/\"><button type=\"button\">Back to dashboard</button></a></p>";
  html += "</div>";
  html += "<script>";
  html += WebStyles::getOtaScript();
  html += "</script></body></html>";
  return html;
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

}  // namespace WebUI
