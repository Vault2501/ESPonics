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

#ifndef PH_H
#define PH_H

#include <Arduino.h>
#include "config.h"

class Ph
{
  public:
    Ph();
    ~Ph();
    void begin();  //initialization
    void setPin(int pin);
    void setTemperature(float temp);
    bool isCalibrated();
    void calibrate(int ph_calib);
    float getPh();
    int getAnalogValue();
    float getCalibB();
    float getCalibM();
    void update();
    void setCalib(float calib_b, float calib_m);

  private:
    void sortArray(int *array, int size);
    int readAnalog(int sampleSize, int pin);
    float analog2Voltage(float analogValue);
    float voltage2Ph(float voltage, float cal_m, float cal_b);
    void setCalibration(float ph1, float ph2, float phAnalog1, float phAnalog2);

    float ph;
    bool calibrated;
    float calibration_b;
    float calibration_m;
    int analog_686;
    int analog_401;
    int analog;
    int pin;
    float temperature;
    int sampleSize;
    float aref;
    float adcRange;
};  

#endif