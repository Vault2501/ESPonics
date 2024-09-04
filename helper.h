#include "Arduino.h"
#include "config.h"

#define DEBUG_HELPER

#ifdef DEBUG_HELPER
#define D_HELPER_PRINT(x) Serial.print(x)
#define D_HELPER_PRINTDEC(x) Serial.print(x, DEC)
#define D_HELPER_PRINTLN(x) Serial.println(x)
#else
#define D_HELPER_PRINT(x)
#define D_HELPER_PRINTDEC(x)
#define D_HELPER_PRINTLN(x)
#endif

String updateTime();
String updateDate();
void setupPins();