#include <WiFiManager.h>
#define WEBSERVER_H
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <TaskScheduler.h>
#include <UnitecRCSwitch.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "index.h"
#include "vars.h"
#include "wifimanager.h"

Preferences preferences;

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws");

DHT dht(DHT_PIN, DHTTYPE);

OneWire oneWire(TEMP_PIN);
DallasTemperature temp_sensor(&oneWire);

Scheduler ts;
void enableSpray();
void disableSpray();
void enableLight();
void disableLight();
void openValve1();
Task tPump(schedule.spray_period *TASK_MILLISECOND, TASK_FOREVER, &enableSpray, &ts, true);
Task tPumpOff(schedule.spray_duration *TASK_MILLISECOND, TASK_ONCE, &disableSpray, &ts, false);
Task tLightOn(schedule.light_on *TASK_SECOND, TASK_FOREVER, &enableLight, &ts, true);
Task tLightOff(schedule.light_off *TASK_SECOND, TASK_ONCE, &disableLight, &ts, false);
Task tOpenValve1(schedule.valve1_delay *TASK_MILLISECOND, TASK_ONCE, &openValve1, &ts, false);


//////////////////////////////////////////////
// Web part

void notifyClients() {
  ws.textAll("{\n\t\"pump1_state\": \"" + String(state.pump1) + "\",\
               \n\t\"pump2_state\": \""
             + String(state.pump2) + "\",\
               \n\t\"valve1_state\": \""
             + String(state.valve1) + "\",\
               \n\t\"valve2_state\": \""
             + String(state.valve2) + "\",\
               \n\t\"fan1_state\": \""
             + String(state.fan1) + "\",\
               \n\t\"fan1_speed\": \""
             + String(fan1_speed) + "\",\
               \n\t\"fan2_state\": \""
             + String(state.fan2) + "\",\
               \n\t\"light_state\": \""
             + String(state.light) + "\",\
               \n\t\"light_on\": \""
             + String(schedule.light_on) + "\",\
               \n\t\"light_off\": \""
             + String(schedule.light_off) + "\",\
               \n\t\"scheduler_active\": \""
             + String(scheduler_active) + "\",\
               \n\t\"flow_rate\": \""
             + String(flowRate) + "\",\
               \n\t\"flow_quantity\": \""
             + String(totalMilliLitres) + "\",\
               \n\t\"spray_period\": \""
             + String(schedule.spray_period) + "\",\
               \n\t\"spray_duration\": \""
             + String(schedule.spray_duration) + "\",\
               \n\t\"dhtValueTemp\": \""
             + String(sensors.dhtValueTemp) + "\",\
               \n\t\"dhtValueHumidity\": \""
             + String(sensors.dhtValueHumidity) + "\",\
               \n\t\"dallasValueTemp\": \""
             + String(sensors.dallasValueTemp) + "\",\
               \n\t\"ph_value\": \""
             + String(sensors.ph_value) + "\",\
               \n\t\"ph_calibrated\": \""
             + String(ph_calibrated) + "\",\
               \n\t\"ph_analog\": \""
             + String(ph_analog) + "\",\
               \n\t\"ec_value\": \""
             + String(sensors.ec_value) + "\",\
               \n\t\"water_state\": \""
             + String(state.water) + "\"\
               \n}");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    D_PRINTLN("Data received: ");
    D_PRINTLN((char *)data);

    DynamicJsonDocument garden_command(1024);
    DeserializationError error = deserializeJson(garden_command, data);
    if (error) {
      Serial.println("Deserialisation failed");
      Serial.println(error.f_str());
      return;
    }

    const char *command_type = garden_command["type"];
    D_PRINT("  [handleWebSocketMessage] Command type: ");
    D_PRINTLN(command_type);

    const char *command_item = garden_command["item"];
    D_PRINT("  [handleWebSocketMessage] Command item: ");
    D_PRINTLN(command_item);

    unsigned long int command_value = garden_command["value"];
    D_PRINT("  [handleWebSocketMessage] Command value: ");
    D_PRINTLN(command_value);

    if (strcmp(command_type, "toggle") == 0) {
      if (strcmp(command_item, "pump1") == 0) {
        state.pump1 = !state.pump1;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "pump2") == 0) {
        state.pump2 = !state.pump2;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "valve1") == 0) {
        state.valve1 = !state.valve1;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "valve2") == 0) {
        state.valve2 = !state.valve2;
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
        state.fan1 = !state.fan1;
        notifyClients();
      }
      if (strcmp(command_item, "fan2") == 0) {
        state.fan2 = !state.fan2;
        notifyClients();
      }
      if (strcmp(command_item, "light") == 0) {
        D_PRINTLN("  [handleWebSocketMessage] light toggle command");
        light_toggle = 0;
        state.light = !state.light;
        scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "calibrate_ph") == 0) {
        int ph = garden_command["value"];
        calibratePh(ph);
        notifyClients();
      }
    }
    if (strcmp(command_type, "update") == 0) {
      if (strcmp(command_item, "light_on") == 0) {
        schedule.light_on = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_on", schedule.light_on);
        preferences.end();
        tLightOn.setInterval(schedule.light_on * TASK_SECOND);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "light_off") == 0) {
        schedule.light_off = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_off", schedule.light_off);
        preferences.end();
        tLightOn.setInterval(schedule.light_off * TASK_SECOND);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "spray_period") == 0) {
        schedule.spray_period = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("spray_period", schedule.spray_period);
        preferences.end();
        tPump.setInterval(schedule.spray_period * TASK_MILLISECOND);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "spray_duration") == 0) {
        schedule.spray_duration = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("spray_duration", schedule.spray_duration);
        preferences.end();
        tPumpOff.setInterval(schedule.spray_duration * TASK_MILLISECOND);
        scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "fan1_speed") == 0) {
        fan1_speed = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("fan1_speed", fan1_speed);
        preferences.end();
        setFanPwm(FANPWM1_PIN, fan1_speed);
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

String processor(const String &var) {
  if (var == "PUMP1_STATE") {
    return String(state.pump1);
  } else if (var == "PUMP2_STATE") {
    return String(state.pump2);
  } else if (var == "VALVE1_STATE") {
    return String(state.valve1);
  } else if (var == "VALVE2_STATE") {
    return String(state.valve2);
  } else if (var == "SCHEDULER_ACTIVE") {
    return String(scheduler_active);
  } else if (var == "FLOW_RATE") {
    return String(flowRate);
  } else if (var == "FLOW_QUANTITY") {
    return String(totalMilliLitres);
  } else if (var == "SPRAY_PERIOD") {
    return String(schedule.spray_period);
  } else if (var == "SPRAY_DURATION") {
    return String(schedule.spray_duration);
  } else if (var == "FAN1_STATE") {
    return String(state.fan1);
  } else if (var == "FAN2_STATE") {
    return String(state.fan2);
  } else if (var == "FAN1_SPEED") {
    return String(fan1_speed);
  } else if (var == "LIGHT_STATE") {
    return String(state.light);
  } else if (var == "LIGHT_ON") {
    return String(schedule.light_on);
  } else if (var == "LIGHT_OFF") {
    return String(schedule.light_off);
  } else if (var == "DHT_TEMP_VALUE") {
    return String(sensors.dhtValueTemp);
  } else if (var == "DHT_HUMIDITY_VALUE") {
    return String(sensors.dhtValueHumidity);
  } else if (var == "DALLAS_TEMP_VALUE") {
    return String(sensors.dallasValueTemp);
  } else if (var == "WATER_STATE") {
    return String(state.water);
  } else if (var == "PH_VALUE") {
    return String(sensors.ph_value);
  } else if (var == "PH_ANALOG") {
    return String(ph_analog);
  } else if (var == "PH_CALIBRATED") {
    return String(ph_calibrated);
  } else if (var == "EC_VALUE") {
    return String(sensors.ec_value);
  } else {
    D_PRINTLN("template: Unknown variable");
    return String("  [processor] Unknown variable");
  }
}


//////////////////////////////////////////////
/// Pumps

void setPumpState_(esp_state& state) {
  digitalWrite(PUMP1_PIN, state.pump1);
  digitalWrite(PUMP2_PIN, state.pump2);
}

void setPumpState() {
  digitalWrite(PUMP1_PIN, state.pump1);
  digitalWrite(PUMP2_PIN, state.pump2);
}

void enableSpray() {
  D_PRINT("  [enableSpray] Enabling spray ");
  D_PRINTLN(active_pump);
  if (active_pump == 1) {
    state.pump1 = 0;
  }
  if (active_pump == 2) {
    state.pump2 = 0;
  }
  //state.valve1 = 0;
  tOpenValve1.restartDelayed();
  notifyClients();
  tPumpOff.restartDelayed();
};

void disableSpray() {
  D_PRINT("  [disableSpray] Disabling Spray ");
  D_PRINTLN(active_pump);
  if (active_pump == 1) {
    state.pump1 = 1;
  }
  if (active_pump == 2) {
    state.pump2 = 1;
  }
  state.valve1 = 1;
  notifyClients();
};


//////////////////////////////////////////////
/// Valves

void openValve1()
{
  state.valve1=0;
}

void setValveState() {
  digitalWrite(VALVE1_PIN, state.valve1);
  digitalWrite(VALVE2_PIN, state.valve2);
}

//////////////////////////////////////////////
/// Fans

void setFanState() {
  digitalWrite(FAN1_PIN, state.fan1);
  digitalWrite(FAN2_PIN, state.fan2);
}

void setFanPwm(int pin, int duty) {
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
  { 13721648, 14595904, 13946480, 14372784 },  // Button A ON codes
  { 13806992, 13996448, 14506528, 13831184 },  // Button A OFF codes
  { 14372788, 13946484, 14595908, 13721652 },  // Button B ON codes
  { 13831188, 14506532, 13996452, 13806996 },  // Button B OFF codes
  { 14595916, 13721660, 14372796, 13946492 },  // Button C ON codes
  { 13996460, 13807004, 13831196, 14506540 },  // Button C OFF codes
  { 13806994, 13996450, 14506530, 13831186 },  // Button D ON codes
  { 13721650, 14595906, 13946482, 14372786 },  // Button D OFF codes
};

void setLightState() {
  if (light_toggle == 0) {
    if (state.light == 1) {
      // Turn light off
      D_PRINTLN("  [setLightState] Turning light off");
      mySwitch.switchOff(UnitecRCSwitch::SOCKET_A);
    }

    if (state.light == 0) {
      // Turn light on
      D_PRINTLN("  [setLightState] Turning light on");
      mySwitch.switchOn(UnitecRCSwitch::SOCKET_A);
    }
    light_toggle = !light_toggle;
  }
}

void enableLight() {
  D_PRINTLN("  [enableLight] Enabling light");
  state.light = 0;
  light_toggle = 0;
  notifyClients();
  tLightOff.restartDelayed();
};

void disableLight() {
  D_PRINTLN("  [disableLight] Disabling Light ");
  state.light = 1;
  light_toggle = 0;
  notifyClients();
};

//////////////////////////////////////////////
/// Flow Rate

void IRAM_ATTR pulseCounter() {
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
  sensors.dhtValueTemp = dht.readTemperature();
  sensors.dhtValueHumidity = dht.readHumidity();
  if (isnan(sensors.dhtValueTemp) || isnan(sensors.dhtValueHumidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } else {
    D_PRINT("  [getDhtValue] DHT Temperature:");
    D_PRINTLN(sensors.dhtValueTemp);
    D_PRINT("  [getDhtValue] DHT Humidity:");
    D_PRINTLN(sensors.dhtValueHumidity);
  }
}

// 18B20 Temperature
void getTempValue() {
  temp_sensor.requestTemperatures();
  sensors.dallasValueTemp = temp_sensor.getTempCByIndex(0);
  if (isnan(sensors.dallasValueTemp)) {
    Serial.println(F("Failed to read from 18B20 sensor!"));
    return;
  } else {
    D_PRINT("  [getTempValue] 18B20 Temperature:");
    D_PRINTLN(sensors.dallasValueTemp);
  }
}

// ph value
void getPhValue(float ph_calibration_m, float ph_calibration_b) {
  ph_analog = readPhAnalog(phSampleSize, PH_PIN);
  float ph_voltage = analog2Voltage(ph_analog);
  sensors.ph_value = voltage2Ph(ph_voltage, ph_calibration_m, ph_calibration_b);

  D_PRINT("  [getPhAnalog]: ");
  D_PRINTLN(ph_analog);
  D_PRINT("  [getPhVoltage]: ");
  D_PRINTLN(ph_voltage);
  D_PRINT("  [getPhValue]: ");
  D_PRINTLN(sensors.ph_value);
}

void sortArray(int *array, int size) {
  int temp;
  for (int i = 0; i < (size - 1); i++) {
    for (int j = i + 1; j < size; j++) {
      if (array[i] > array[j]) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
      }
    }
  }
}

int readPhAnalog(int sampleSize, int pin) {
  // read sampleSize amount of values
  int buf[sampleSize];
  int sumValue = 0;
  int avgValue = 0;
  for (int i = 0; i < sampleSize; i++) {
    for (int i = 0; i < sampleSize; i++) {
      buf[i] = analogRead(pin);
      delay(10);
    }
  }

  sortArray(buf, sampleSize);

  // ignore the lowest and highest 20% of the sample
  int ignore = (int((sampleSize * 20) / 100));
  for (int i = ignore; i < (sampleSize - ignore); i++) {
    sumValue += buf[i];
  }
  avgValue = sumValue / (sampleSize - (2 * ignore));

  return avgValue;
}

float analog2Voltage(float analogValue) {
  float voltage = (float)analogValue * (3.3 / 4095.0);
  return voltage;
}

float voltage2Ph(float voltage, float cal_m, float cal_b) {
  float phValue = cal_m * voltage + cal_b;
  return phValue;
}

bool isCalibrated() {
  if (ph_calibration_m != 1 && ph_calibration_b != 0) {
    return true;
  } else {
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
  if (ph_calib == 401) {
    ph_analog_401 = readPhAnalog(phSampleSize, PH_PIN);
    D_PRINT("  [calibratePh] ph_analog_401: ");
    D_PRINTLN(ph_analog_401);
  } else if (ph_calib == 686) {
    ph_analog_686 = readPhAnalog(phSampleSize, PH_PIN);
    D_PRINT("  [calibratePh] ph_analog_686: ");
    D_PRINTLN(ph_analog_686);
  } else {
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
  if (isnan(sensors.ec_value)) {
    Serial.println(F("Failed to read from ec sensor!"));
    return;
  } else {
    D_PRINT("  [getEcValue] ec value:");
    D_PRINTLN(sensors.ec_value);
  }
}

// water level
void getWaterState() {
  bool state_water = digitalRead(LEVEL_PIN);
  if (state_water == LOW) {
    D_PRINTLN("  [getWaterState] Water ok");
    state.water = 1;
  } else {
    D_PRINTLN("  [getWaterState] Water low");
    state.water = 0;
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
/// Main part

void setup() {

  Serial.begin(115200);
  Serial.println("Starting");

  // initialize pump pins
  pinMode(PUMP1_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN, LOW);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP2_PIN, LOW);

  // initialize valve pins
  pinMode(VALVE1_PIN, OUTPUT);
  digitalWrite(VALVE1_PIN, LOW);
  pinMode(VALVE2_PIN, OUTPUT);
  digitalWrite(VALVE2_PIN, LOW);

  // initialize flow pin
  pinMode(FLOW_PIN, INPUT_PULLUP);

  // initialize fan pins
  pinMode(FAN1_PIN, OUTPUT);
  digitalWrite(FAN1_PIN, LOW);
  pinMode(FAN2_PIN, OUTPUT);
  digitalWrite(FAN2_PIN, LOW);

  // initialize rf pins
  pinMode(RF_PIN, OUTPUT);
  digitalWrite(RF_PIN, LOW);

  // initialize level pin
  pinMode(LEVEL_PIN, INPUT);

  // button setup for rf switch
  mySwitch.setBtnCodes(&codes);
  mySwitch.enableTransmit(RF_PIN);

  // initialize dht
  pinMode(DHT_PIN, INPUT);
  dht.begin();

  // initialize 18B20 temperature sensor
  temp_sensor.begin();

  // initialize variables for flow meter and attach flow pin to interrupt handler
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  //previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), pulseCounter, FALLING);

  // setup fan pwm
  pinMode(FANPWM1_PIN, OUTPUT);
  //analogWriteFrequency(25000);
  ledcSetup(fanChannel, freq, resolution);
  ledcAttachPin(FANPWM1_PIN, fanChannel);
  //ledcAttach(FANPWM1_PIN, freq, resolution);
  //ledcAttachChannel(FANPWM1_PIN, freq, resolution, fanChannel);

  // setup WiFiManager
  setupWiFiManager();

  // configure preferences structure for persistent saving
  preferences.begin("garden", true);
  schedule.spray_period = preferences.getULong("spray_period", 10000);
  schedule.spray_duration = preferences.getULong("spray_duration", 1000);
  schedule.light_on = preferences.getULong("light_on", 12);
  schedule.light_off = preferences.getULong("light_off", 12);
  schedule.valve1_delay = preferences.getULong("valve1_delay", 100);
  scheduler_active = preferences.getBool("scheduler", 0);
  ph_calibration_b = preferences.getFloat("ph_calibration_b", 1);
  ph_calibration_m = preferences.getFloat("ph_calibration_m", 1);
  preferences.end();

  // check for calibration
  ph_calibrated = isCalibrated();

  // setup Scheduler intervals for pumps
  tPump.setInterval(schedule.spray_period * TASK_MILLISECOND);
  tPumpOff.setInterval(schedule.spray_duration * TASK_MILLISECOND);

  // setup Scheduler intervals for light
  tLightOn.setInterval(schedule.light_on * TASK_SECOND);
  tLightOff.setInterval(schedule.light_off * TASK_SECOND);

  // setup Scheduler intervals for valves
  tOpenValve1.setInterval(schedule.valve1_delay * TASK_SECOND);

  // start websocket
  initWebSocket();

  // setup handler for web access
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
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
  readSensors();

  //notifyClients();
}
