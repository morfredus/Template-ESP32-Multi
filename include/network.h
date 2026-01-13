#pragma once

#include <Arduino.h>

namespace Network {

/**
 * Initialize WiFi multi-network connection with feedback.
 * Attempts to connect to configured networks with timeout and retry logic.
 * Shows boot screen progress during connection attempts.
 * @return true if connected to WiFi, false otherwise
 */
bool connectWifiWithFeedback();

/**
 * Initialize mDNS service with configured hostname.
 * @return true if mDNS initialized successfully
 */
bool initMdns();

/**
 * Check if mDNS service is currently running.
 * Queries actual mDNS state instead of relying on boot-time flag.
 * @return true if mDNS is active and responding
 */
bool isMdnsRunning();

/**
 * Initialize ArduinoOTA service for over-the-air updates.
 * Sets hostname and enables OTA updates.
 */
void initOta();

/**
 * Handle WiFi multicast DNS resolution.
 * Should be called periodically in the main loop.
 */
void handleMdns();

/**
 * Get current WiFi connection status.
 * @return true if WiFi is connected, false otherwise
 */
bool isConnected();

/**
 * Get current WiFi SSID.
 * @return SSID string or "Offline" if not connected
 */
String getSsid();

/**
 * Get current WiFi IP address.
 * @return IP address as string
 */
String getIpAddress();

}  // namespace Network
