#include <WiFiManager.h>
#define WEBSERVER_H
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include <UnitecRCSwitch.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "wifi.h"
#include "index.h"
#include "vars.h"



Preferences preferences;

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws");

DHT dht(dht_pin, DHTTYPE);

OneWire oneWire(temp_pin);
DallasTemperature temp_sensor(&oneWire);

Scheduler ts;
void enableSpray();
void disableSpray();
void enableLight();
void disableLight();
Task tPump (spray_period * TASK_MILLISECOND, TASK_FOREVER, &enableSpray, &ts, true);
Task tPumpOff (spray_duration * TASK_MILLISECOND, TASK_ONCE, &disableSpray, &ts, false);
Task tLightOn (light_on * TASK_SECOND, TASK_FOREVER, &enableLight, &ts, true);
Task tLightOff (light_off * TASK_SECOND, TASK_ONCE, &disableLight, &ts, false);




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
               \n\t\"spray_duration\": \"" + String(spray_duration) +"\",\
               \n\t\"dhtValueTemp\": \"" + String(dhtValueTemp) +"\",\
               \n\t\"dhtValueHumidity\": \"" + String(dhtValueHumidity) +"\",\
               \n\t\"dallasValueTemp\": \"" + String(dallasValueTemp) +"\",\
               \n\t\"ph_value\": \"" + String(ph_value) +"\",\
               \n\t\"ph_calibrated\": \"" + String(ph_calibrated) +"\",\
               \n\t\"ph_analog\": \"" + String(ph_analog) +"\",\
               \n\t\"ec_value\": \"" + String(ec_value) +"\",\
               \n\t\"water_state\": \"" + String(water_state) +"\"\
               \n}");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    D_PRINTLN("Data received: ");
    D_PRINTLN((char*)data);

    DynamicJsonDocument garden_command(1024);
    DeserializationError error = deserializeJson(garden_command, data);
    if (error) {
      Serial.println("Deserialisation failed");
      Serial.println(error.f_str());
      return;
    }

    const char* command_type = garden_command["type"];
    D_PRINT("  [handleWebSocketMessage] Command type: ");
    D_PRINTLN(command_type);

    const char* command_item = garden_command["item"];
    D_PRINT("  [handleWebSocketMessage] Command item: ");
    D_PRINTLN(command_item);

    unsigned long int command_value = garden_command["value"];
    D_PRINT("  [handleWebSocketMessage] Command value: ");
    D_PRINTLN(command_value);

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
        preferences.begin("garden", false);
        preferences.putBool("scheduler", scheduler_active);
        preferences.end();
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
        D_PRINTLN("  [handleWebSocketMessage] light toggle command");
        light_toggle = 0;
        light_state = !light_state;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "calibrate_ph") == 0) {
        calibratePh(garden_command["value"]);
        notifyClients();
      }
    }
    if (strcmp(command_type, "update") == 0) {
      if (strcmp(command_item, "light_on") == 0) {
        light_on = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_on", light_on);
        preferences.end();
        tLightOn.setInterval(light_on * TASK_SECOND);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "light_off") == 0) {
        light_off = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_off", light_off);
        preferences.end();
        tLightOn.setInterval(light_off * TASK_SECOND);
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
  else if(var == "DHT_TEMP_VALUE"){
    return String(dhtValueTemp);
  }
  else if(var == "DHT_HUMIDITY_VALUE"){
    return String(dhtValueHumidity);
  }
  else if(var == "DALLAS_TEMP_VALUE"){
    return String(dallasValueTemp);
  }
  else if(var == "WATER_STATE"){
    return String(water_state);
  }
  else if(var == "PH_VALUE"){
    return String(ph_value);
  }
  else if(var == "PH_ANALOG"){
    return String(ph_analog);
  }
  else if(var == "PH_CALIBRATED"){
    return String(ph_calibrated);
  }
  else if(var == "EC_VALUE"){
    return String(ec_value);
  }
  else if(var == "WATER_STATE"){
    return String(water_state);
  }
  else {
    D_PRINTLN("template: Unknown variable");
    return String("  [processor] Unknown variable");
  }
}


//////////////////////////////////////////////
/// Pumps

void setPumpState() {
  digitalWrite(pump1_pin, pump1_state);
  digitalWrite(pump2_pin, pump2_state);
}

void enableSpray() {
  D_PRINT("  [enableSpray] Enabling spray ");
  D_PRINTLN(active_pump);
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
  D_PRINT("  [disableSpray] Disabling Spray ");
  D_PRINTLN(active_pump);
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
  D_PRINT("  [setFanPwm] Setting PWM ");
  D_PRINT(pin);
  D_PRINT(" to ");
  D_PRINT(duty);
  int dutyCycle = (duty * 255) / 100;
  D_PRINTLN(dutyCycle);
  ledcWrite(fanChannel, dutyCycle);
  //analogWrite(pin, (duty / 100) * 1023);
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
      D_PRINTLN("  [setLightState] Turning light off");
      mySwitch.switchOff(UnitecRCSwitch::SOCKET_A);
    }

    if (light_state == 0)
    {
      // Turn light on
      D_PRINTLN("  [setLightState] Turning light on");
      mySwitch.switchOn(UnitecRCSwitch::SOCKET_A);
    }
    light_toggle = !light_toggle;
  }
}

void enableLight() {
  D_PRINTLN("  [enableLight] Enabling light");
  light_state = 0;
  light_toggle = 0;
  notifyClients();
  tLightOff.restartDelayed();
};

void disableLight() {
  D_PRINTLN("  [disableLight] Disabling Light ");
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
  currentFlowMillis = millis();
  if (currentFlowMillis - previousFlowMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousFlowMillis)) * pulse1Sec) / calibrationFactor;
    previousFlowMillis = millis();

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


// DHT Humidity/Temperature
void getDhtValue() {
  dhtValueTemp = dht.readTemperature();
  dhtValueHumidity = dht.readHumidity();
  if (isnan(dhtValueTemp) || isnan(dhtValueHumidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } else {
    D_PRINT("  [getDhtValue] DHT Temperature:");
    D_PRINTLN(dhtValueTemp);
    D_PRINT("  [getDhtValue] DHT Humidity:");
    D_PRINTLN(dhtValueHumidity);
  }
}

// 18B20 Temperature
void getTempValue() {
  temp_sensor.requestTemperatures();
  dallasValueTemp = temp_sensor.getTempCByIndex(0);
  if (isnan(dallasValueTemp)) {
    Serial.println(F("Failed to read from 18B20 sensor!"));
    return;
  } else {
    D_PRINT("  [getTempValue] 18B20 Temperature:");
    D_PRINTLN(dallasValueTemp);
  }
}

// ph value
void getPhValue(float ph_calibration_m, float ph_calibration_b) {   
  ph_analog = readPhAnalog(phSampleSize, ph_pin);
  float ph_voltage = analog2Voltage(ph_analog);
  ph_value = voltage2Ph(ph_voltage, ph_calibration_m, ph_calibration_b);

  D_PRINT("  [getPhAnalog]: ");
  D_PRINTLN(ph_analog);
  D_PRINT("  [getPhVoltage]: ");
  D_PRINTLN(ph_voltage);
  D_PRINT("  [getPhValue]: ");
  D_PRINTLN(ph_value);
}

void sortArray(int* array, int size) {
  int temp;
  for(int i=0;i<(size-1);i++)
  {
    for(int j=i+1;j<size;j++)
    {
      if(array[i]>array[j])
      {
        temp=array[i];
        array[i]=array[j];
        array[j]=temp;
      }
    }
  }
}

int readPhAnalog(int sampleSize, int pin) {
  // read sampleSize amount of values
  int buf[sampleSize];
  int sumValue=0;
  int avgValue=0;
  for(int i=0;i<sampleSize;i++) 
  { 
    for(int i=0;i<sampleSize;i++) 
    { 
      buf[i]=analogRead(pin);
      D_PRINT("buf[");
      D_PRINT(i);
      D_PRINT("] = ");
      D_PRINTLN(buf[i]);
      delay(10);
    }
  }

  sortArray(buf, sampleSize);

  for(int i=0;i<sampleSize;i++) 
  { 
    Serial.print("buf[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(buf[i]);
  }  
  // ignore the lowest and highest 20% of the sample
  int ignore=(int((sampleSize*20)/100));
  for(int i=ignore;i<(sampleSize-ignore);i++){
    sumValue+=buf[i];
  }
  avgValue = sumValue/(sampleSize-(2*ignore));
  D_PRINT("avgValue = ");
  D_PRINTLN(avgValue);

  return avgValue;
}

float analog2Voltage(float analogValue) {
  float voltage=(float)analogValue*(3.3/4095.0);
  return voltage;
}

float voltage2Ph(float voltage, float cal_m, float cal_b) {
  float phValue = cal_m * voltage + cal_b;
  return phValue;
}

bool isCalibrated() {
  if (ph_calibration_m != 1 && ph_calibration_b != 0){
    return true;
  }
  else {
    return false;
  }
}

void calcCalibration(float ph1, float ph2, float phAnalog1, float phAnalog2) {
  ph_calibration_m = (ph2 - ph1) / (phAnalog2 - phAnalog1);
  ph_calibration_b = ph1 - (ph_calibration_m * phAnalog1);
  D_PRINT("  [calcCalibration] ph_calibration_b: ");
  D_PRINTLN(ph_calibration_b);
  D_PRINT("  [calcCalibration] ph_calibration_m: ");
  D_PRINTLN(ph_calibration_m);
}


void calibratePh(int ph_calib) {
  while (Serial.available() <= 0) {}
  if(ph_calib == 401) {
    ph_analog_401 = readPhAnalog(phSampleSize,ph_pin);
    D_PRINT("  [calibratePh] ph_analog_401: ");
    D_PRINTLN(ph_analog_401);  
  }
  else if(ph_calib == 686) {
    ph_analog_686 = readPhAnalog(phSampleSize,ph_pin);
    D_PRINT("  [calibratePh] ph_analog_686: ");
    D_PRINTLN(ph_analog_686);
  }
  else {
    Serial.print("Unknown ph calibration value: ");
    Serial.println(ph_calib);
  }
  if (ph_analog_401 != -1 && ph_analog_686 != -1) {
    calcCalibration(4.01, 6.86, ph_analog_401, ph_analog_686);
    preferences.begin("garden", false);
    preferences.putULong("ph_calibration_b", ph_calibration_b);
    preferences.putULong("ph_calibration_m", ph_calibration_m);
    preferences.end();
    ph_analog_401 = -1;
    ph_analog_686 = -1;
  }
}

// ecc value
void getEcValue() {
  if (isnan(ec_value)) {
    Serial.println(F("Failed to read from ec sensor!"));
    return;
  } else {
    D_PRINT("  [getEcValue] ec value:");
    D_PRINTLN(ec_value);
  }
}

// water level
void getWaterState() {
  bool state = digitalRead(level_pin);
  if( state == LOW)  {
    D_PRINTLN("  [getWaterState] Water ok");
    water_state = 1;
  } else {
    D_PRINTLN("  [getWaterState] Water low");
    water_state = 0;
  }
}

void readSensors() {
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    D_PRINTLN("  [readSensors] Reading Sensors");
    getWaterState();
    getTempValue();
    getDhtValue();
    //getEcValue;
    getPhValue(ph_calibration_m, ph_calibration_b);

    notifyClients();
    previousMillis = millis();
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

  // initialize level pin
  pinMode(level_pin, INPUT);

  // button setup for rf switch
  mySwitch.setBtnCodes(&codes);
  mySwitch.enableTransmit(rf_pin);

  // initialize dht
  pinMode(dht_pin, INPUT);
  dht.begin();

  // initialize 18B20 temperature sensor
  temp_sensor.begin();

  // initialize variables for flow meter and attach flow pin to interrupt handler
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  //previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(flow_pin), pulseCounter, FALLING);

  // setup fan pwm
  pinMode(fanpwm1_pin,OUTPUT);
  //analogWriteFrequency(25000);
  ledcSetup(fanChannel, freq, resolution);
  ledcAttachPin(fanpwm1_pin, fanChannel);

  // setup WiFiManager and OTA updates
  setupWiFiManager();  
  setupOTA();

  // configure preferences strcture for persistent saving
  preferences.begin("garden", true);
  spray_period = preferences.getULong("spray_period", 10000);
  spray_duration = preferences.getULong("spray_duration", 1000);
  light_on = preferences.getULong("light_on", 12);
  light_off = preferences.getULong("light_off", 12);
  scheduler_active = preferences.getBool("scheduler", 0);
  ph_calibration_b = preferences.getFloat("ph_calibration_b", 1);
  ph_calibration_m = preferences.getFloat("ph_calibration_m", 1);
  preferences.end();

  // check for calibration
  ph_calibrated=isCalibrated();

  // setup Scheduler intervals for pumps
  tPump.setInterval(spray_period * TASK_MILLISECOND);
  tPumpOff.setInterval(spray_duration * TASK_MILLISECOND);
  tLightOn.setInterval(light_on *TASK_SECOND);
  tLightOff.setInterval(light_off *TASK_SECOND);

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
  //setLightState();
  readSensors();
}




