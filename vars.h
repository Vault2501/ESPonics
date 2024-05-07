#include "config.h"

#define DEBUG
#define DHTTYPE DHT22

#ifdef DEBUG
#define D_PRINT(x) Serial.print(x)
#define D_PRINTDEC(x) Serial.print(x, DEC)
#define D_PRINTLN(x) Serial.println(x)
#else
#define D_PRINT(x)
#define D_PRINTDEC(x)
#define D_PRINTLN(x)
#endif

struct esp_settings {
  bool scheduler_active;
  int active_pump;
  int fan1_speed;
  bool ph_calibrated;
  float ph_calibration_b;
  float ph_calibration_m;
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
  float ec_value;
};

esp_settings settings={SCHEDULER_ACTIVE,
                       ACTIVE_PUMP,
                       FAN1_SPEED,
                       PH_CALIBRATED,
                       PH_CALIBRATION_B,
                       PH_CALIBRATION_M};

esp_state state={0,
                 0,
                 0,
                 0,
                 0,
                 0,
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
                       0};


// Hostname
const char* myhostname = "esponics";

// Toggle light (0=yes,1=no)
bool light_toggle = 1;

// Calibration values
const int phSampleSize = 10;
int ph_analog_686 = -1;
int ph_analog_401 = -1;
int ph_analog = 0;

// Flow meter vars
int interval = 1000;
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
