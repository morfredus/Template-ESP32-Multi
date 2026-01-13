#pragma once

#include <Arduino.h>

namespace WebUI {

/**
 * Build the main dashboard page HTML.
 * Displays system info, WiFi status, and links to OTA page.
 * @return HTML string for dashboard page
 */
String buildRootPage();

/**
 * Build the OTA update page HTML.
 * Allows file upload for firmware update with progress tracking.
 * @return HTML string for OTA page
 */
String buildOtaPage();

/**
 * Build the system status JSON response.
 * Contains project info, hardware details, and network status.
 * @return JSON string with system status
 */
String buildStatusJson();

}  // namespace WebUI
