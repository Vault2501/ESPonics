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

// WiFi AP credentials
const char* myhostname = "esponics";

// Setting PWM properties
const int freq = 25000;
const int fanChannel = 0;
const int resolution = 8;

// Scheduler settings (milliseconds)
bool scheduler_active = 1;
unsigned long spray_period = 10000;
unsigned long spray_duration = 1000;
unsigned long valve1_delay = 100;

// Active pump (1/2)
int active_pump = 1;
// Pump/valve states (1 = off)
bool pump1_state = 1;
bool pump2_state = 1;
bool valve1_state = 1;
bool valve2_state = 1;

// Fan/light states (1 = off)
bool fan1_state = 1;
bool fan2_state = 1;
bool light_state = 1;
// pwm fan speed (0-100)
int fan1_speed = 0;

// Light schedule defaults (hours)
int light_on = 12;
int light_off = 12;

// Toggle light (0=yes,1=no)
bool light_toggle = 1;

// Water level (0=low,1=ok)
bool water_state = 0;

// Pump module pins
const int pump1_pin = 27;
const int pump2_pin = 14;
const int valve1_pin = 12;
const int valve2_pin = 15;
const int flow_pin = 32;

// Fan/light module pins
const int fan1_pin = 16;
const int fan2_pin = 17;
const int fanpwm1_pin = 25;
const int rf_pin = 13;

// Sensor box pins
const int temp_pin = 33;
const int dht_pin = 26;
const int ec_pin = 35;
const int ph_pin = 36;     //A0, VP
const int level_pin = 39;  //A3. VN

// Sensor vars
float dhtValueTemp = 0;
float dhtValueHumidity = 0;
float dallasValueTemp = 0;
float ph_value = 7;
float ec_value = 0;

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
