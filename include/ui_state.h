#pragma once

#include <Arduino.h>
#include <IPAddress.h>

namespace UiState {

/**
 * Structure containing all UI state information.
 */
struct State {
  bool wifiConnected = false;
  String currentSsid;
  IPAddress currentIp;
  bool mdnsOk = false;
  unsigned long lastUiRefresh = 0;
  bool rebootPrompt = false;
  unsigned long rebootHoldStart = 0;
};

/**
 * Get the global UI state instance.
 * @return Reference to global UI state
 */
State& getState();

/**
 * Reset UI state to initial values.
 */
void reset();

}  // namespace UiState
