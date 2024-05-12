#include "config.h"

struct esp_settings {
  bool scheduler_active;
  int active_pump;
  int fan1_speed;
  bool ph_calibrated;
  bool tds_calibrated;
  float ph_calibration_b;
  float ph_calibration_m;
  float tds_kvalue;
};

struct esp_state {
  bool pump1;
  bool pump2;
  bool valve1;
  bool valve2;
  bool fan1;
  bool fan2;
  bool light;
  bool water;
};

struct esp_schedule {
  unsigned long spray_period;
  unsigned long spray_duration;
  unsigned long valve1_delay;
  int light_on;
  int light_off;
};

struct esp_sensors {
  float dhtValueTemp;
  float dhtValueHumidity;
  float dallasValueTemp;
  float ph_value;
  float ph_analog;
  float tds_value;
  float tds_analog;
  int interval;
};

esp_settings settings={SCHEDULER_ACTIVE,
                       ACTIVE_PUMP,
                       FAN1_SPEED,
                       PH_CALIBRATED,
                       TDS_CALIBRATED,
                       PH_CALIBRATION_B,
                       PH_CALIBRATION_M,
                       TDS_KVALUE};

esp_state state={1,
                 1,
                 1,
                 1,
                 1,
                 1,
                 1,
                 0};

esp_schedule schedule{SPRAY_PERIOD,
                      SPRAY_DURATION,
                      VALVE1_DELAY,
                      LIGHT_ON,
                      LIGHT_OFF};

esp_sensors sensors = {0,
                       0,
                       0,
                       7,
                       0,
                       0,
                       0,
                       SENSOR_INTERVAL};


// Hostname
//const char* hostname = HOSTNAME;

// Toggle light (0=yes,1=no)
bool light_toggle = 1;

// ph variables
//const int phSampleSize = 10;
//int ph_analog_686 = -1;
//int ph_analog_401 = -1;
//int ph_analog = 0;
//int tds_analog = 0;

// Flow meter vars
//int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

// Timing
long currentMillis = 0;
long previousMillis = 0;
long currentFlowMillis = 0;
long previousFlowMillis = 0;
