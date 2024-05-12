#include <DHT.h>
#include <DallasTemperature.h>
#include "Arduino.h"
#include "struct.h"
#include "config.h"


#define DEBUG_SENSOR

#ifdef DEBUG_SENSOR
#define D_SENSOR_PRINT(x) Serial.print(x)
#define D_SENSOR_PRINTDEC(x) Serial.print(x, DEC)
#define D_SENSOR_PRINTLN(x) Serial.println(x)
#else
#define D_SENSOR_PRINT(x)
#define D_SENSOR_PRINTDEC(x)
#define D_SENSOR_PRINTLN(x)
#endif

void getDhtValue(DHT& dht, esp_sensors& sensors);
void getTempValue(DallasTemperature& temp_sensor, esp_sensors& sensors);
void getWaterState(esp_state& state);