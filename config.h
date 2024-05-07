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
#define RF_PIN 13
// Sensor box pins
#define TEMP_PIN 33
#define DHT_PIN 26
#define EC_PIN 35
#define PH_PIN 36     //A0, VP
#define LEVEL_PIN 39  //A3. VN

// Spray schedule defaults (milliseconds)
#define SPRAY_PERIOD 10000
#define SPRAY_DURATION 1000
#define VALVE1_DELAY 100
// Light schedule defaults (hours)
#define LIGHT_ON 12
#define LIGHT_OFF 12

#endif