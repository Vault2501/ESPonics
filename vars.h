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


const char* myhostname = "esponics";
const char* ssid = APSSID;
const char* key = APKEY;

// setting PWM properties
const int freq = 25000;
const int fanChannel = 0;
const int resolution = 8;

// scheduler settings
bool scheduler_active = 1;
unsigned long spray_period = 10000;
unsigned long spray_duration = 1000;
unsigned long valve1_delay = 100;

// pump/valve states
int active_pump = 1;
bool pump1_state = 1;
bool pump2_state = 1;
bool valve1_state = 1;
bool valve2_state = 1;

// fan/light states
bool fan1_state = 1;
int fan1_speed = 0;
bool fan2_state = 1;
bool light_state = 1;
int light_on = 12;
int light_off = 12;
bool light_toggle = 1;
bool water_state = 0;

// pump module pins
const int pump1_pin = 27;
const int pump2_pin = 14;
const int valve1_pin = 12;
const int valve2_pin = 15;
const int flow_pin = 32;

// fan/light module pins
const int fan1_pin = 16;
const int fan2_pin = 17;
// const int fan3_pin = ;
const int fanpwm1_pin = 25;
//const int fanpwm2_pin = ;
const int rf_pin = 13;

// sensor box pins
const int temp_pin = 33;
const int dht_pin = 26;
const int ec_pin = 35;
const int ph_pin = 36;     //A0, VP
const int level_pin = 39;  //A3. VN

// sensor vars
float dhtValueTemp = 0;
float dhtValueHumidity = 0;
float dallasValueTemp = 0;
float ph_value = 7;
float ec_value = 0;

// calibration values
const int phSampleSize = 10;
int ph_analog_686 = -1;
int ph_analog_401 = -1;
int ph_analog = 0;
float ph_calibration_b = 0;
float ph_calibration_m = 1;
bool ph_calibrated = false;

// flow meter vars
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

// timing
long currentMillis = 0;
long previousMillis = 0;
long currentFlowMillis = 0;
long previousFlowMillis = 0;
