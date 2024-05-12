#include "config.h"
#include "struct.h"

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

// Flow meter vars
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
