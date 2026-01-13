#include "web_server.h"
#include "web_ui.h"
#include "config.h"
#include <Arduino.h>
#include <Update.h>

namespace HttpServer {

static ::WebServer server(80);

void init() {
  if (!kTemplateConfig.enableWebUi) {
    return;
  }

  server.on("/", []() {
    server.send(200, "text/html", WebUI::buildRootPage());
  });

  server.on("/update", HTTP_GET, []() {
    server.send(200, "text/html", WebUI::buildOtaPage());
  });

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

  server.on("/status", []() {
    server.send(200, "application/json", WebUI::buildStatusJson());
  });

  server.begin();
  Serial.println("[WebServer] Started on port 80");
}

void handleClient() {
  if (kTemplateConfig.enableWebUi) {
    server.handleClient();
  }
}

}  // namespace HttpServer
