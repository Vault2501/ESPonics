#ifndef STRUCT
#define STRUCT

struct esp_settings {
  bool scheduler_active;
  int active_pump;
  int fan1_speed;
  bool ph_calibrated;
  bool tds_calibrated;
  float ph_neutralVoltage;
  float ph_acidVoltage;
  float tds_kvalue;
  float flow_calibration;
};

struct esp_state {
  bool pump1;
  bool pump2;
  bool valve1;
  bool valve2;
  bool fan1;
  bool fan2;
  bool light;
  bool water;
};

struct esp_schedule {
  unsigned long spray_period;
  unsigned long spray_duration;
  unsigned long valve1_delay;
  int light_on;
  int light_off;
};

struct esp_sensors {
  float dhtValueTemp;
  float dhtValueHumidity;
  float dallasValueTemp;
  float ph_value;
  float ph_analog;
  float tds_value;
  float tds_analog;
  volatile byte flow_count;
  float flow_rate;
  int interval;
  unsigned int flowMilliLiters;
  unsigned long totalMilliLiters;
};

struct esp_message {
  String log;
  String formattedDate;
  String dayStamp;
  String timeStamp;
};

#endif