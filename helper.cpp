#include "helper.h"

String updateTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "00:00:00";
  }
  char buffer[40];
  sprintf(buffer, "%2d.%2d.%2d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buffer);
}

String updateDate() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "00.00.0000";
  }
  char buffer[40];
  sprintf(buffer, "%2d.%2d.%4d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  return String(buffer);
}

void setupPins()
{
  pinMode(PUMP1_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN, LOW);

  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP2_PIN, LOW);

  pinMode(VALVE1_PIN, OUTPUT);
  digitalWrite(VALVE1_PIN, LOW);

  pinMode(VALVE2_PIN, OUTPUT);
  digitalWrite(VALVE2_PIN, LOW);

  pinMode(FAN1_PIN, OUTPUT);
  digitalWrite(FAN1_PIN, LOW);

  pinMode(FAN2_PIN, OUTPUT);
  digitalWrite(FAN2_PIN, LOW);

  pinMode(RF_PIN, OUTPUT);
  digitalWrite(RF_PIN, LOW);

  pinMode(FLOW_PIN, INPUT_PULLUP);
  pinMode(LEVEL_PIN, INPUT);
  pinMode(DHT_PIN, INPUT);
  pinMode(FANPWM1_PIN, OUTPUT);
}