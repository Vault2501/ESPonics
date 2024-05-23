//#define DEBUG_TDS

#ifdef DEBUG_TDS
#define D_TDS_PRINT(x) Serial.print(x)
#define D_TDS_PRINTDEC(x) Serial.print(x, DEC)
#define D_TDS_PRINTLN(x) Serial.println(x)
#else
#define D_TDS_PRINT(x)
#define D_TDS_PRINTDEC(x)
#define D_TDS_PRINTLN(x)
#endif

#ifndef TDS_H
#define TDS_H

#include "Arduino.h"

#define ReceivedBufferLength 15
#define TdsFactor 0.5  // tds = ec / 2

class TDS
{
  public:
    TDS();
    ~TDS();

    void begin();
    void update(); //read and calculate
    void setPin(int pin); 
    void setTemperature(float temp);  //set the temperature and execute temperature compensation
    void setAref(float value);  //reference voltage on ADC, default 5.0V on Arduino UNO
    void setAdcRange(float range);  //1024 for 10bit ADC;4096 for 12bit ADC
    float getKvalue(); 
    int getAnalogValue();
    float getTdsValue();
    float getEcValue();
    void calibrate(float calib);
    void setKvalue(float Kvalue);
    bool isCalibrated();

  private:
    int pin;
    float aref;
    float adcRange;
    float temperature;

    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C 
    int analogValue;
    float voltage;
    float ecValue; //before temperature compensation
    float ecValue25; //after temperature compensation
    float tdsValue;
};


#endif


