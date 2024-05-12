#include "sensors.h"

// DHT Humidity/Temperature
void getDhtValue(DHT& dht, esp_sensors& sensors) {
  sensors.dhtValueTemp = dht.readTemperature();
  sensors.dhtValueHumidity = dht.readHumidity();
  if (isnan(sensors.dhtValueTemp) || isnan(sensors.dhtValueHumidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } else {
    D_SENSOR_PRINT("  [getDhtValue] DHT Temperature:");
    D_SENSOR_PRINTLN(sensors.dhtValueTemp);
    D_SENSOR_PRINT("  [getDhtValue] DHT Humidity:");
    D_SENSOR_PRINTLN(sensors.dhtValueHumidity);
  }
}

// 18B20 Temperature
void getTempValue(DallasTemperature& temp_sensor, esp_sensors& sensors) {
  temp_sensor.requestTemperatures();
  sensors.dallasValueTemp = temp_sensor.getTempCByIndex(0);
  if (isnan(sensors.dallasValueTemp)) {
    Serial.println(F("Failed to read from 18B20 sensor!"));
    return;
  } else {
    D_SENSOR_PRINT("  [getTempValue] 18B20 Temperature:");
    D_SENSOR_PRINTLN(sensors.dallasValueTemp);
  }
}


// water level
void getWaterState(esp_state& state) {
  bool state_water = digitalRead(LEVEL_PIN);
  if (state_water == LOW) {
    D_SENSOR_PRINTLN("  [getWaterState] Water ok");
    state.water = 1;
  } else {
    D_SENSOR_PRINTLN("  [getWaterState] Water low");
    state.water = 0;
  }
}
