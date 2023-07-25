#include <WiFiManager.h>
#define WEBSERVER_H
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include <UnitecRCSwitch.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "wifi.h"
#include "index.h"

#define DEBUG 1



const char* myhostname = "esponics";
const char* ssid = APSSID;
const char* key =  APKEY;

bool pump1_state=1;
bool pump2_state=1;
bool valve1_state=1;
bool valve2_state=1;
bool fan1_state=1;
int fan1_speed = 0;
bool fan2_state=1;
bool light_state=1;
int light_on=12;
int light_off=12;
bool light_toggle=1;
int active_pump=1;
bool scheduler_active=1;

const int pump1_pin = 27;
const int pump2_pin = 14;
const int valve1_pin = 12;
const int valve2_pin = 15;
const int fan1_pin = 16;
const int fan2_pin = 17;
const int fanpwm1_pin = 25;
//const int fanpwm2_pin = ;
const int rf_pin = 13;
const int flow_pin = 32;

Preferences preferences;

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws");

unsigned long spray_period = 10000;
unsigned long spray_duration = 1000;

Scheduler ts;
void enableSpray();
void disableSpray();
void enableLight();
void disableLight();
Task tPump (spray_period * TASK_MILLISECOND, TASK_FOREVER, &enableSpray, &ts, true);
Task tPumpOff (spray_duration * TASK_MILLISECOND, TASK_ONCE, &disableSpray, &ts, false);
Task tLightOn (light_on * TASK_HOUR, TASK_FOREVER, &enableLight, &ts, true);
Task tLightOff (light_off * TASK_HOUR, TASK_ONCE, &disableLight, &ts, false);

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;


//////////////////////////////////////////////
// WiFi Manager part

void setupWiFiManager() {
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect(APSSID, APKEY);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("No WiFi found");
  Serial.print("AP:");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.print("Key:");
  Serial.println(key);
  Serial.print("IP:");
  Serial.println(WiFi.softAPIP());
}


//////////////////////////////////////////////
// Web part

void notifyClients() {
  ws.textAll("{\n\t\"pump1_state\": \"" + String(pump1_state) + "\",\
               \n\t\"pump2_state\": \"" + String(pump2_state) + "\",\
               \n\t\"valve1_state\": \"" + String(valve1_state) +"\",\
               \n\t\"valve2_state\": \"" + String(valve2_state) +"\",\
               \n\t\"fan1_state\": \"" + String(fan1_state) +"\",\
               \n\t\"fan1_speed\": \"" + String(fan1_speed) +"\",\
               \n\t\"fan2_state\": \"" + String(fan2_state) +"\",\
               \n\t\"light_state\": \"" + String(light_state) +"\",\
               \n\t\"light_on\": \"" + String(light_on) +"\",\
               \n\t\"light_off\": \"" + String(light_off) +"\",\
               \n\t\"scheduler_active\": \"" + String(scheduler_active) +"\",\ 
               \n\t\"flow_rate\": \"" + String(flowRate) +"\",\
               \n\t\"flow_quantity\": \"" + String(totalMilliLitres) +"\",\
               \n\t\"spray_period\": \"" + String(spray_period) +"\",\
               \n\t\"spray_duration\": \"" + String(spray_duration) +"\"\n}");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println("Data received: ");
    Serial.println((char*)data);

    DynamicJsonDocument garden_command(1024);
    DeserializationError error = deserializeJson(garden_command, data);
    if (error) {
      Serial.println("Deserialisation failed");
      Serial.println(error.f_str());
      return;
    }

    const char* command_type = garden_command["type"];
    Serial.print("Command type: ");
    Serial.println(command_type);

    const char* command_item = garden_command["item"];
    Serial.print("Command item: ");
    Serial.println(command_item);

    unsigned long int command_value = garden_command["value"];
    Serial.print("Command value: ");
    Serial.println(command_value);

    if (strcmp(command_type, "toggle") == 0) {
      if (strcmp(command_item, "pump1") == 0) {
        pump1_state = !pump1_state;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "pump2") == 0) {
        pump2_state = !pump2_state;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "valve1") == 0) {
        valve1_state = !valve1_state;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "valve2") == 0) {
        valve2_state = !valve2_state;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "scheduler") == 0) {
        scheduler_active = !scheduler_active;
        notifyClients();
      }
      if (strcmp(command_item, "fan1") == 0) {
        fan1_state = !fan1_state;
        notifyClients();
      }
      if (strcmp(command_item, "fan2") == 0) {
        fan2_state = !fan2_state;
        notifyClients();
      }
      if (strcmp(command_item, "light") == 0) {
        Serial.println("light toggle command");
        light_toggle = 0;
        light_state = !light_state;
        scheduler_active = 0;
        notifyClients();
      }
    }
    if (strcmp(command_type, "update") == 0) {
      if (strcmp(command_item, "light_on") == 0) {
        spray_period = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_on", light_on);
        preferences.end();
        tLightOn.setInterval(light_on * TASK_HOUR);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "light_off") == 0) {
        spray_period = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_off", light_off);
        preferences.end();
        tLightOn.setInterval(light_off * TASK_HOUR);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "spray_period") == 0) {
        spray_period = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("spray_period", spray_period);
        preferences.end();
        tPump.setInterval(spray_period * TASK_MILLISECOND);
        scheduler_active = 1;
        notifyClients();
      }      
      if (strcmp(command_item, "spray_duration") == 0) {
        spray_duration = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("spray_duration", spray_duration);
        preferences.end();
        tPumpOff.setInterval(spray_duration * TASK_MILLISECOND);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "fan1_speed") == 0) {
        fan1_speed = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("fan1_speed", fan1_speed);
        preferences.end();
        setFanPwm(fanpwm1_pin,fan1_speed);
        notifyClients();
      }
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  if(var == "PUMP1_STATE"){
    return String(pump1_state);
  }
  else if(var == "PUMP2_STATE"){
    return String(pump2_state);
  }
  else if(var == "VALVE1_STATE"){
    return String(valve1_state);
  }
  else if(var == "VALVE2_STATE"){
    return String(valve2_state);
  }
  else if(var == "SCHEDULER_ACTIVE"){
    return String(scheduler_active);
  }
  else if(var == "FLOW_RATE"){
    return String(flowRate);
  }
  else if(var == "FLOW_QUANTITY"){
    return String(totalMilliLitres);
  }
  else if(var == "SPRAY_PERIOD"){
    return String(spray_period);
  }
  else if(var == "SPRAY_DURATION"){
    return String(spray_duration);
  }
  else if(var == "FAN1_STATE"){
    return String(fan1_state);
  }  
  else if(var == "FAN2_STATE"){
    return String(fan2_state);
  }
  else if(var == "FAN1_SPEED"){
    return String(fan1_speed);
  }
  else if(var == "LIGHT_STATE"){
    return String(light_state);
  }
  else if(var == "LIGHT_ON"){
    return String(light_on);
  }
  else if(var == "LIGHT_OFF"){
    return String(light_off);
  }
  else {
    Serial.println("template: Unknown variable");
    return String("Unknown variable");
  }
}


//////////////////////////////////////////////
/// Pumps

void setPumpState() {
  digitalWrite(pump1_pin, pump1_state);
  digitalWrite(pump2_pin, pump2_state);
}

void enableSpray() {
  Serial.print("Enabling spray ");
  Serial.println(active_pump);
  if (active_pump == 1) {
    pump1_state = 0;
  }
  if (active_pump == 2) {
    pump2_state = 0;
  }
  valve1_state = 0;
  notifyClients();
  tPumpOff.restartDelayed();
};

void disableSpray() {
  Serial.print("Disabling Spray ");
  Serial.println(active_pump);
  if (active_pump == 1) {
    pump1_state = 1;
  }
  if (active_pump == 2) {
    pump2_state = 1;
  }
  valve1_state = 1;
  notifyClients();
};


//////////////////////////////////////////////
/// Valves

void setValveState() {
  digitalWrite(valve1_pin, valve1_state);
  digitalWrite(valve2_pin, valve2_state);  
}

//////////////////////////////////////////////
/// Fans

void setFanState()
{
  digitalWrite(fan1_pin,fan1_state);
  digitalWrite(fan2_pin,fan2_state);
}

void setFanPwm(int pin, int duty)
{
  Serial.print("Setting PWM ");
  Serial.print(pin);
  Serial.print(" to ");
  Serial.println(duty);
  analogWrite(pin, (duty / 100) * 1023);
}
//////////////////////////////////////////////
/// Light

UnitecRCSwitch mySwitch;

UnitecRCSwitch::ButtonCodes codes = {
 {13721648, 14595904, 13946480, 14372784}, // Button A ON codes
 {13806992, 13996448, 14506528, 13831184}, // Button A OFF codes
 {14372788, 13946484, 14595908, 13721652}, // Button B ON codes
 {13831188, 14506532, 13996452, 13806996}, // Button B OFF codes
 {14595916, 13721660, 14372796, 13946492}, // Button C ON codes
 {13996460, 13807004, 13831196, 14506540}, // Button C OFF codes
 {13806994, 13996450, 14506530, 13831186}, // Button D ON codes
 {13721650, 14595906, 13946482, 14372786}, // Button D OFF codes
};

void setLightState()
{
  if (light_toggle == 0)
  {
    if (light_state == 1)
    {
      // Turn light off
      Serial.println("Turning light off");
      mySwitch.switchOff(UnitecRCSwitch::SOCKET_A);
    }

    if (light_state == 0)
    {
      // Turn light on
      Serial.println("Turning light on");
      mySwitch.switchOn(UnitecRCSwitch::SOCKET_A);
    }
    light_toggle = !light_toggle;
  }
}

void enableLight() {
  Serial.print("Enabling light");
  light_state = 0;
  light_toggle = 0;
  notifyClients();
  tLightOff.restartDelayed();
};

void disableLight() {
  Serial.print("Disabling Light ");
  light_state = 1;
  light_toggle = 0;
  notifyClients();
};

//////////////////////////////////////////////
/// Flow Rate

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void getFlowRate() {
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    //Serial.print("Flow rate: ");
    //Serial.print(int(flowRate));  // Print the integer part of the variable
    //Serial.print("L/min");
    //Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    //Serial.print("Output Liquid Quantity: ");
    //Serial.print(totalMilliLitres);
    //Serial.print("mL / ");
    //Serial.print(totalMilliLitres / 1000);
    //Serial.println("L");
  }
}


//////////////////////////////////////////////
/// OTA part

void setupOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(myhostname);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //SPIFFS.end();
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}


//////////////////////////////////////////////
/// Main part

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");

  // initialize pump pins
  pinMode(pump1_pin, OUTPUT);
  digitalWrite(pump1_pin, LOW);
  pinMode(pump2_pin, OUTPUT);
  digitalWrite(pump2_pin, LOW);

  // initialize valve pins
  pinMode(valve1_pin, OUTPUT);
  digitalWrite(valve1_pin, LOW);
  pinMode(valve2_pin, OUTPUT);
  digitalWrite(valve2_pin, LOW);

  // initialize flow pin
  pinMode(flow_pin, INPUT_PULLUP);

  // initialize fan pins
  pinMode(fan1_pin, OUTPUT);
  digitalWrite(fan1_pin, LOW);
  pinMode(fan2_pin, OUTPUT);
  digitalWrite(fan2_pin,LOW);

  // initialize rf pins
  pinMode(rf_pin,OUTPUT);
  digitalWrite(rf_pin,LOW);

  // button setup for rf switch
  mySwitch.setBtnCodes(&codes);
  mySwitch.enableTransmit(rf_pin);

  // initialize variables for flow meter and attach flow pin to interrupt handler
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(flow_pin), pulseCounter, FALLING);

  pinMode(fanpwm1_pin,OUTPUT);
  analogWriteFrequency(25000);

  // setup WiFiManager and OTA updates
  setupWiFiManager();  
  setupOTA();

  // configure preferences strcture for persistent saving
  preferences.begin("garden", true);
  spray_period = preferences.getULong("spray_period", 10000);
  spray_duration = preferences.getULong("spray_duration", 1000);
  light_on = preferences.getULong("light_on", 12);
  light_off = preferences.getULong("light_off", 12);
  preferences.end();

  // setup Scheduler intervals for pumps
  tPump.setInterval(spray_period * TASK_MILLISECOND);
  tPumpOff.setInterval(spray_duration * TASK_MILLISECOND);
  tLightOn.setInterval(light_on *TASK_HOUR);
  tLightOff.setInterval(light_off *TASK_HOUR);

  // start websocket
  initWebSocket();

  // setup handler for web access
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // start webserver
  server.begin();
}

void loop() {
  ws.cleanupClients();
  if (scheduler_active == 1) {
    ts.execute();
  }
  getFlowRate();
  setPumpState();
  setValveState();
  setFanState();
  setLightState();
}




