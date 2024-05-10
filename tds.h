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
    float getTdsValue();
    float getEcValue();
    void calibrate(int calib);
    void setCalib(float kValue);
    bool isCalibrated();

  private:
    int pin;
    float aref;  // default 5.0V on Arduino UNO
    float adcRange;
    float temperature;

    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C 
    float analogValue;
    float voltage;
    float ecValue; //before temperature compensation
    float ecValue25; //after temperature compensation
    float tdsValue;
};


#endif


