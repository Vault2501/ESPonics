#include "setup.h"

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
