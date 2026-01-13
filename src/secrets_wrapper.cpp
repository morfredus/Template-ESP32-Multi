// ============================================================================
// SECRETS WRAPPER
// ============================================================================
// Ce fichier est le SEUL qui inclut secrets.h pour éviter les conflits
// de définitions multiples lors du linking.
// Les autres fichiers doivent utiliser les fonctions ci-dessous.
// ============================================================================

#include <cstddef>
#include "secrets.h"

// Expose les données WiFi via des fonctions
const char* getWifiSsid(size_t index) {
  constexpr size_t wifiCount = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
  if (index >= wifiCount) return nullptr;
  return WIFI_NETWORKS[index][0];
}

const char* getWifiPassword(size_t index) {
  constexpr size_t wifiCount = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
  if (index >= wifiCount) return nullptr;
  return WIFI_NETWORKS[index][1];
}

size_t getWifiNetworksCount() {
  return sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);
}
