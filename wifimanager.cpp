#include "wifimanager.h"

void setupWiFiManager() {
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect(APSSID, APKEY);
}

void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("No WiFi found");
  Serial.print("AP:");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.print("Key:");
  Serial.println(APKEY);
  Serial.print("IP:");
  Serial.println(WiFi.softAPIP());
}