#ifndef VARS
#define VARS

#include "config.h"
#include "struct.h"

esp_settings settings={SCHEDULER_ACTIVE,
                       ACTIVE_PUMP,
                       FAN1_SPEED,
                       false,
                       false,
                       PH_NEUTRALVOLTAGE,
                       PH_ACIDVOLTAGE,
                       TDS_KVALUE,
                       FLOW_CALIBRATION};

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
                       0,
                       0.0,
                       SENSOR_INTERVAL,
                       0,
                       0};

esp_message message = {"Started",
                       "undefined",
                       "undefined",
                       "undefiend"};

// Hostname
//const char* hostname = HOSTNAME;

// Toggle light (0=yes,1=no)
bool light_toggle = 1;

// Flow meter vars
volatile byte pulseCount;
byte pulse1Sec = 0;
unsigned long flow_cal_measured = 0;


// Timing
long currentMillis = 0;
long previousMillis = 0;
long currentFlowMillis = 0;
long previousFlowMillis = 0;

// Date and time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

#endif
