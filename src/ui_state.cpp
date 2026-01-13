#include "ui_state.h"

namespace UiState {

static State g_state;

State& getState() {
  return g_state;
}

void reset() {
  g_state = State();
}

}  // namespace UiState

// Make global accessible from other modules
UiState::State g_uiState;
