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
#include "struct.h"
#include "vars.h"
#include "setup.h"
#include "wifimanager.h"
#include "ph2.h"
#include "tds.h"
#include "config.h"
#include "sensors.h"

Preferences preferences;

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws");

DHT dht(DHT_PIN, DHTTYPE);

PH ph;
TDS tds;

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
  ws.textAll("{\n\t\"pump1_state\": \""
             + String(state.pump1) + "\",\
               \n\t\"pump2_state\": \""
             + String(state.pump2) + "\",\
               \n\t\"valve1_state\": \""
             + String(state.valve1) + "\",\
               \n\t\"valve2_state\": \""
             + String(state.valve2) + "\",\
               \n\t\"fan1_state\": \""
             + String(state.fan1) + "\",\
               \n\t\"fan1_speed\": \""
             + String(settings.fan1_speed) + "\",\
               \n\t\"fan2_state\": \""
             + String(state.fan2) + "\",\
               \n\t\"light_state\": \""
             + String(state.light) + "\",\
               \n\t\"light_on\": \""
             + String(schedule.light_on) + "\",\
               \n\t\"light_off\": \""
             + String(schedule.light_off) + "\",\
               \n\t\"scheduler_active\": \""
             + String(settings.scheduler_active) + "\",\
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
             + String(settings.ph_calibrated) + "\",\
               \n\t\"ph_analog\": \""
             + String(sensors.ph_analog) + "\",\
               \n\t\"ph_neutralVoltage\": \""
             + String(settings.ph_neutralVoltage) + "\",\
               \n\t\"ph_acidVoltage\": \""
             + String(settings.ph_acidVoltage) + "\",\
               \n\t\"tds_value\": \""
             + String(sensors.tds_value) + "\",\
               \n\t\"tds_calibrated\": \""
             + String(settings.tds_calibrated) + "\",\
               \n\t\"tds_analog\": \""
             + String(sensors.tds_analog) + "\",\
               \n\t\"water_state\": \""
             + String(state.water) + "\",\
               \n\t\"message_log\": \""
             + String(message.log) + "\"\
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
        settings.scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "pump2") == 0) {
        state.pump2 = !state.pump2;
        settings.scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "valve1") == 0) {
        state.valve1 = !state.valve1;
        settings.scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "valve2") == 0) {
        state.valve2 = !state.valve2;
        settings.scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "scheduler") == 0) {
        settings.scheduler_active = !settings.scheduler_active;
        preferences.begin("garden", false);
        preferences.putBool("scheduler", settings.scheduler_active);
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
        settings.scheduler_active = 0;
        notifyClients();
      }
      if (strcmp(command_item, "calibrate_ph") == 0) {
        int phc = garden_command["value"];
        //calibratePh(phc);
        ph.calibrate(phc);
        settings.ph_calibrated = ph.getCalibrated();
        if(settings.ph_calibrated)
        {
          settings.ph_acidVoltage = ph.getAcidVoltage();
          settings.ph_neutralVoltage = ph.getNeutralVoltage();
          settings.ph_calibrated = ph.getCalibrated();
          preferences.begin("garden", false);
          preferences.putFloat("ph_neutralVoltage", settings.ph_neutralVoltage);
          preferences.putFloat("ph_acidVoltage", settings.ph_acidVoltage);
          preferences.putBool("ph_calibrated", settings.ph_calibrated);
          preferences.end();       
          notifyClients();
        }
      }
      if (strcmp(command_item, "calibrate_tds") == 0) {
        float tdsc = garden_command["value"];
        tds.calibrate(tdsc);
        settings.tds_calibrated = tds.getCalibrated();
        if(settings.tds_calibrated)
        {
          settings.tds_kvalue = tds.getKvalue();
          preferences.begin("garden", false);
          preferences.putFloat("tds_kvalue", settings.tds_kvalue);
          preferences.putBool("tds_calibrated", settings.tds_calibrated);
          preferences.end();
          notifyClients();
        } 
      }       
    }
    if (strcmp(command_type, "update") == 0) {
      if (strcmp(command_item, "light_on") == 0) {
        schedule.light_on = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_on", schedule.light_on);
        preferences.end();
        tLightOn.setInterval(schedule.light_on * TASK_SECOND);
        settings.scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "light_off") == 0) {
        schedule.light_off = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("light_off", schedule.light_off);
        preferences.end();
        tLightOn.setInterval(schedule.light_off * TASK_SECOND);
        settings.scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "spray_period") == 0) {
        schedule.spray_period = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("spray_period", schedule.spray_period);
        preferences.end();
        tPump.setInterval(schedule.spray_period * TASK_MILLISECOND);
        settings.scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "spray_duration") == 0) {
        schedule.spray_duration = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("spray_duration", schedule.spray_duration);
        preferences.end();
        tPumpOff.setInterval(schedule.spray_duration * TASK_MILLISECOND);
        settings.scheduler_active = 1;
        notifyClients();
      }
      if (strcmp(command_item, "fan1_speed") == 0) {
        settings.fan1_speed = garden_command["value"];
        preferences.begin("garden", false);
        preferences.putULong("fan1_speed", settings.fan1_speed);
        preferences.end();
        setFanPwm(FANPWM1_PIN, settings.fan1_speed);
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
    return String(settings.scheduler_active);
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
    return String(settings.fan1_speed);
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
    return String(sensors.ph_analog);
  } else if (var == "PH_CALIBRATED") {
    return String(settings.ph_calibrated);
  } else if (var == "TDS_VALUE") {
    return String(sensors.tds_value);
  } else if (var == "TDS_ANALOG") {
    return String(sensors.tds_analog);
  } else if (var == "TDS_CALIBRATED") {
    return String(settings.tds_calibrated);  
  } else {
    D_PRINTLN("template: Unknown variable");
    return String("  [processor] Unknown variable");
  }
}


//////////////////////////////////////////////
/// Pumps


void setPumpState() {
  digitalWrite(PUMP1_PIN, state.pump1);
  digitalWrite(PUMP2_PIN, state.pump2);
}

void enableSpray() {
  D_PRINT("  [enableSpray] Enabling spray ");
  D_PRINTLN(settings.active_pump);
  if (settings.active_pump == 1) {
    state.pump1 = 0;
  }
  if (settings.active_pump == 2) {
    state.pump2 = 0;
  }
  //state.valve1 = 0;
  tOpenValve1.restartDelayed();
  notifyClients();
  tPumpOff.restartDelayed();
}

void disableSpray() {
  D_PRINT("  [disableSpray] Disabling Spray ");
  D_PRINTLN(settings.active_pump);
  if (settings.active_pump == 1) {
    state.pump1 = 1;
  }
  if (settings.active_pump == 2) {
    state.pump2 = 1;
  }
  state.valve1 = 1;
  notifyClients();
}


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
  ledcWrite(PWM_FANCHANNEL, dutyCycle);
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
  if (currentFlowMillis - previousFlowMillis > sensors.interval) {

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


void readSensors() {
  currentMillis = millis();
  if (currentMillis - previousMillis > sensors.interval) {
    D_PRINTLN("  [readSensors] Reading Sensors");
    
    state.water = getWaterState();
    
    sensors.dallasValueTemp = getTempValue(temp_sensor);

    sensors.dallasValueTemp = getDhtValueTemp(dht);
    sensors.dhtValueHumidity = getDhtValueTemp(dht);

    ph.setTemperature(sensors.dallasValueTemp);
    //ph.update();
    sensors.ph_value = ph.readPH();
    sensors.ph_analog = ph.getVoltage();

    tds.setTemperature(sensors.dallasValueTemp);
    tds.update();
    sensors.tds_value = tds.getTdsValue();
    sensors.tds_analog = tds.getAnalogValue();

    notifyClients();
    previousMillis = millis();
  }
}


//////////////////////////////////////////////
/// Main part

void setup() {

  Serial.begin(115200);
  Serial.println("Starting");

  setupPins();

  // button setup for rf switch
  mySwitch.setBtnCodes(&codes);
  mySwitch.enableTransmit(RF_PIN);

  // // initialize dht
  dht.begin();

  // initialize ph
  ph.setPin(PH_PIN);
  ph.begin();

  tds.setPin(TDS_PIN);
  tds.begin();

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
  //pinMode(FANPWM1_PIN, OUTPUT);
  //analogWriteFrequency(25000);
  ledcSetup(PWM_FANCHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(FANPWM1_PIN, PWM_FANCHANNEL);
  //ledcAttach(FANPWM1_PIN, PWM_FREQ, PWM_RESOLUTION);
  //ledcAttachChannel(FANPWM1_PIN, PWM_FREQ, PWM_RESOLUTION, PWM_FANCHANNEL);

  // setup WiFiManager
  setupWiFiManager();

  // configure preferences structure for persistent saving
  preferences.begin("garden", true);
  
  schedule.spray_period = preferences.getULong("spray_period", SPRAY_PERIOD);
  schedule.spray_duration = preferences.getULong("spray_duration", SPRAY_DURATION);
  schedule.light_on = preferences.getULong("light_on", LIGHT_ON);
  schedule.light_off = preferences.getULong("light_off", LIGHT_OFF);
  schedule.valve1_delay = preferences.getULong("valve1_delay", VALVE1_DELAY);
  
  settings.scheduler_active = preferences.getBool("scheduler", SCHEDULER_ACTIVE);
  settings.ph_neutralVoltage = preferences.getFloat("ph_neutralVoltage", PH_NEUTRALVOLTAGE);
  settings.ph_acidVoltage = preferences.getFloat("ph_acidVoltage", PH_ACIDVOLTAGE);
  settings.ph_calibrated = preferences.getBool("ph_calibrated", false);
  settings.tds_kvalue = preferences.getFloat("tds_kvalue", TDS_KVALUE);
  settings.tds_calibrated = preferences.getBool("tds_calibrated", false);
  
  //ph.setNeutralVoltage(settings.ph_neutralVoltage);
  //ph.setAcidVoltage(settings.ph_acidVoltage);
  //tds.setKvalue(settings.tds_kvalue);
  preferences.end();

  // set calibration state
  ph.setCalibrated(settings.ph_calibrated);
  tds.setCalibrated(settings.tds_calibrated);

  // setup Scheduler intervals for pumps
  tPump.setInterval(schedule.spray_period * TASK_MILLISECOND);
  tPumpOff.setInterval(schedule.spray_duration * TASK_MILLISECOND);

  // setup Scheduler intervals for light
  tLightOn.setInterval(schedule.light_on * TASK_SECOND);
  tLightOff.setInterval(schedule.light_off * TASK_SECOND);

  // setup Scheduler intervals for valves
  tOpenValve1.setInterval(schedule.valve1_delay * TASK_MILLISECOND);

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
  if (settings.scheduler_active == 1) {
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
