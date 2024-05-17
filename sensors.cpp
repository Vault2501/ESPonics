#include "sensors.h"

// DHT Temperature
float getDhtValueTemp(DHT& dht) {
  float dhtValueTemp = dht.readTemperature();
  if (isnan(dhtValueTemp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return -1;
  } else {
    D_SENSOR_PRINT("  [getDhtValueTemp] DHT Temperature:");
    D_SENSOR_PRINTLN(dhtValueTemp);
    return dhtValueTemp;
  }
}

// DHT Humidity
float getDhtValueHumidity(DHT& dht) {
  float dhtValueHumidity = dht.readHumidity();
  if (isnan(dhtValueHumidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return -1;
  } else {
    D_SENSOR_PRINT("  [getDhtValueHumidity] DHT Humidity:");
    D_SENSOR_PRINTLN(dhtValueHumidity);
    return dhtValueHumidity;
  }
}

// 18B20 Temperature
float getTempValue(DallasTemperature& temp_sensor) {
  temp_sensor.requestTemperatures();
  float dallasValueTemp = temp_sensor.getTempCByIndex(0);
  if (isnan(dallasValueTemp)) {
    Serial.println(F("Failed to read from 18B20 sensor!"));
    return -1;
  } else {
    D_SENSOR_PRINT("  [getTempValue] 18B20 Temperature:");
    D_SENSOR_PRINTLN(dallasValueTemp);
    return dallasValueTemp;
  }
}

// water level
bool getWaterState() {
  bool state_water = digitalRead(LEVEL_PIN);
  if (state_water == LOW) {
    D_SENSOR_PRINTLN("  [getWaterState] Water ok");
    return 1;
  } else {
    D_SENSOR_PRINTLN("  [getWaterState] Water low");
    return 0;
  }
}