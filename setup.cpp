#include "setup.h"

void setupPins()
{
  // initialize pump pins
  pinMode(PUMP1_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN, LOW);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP2_PIN, LOW);

  // initialize valve pins
  pinMode(VALVE1_PIN, OUTPUT);
  digitalWrite(VALVE1_PIN, LOW);
  pinMode(VALVE2_PIN, OUTPUT);
  digitalWrite(VALVE2_PIN, LOW);

  // initialize flow pin
  pinMode(FLOW_PIN, INPUT_PULLUP);

  // initialize fan pins
  pinMode(FAN1_PIN, OUTPUT);
  digitalWrite(FAN1_PIN, LOW);
  pinMode(FAN2_PIN, OUTPUT);
  digitalWrite(FAN2_PIN, LOW);

  // initialize rf pins
  pinMode(RF_PIN, OUTPUT);
  digitalWrite(RF_PIN, LOW);

  // initialize level pin
  pinMode(LEVEL_PIN, INPUT);

  // initialize dht pin
  pinMode(DHT_PIN, INPUT);
}
