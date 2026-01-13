#pragma once

namespace Psram {

/**
 * Initialize PSRAM if available on the board.
 * Logs PSRAM status and free memory to Serial.
 * Only functional on boards with PSRAM support (e.g., ESP32S3).
 */
void init();

}  // namespace Psram
