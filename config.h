//#define DEBUG

#ifdef DEBUG
#define D_PRINT(x) Serial.print(x)
#define D_PRINTDEC(x) Serial.print(x, DEC)
#define D_PRINTLN(x) Serial.println(x)
#else
#define D_PRINT(x)
#define D_PRINTDEC(x)
#define D_PRINTLN(x)
#endif

#ifndef CONFIG
#define CONFIG

// Pump module pins
#define PUMP1_PIN 27
#define PUMP2_PIN 14
#define VALVE1_PIN 12
#define VALVE2_PIN 15
#define FLOW_PIN 32
// Fan/light module pins
#define FAN1_PIN 16
#define FAN2_PIN 17
#define FANPWM1_PIN 25
// Light rf switch pin
#define RF_PIN 13
// Sensor pins
#define TEMP_PIN 33
#define DHT_PIN 26
#define TDS_PIN 35
#define PH_PIN 36     //A0, VP
#define LEVEL_PIN 39  //A3. VN

// Board voltage and samplerate
#define AREF 3300
#define ADCRANGE 4096.0

// dht type
#define DHTTYPE DHT22

// Default PWM properties
#define PWM_FREQ 25000
#define PWM_FANCHANNEL 0
#define PWM_RESOLUTION 8

// Sensor read interval in ms
#define SENSOR_INTERVAL 1000

// Spray schedule defaults (milliseconds)
#define SPRAY_PERIOD 10000
#define SPRAY_DURATION 1000
#define VALVE1_DELAY 100
// Light schedule defaults (hours)
#define LIGHT_ON 12
#define LIGHT_OFF 12

// Default active pump (1/2)
#define ACTIVE_PUMP 1
// Default scheduler active
#define SCHEDULER_ACTIVE 0
// Default pwm fan speed (0-100)
#define FAN1_SPEED 0
// Calibration defaults
//#define PH_CALIBRATED false
#define PH_NEUTRALVOLTAGE 1500.0
#define PH_ACIDVOLTAGE 2032.44
//#define TDS_CALIBRATED false
#define TDS_KVALUE 1.0
// PH sample size
#define PH_SAMPLESIZE 10
// PH ranges
#define PH_SENSOR_DUPONT
//#define PH_SENSOR_GRAVITY_V2

#ifdef PH_SENSOR_DUPONT
#define PH_8_VOLTAGE 1500
#define PH_6_VOLTAGE 1800
#define PH_5_VOLTAGE 2000
#define PH_3_VOLTAGE 2310
#endif

#ifdef PH_SENSOR_GRAVITY_V2
#define PH_8_VOLTAGE 1122
#define PH_6_VOLTAGE 1478
#define PH_5_VOLTAGE 1654
#define PH_3_VOLTAGE 2010
#endif

// flow ensor
#define FLOW_CALIBRATION 4.5


#endif