#include "tds.h"
#include "config.h"

TDS::TDS()
{
    this->pin = TDS_PIN;
    this->temperature = 25.0;
    this->aref = TDS_AREF;
    this->adcRange = TDS_ADCRANGE;
    this->kValue = 1.0;
}

TDS::~TDS()
{
}

void TDS::setPin(int pin)
{
  D_TDS_PRINT("  [TDS::setPin] pin set to: ");
  D_TDS_PRINTLN(pin);
	this->pin = pin;
}

void TDS::setTemperature(float temp)
{
	this->temperature = temp;
}

void TDS::setAref(float value)
{
	this->aref = value;
}

void TDS::setAdcRange(float range)
{
  this->adcRange = range;
}

void TDS::begin()
{
	pinMode(this->pin,INPUT);
  D_TDS_PRINT("  [TDS::begin] Set pin to INPUT: ");
  D_TDS_PRINTLN(pin);
}

float TDS::getKvalue()
{
	return this->kValue;
}

void TDS::update()
{
	this->analogValue = analogRead(this->pin);
	this->voltage = (float)this->analogValue/this->adcRange*this->aref;
	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage)*this->kValue;
	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
	this->tdsValue = ecValue25 * TdsFactor;


  D_TDS_PRINT("  [TDS::update analogValue]: ");
  D_TDS_PRINTLN(analogValue);
  D_TDS_PRINT("  [TDS::update voltage]: ");
  D_TDS_PRINTLN(voltage);
  D_TDS_PRINT("  [TDS::update tdsValue]: ");
  D_TDS_PRINTLN(tdsValue);
}

float TDS::getTdsValue()
{
	return tdsValue;
}

float TDS::getEcValue()
{
  return ecValue25;
}

int TDS::getAnalogValue()
{
  return analogValue;
}

void TDS::setKvalue(float KValue)
{
  this->kValue=KValue;
}

bool TDS::isCalibrated()
{
  if(kValue == 1.0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void TDS::calibrate(float calib)
{
  float rawECsolution;

  rawECsolution = calib/(float)(TdsFactor);
  this->update();
  kValue = rawECsolution/(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage);
  D_TDS_PRINT("  [TDS::calibrate kValue]: ");
  D_TDS_PRINTLN(kValue);
}