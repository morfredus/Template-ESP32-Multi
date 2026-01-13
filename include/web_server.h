#pragma once

#include <WebServer.h>

namespace HttpServer {

/**
 * Initialize and setup the web server with all routes.
 * Configures endpoints for dashboard, OTA, and status API.
 * Only initializes if kTemplateConfig.enableWebUi is true.
 */
void init();

/**
 * Handle incoming HTTP requests.
 * Should be called regularly in the main loop.
 */
void handleClient();

}  // namespace HttpServer
