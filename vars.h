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


// Pump/valve states (1 = off)
bool pump1_state = 1;
bool pump2_state = 1;
bool valve1_state = 1;
bool valve2_state = 1;
// Fan/light states (1 = off)
bool fan1_state = 1;
bool fan2_state = 1;
bool light_state = 1;
// Water level (0=low,1=ok)
bool water_state = 0;

esp_state state={pump1_state,
                 pump2_state,
                 valve1_state,
                 valve2_state,
                 fan1_state,
                 fan2_state,
                 light_state,
                 water_state};


// Spray schedule settings (milliseconds)
unsigned long spray_period = 10000;
unsigned long spray_duration = 1000;
unsigned long valve1_delay = 100;
// Light schedule defaults (hours)
int light_on = 12;
int light_off = 12;

esp_schedule schedule{spray_period,
                      spray_duration,
                      valve1_delay,
                      light_on,
                      light_off};


// Sensor vars
float dhtValueTemp = 0;
float dhtValueHumidity = 0;
float dallasValueTemp = 0;
float ph_value = 7;
float ec_value = 0;

esp_sensors sensors = {dhtValueTemp,
                       dhtValueHumidity,
                       dallasValueTemp,
                       ph_value,
                       ec_value};


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
