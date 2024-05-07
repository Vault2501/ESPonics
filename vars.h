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


// WiFi AP credentials
const char* myhostname = "esponics";

// Setting PWM properties
const int freq = 25000;
const int fanChannel = 0;
const int resolution = 8;

// Scheduler active (1=off, 0=on)
bool scheduler_active = 1;

// Active pump (1/2)
int active_pump = 1;

// pwm fan speed (0-100)
int fan1_speed = 0;

// Toggle light (0=yes,1=no)
bool light_toggle = 1;

// Calibration values
const int phSampleSize = 10;
int ph_analog_686 = -1;
int ph_analog_401 = -1;
int ph_analog = 0;
float ph_calibration_b = 0;
float ph_calibration_m = 1;
bool ph_calibrated = false;

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
