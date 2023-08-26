#define DEBUG 1
#define DHTTYPE DHT22


const char* myhostname = "esponics";
const char* ssid = APSSID;
const char* key =  APKEY;

// setting PWM properties
const int freq = 25000;
const int fanChannel = 0;
const int resolution = 8;

// scheduler settings
bool scheduler_active=1;
unsigned long spray_period = 10000;
unsigned long spray_duration = 1000;

// pump/valve states
int active_pump=1;
bool pump1_state=1;
bool pump2_state=1;
bool valve1_state=1;
bool valve2_state=1;

// fan/light states
bool fan1_state=1;
int fan1_speed=0;
bool fan2_state=1;
bool light_state=1;
int light_on=12;
int light_off=12;
bool light_toggle=1;
bool water_state=0;

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
const int temp_pin = 39;
const int dht_pin = 36;
const int ec_pin = 35;
const int ph_pin = 34;
const int level_pin = 33;

// sensor vars
float dhtValueTemp = 0;
float dhtValueHumidity = 0;
float dallasValueTemp;

// flow meter vars
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
