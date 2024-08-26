#define DEBUG_PH

#ifdef DEBUG_PH
#define D_PH_PRINT(x) Serial.print(x)
#define D_PH_PRINTDEC(x) Serial.print(x, DEC)
#define D_PH_PRINTLN(x) Serial.println(x)
#else
#define D_PH_PRINT(x)
#define D_PH_PRINTDEC(x)
#define D_PH_PRINTLN(x)
#endif


#ifndef _PH_H_
#define _PH_H_

#include "Arduino.h"
#include "config.h"

class PH
{
public:
  PH(String* logPtr);
  ~PH();
  void  calibrate();
  float readPH(); 
  void  begin();
  void  readVoltage();
  int   analogPHRead(int sampleSize);
  int   getVoltage();
  float getAcidVoltage();
  float getNeutralVoltage();
  bool  getCalibrated();
  void  setAcidVoltage(int voltage);
  void  setNeutralVoltage(int voltage);
  void  setCalibrated(bool state);
  void  setTemperature(float temp);
  void  sortArray(int *array, int size);
  void  setPin(int pin);

private:
  int 	_pin;
  float _phValue;
  float _acidVoltage;
  float _neutralVoltage;
  float _voltage;
  float _temperature;
  float _aref;
  float _adcRange;
  int   _sampleSize;
  bool  _calibrated;
  bool  _acidCalibrated;
  bool  _neutralCalibrated;
  String* _logPtr;
};

#endif
