#pragma once

namespace Buttons {

/**
 * Initialize all button pins as inputs with pull-up configuration.
 */
void init();

/**
 * Handle button input and reboot logic.
 * Manages boot button hold detection and reboot confirmation screen.
 * Should be called regularly in the main loop.
 */
void handle();

}  // namespace Buttons
