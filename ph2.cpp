#include "ph2.h"

PH::PH()
{
  this->_pin		        = PH_PIN;
  this->_temperature    = 25.0;
  this->_phValue        = 7.0;
  this->_acidVoltage    = 2032.44;    //buffer solution 4.0 at 25C
  this->_neutralVoltage = 1500.0;     //buffer solution 7.0 at 25C
  this->_voltage        = 1500.0;
  this->_aref 	        = AREF;
  this->_adcRange 	    = ADCRANGE;
  this->_sampleSize 	  = PH_SAMPLESIZE;
}

PH::~PH()
{

}

void PH::begin()
{
  pinMode(this->_pin,INPUT);
}

void PH::readVoltage()
{
  //this->_voltage = analogPHRead(this->_sampleSize)/this->_adcRange*(this->_aref*1000);
  int analog = analogRead(this->_pin);
  D_PH_PRINT("  [Ph::readVoltage analog]: ");
  D_PH_PRINTLN(analog);
  //this->_voltage = analog / this->_adcRange * (this->_aref*1000);
  this->_voltage = analog / 4096.0 * 5000;
}

float PH::readPH()
{
  readVoltage();
  float slope = (7.0-4.0)/((this->_neutralVoltage-1500.0)/3.0 - (this->_acidVoltage-1500.0)/3.0);  // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
  float intercept =  7.0 - slope*(this->_neutralVoltage-1500.0)/3.0;
  //Serial.print("slope:");
  //Serial.print(slope);
  //Serial.print(",intercept:");
  //Serial.println(intercept);
  this->_phValue = slope*(this->_voltage-1500.0)/3.0+intercept;  //y = k*x + b
  D_PH_PRINT("  [Ph::update voltage]: ");
  D_PH_PRINTLN(this->_voltage);
  D_PH_PRINT("  [Ph::update ph]: ");
  D_PH_PRINTLN(this->_phValue);
  return _phValue;
}

void PH::calibrate(int ph_calib)
{
  readVoltage();
  if (ph_calib == 7)
  {
    Serial.println();
    Serial.print(F(">>>Buffer Solution:7.0"));
    this->_neutralVoltage =  this->_voltage;
  }
  else if (ph_calib == 4)
  {
    Serial.println();
    Serial.print(F(">>>Buffer Solution:4.0"));
    this->_acidVoltage =  this->_voltage;
  } 
  else
  {
    Serial.print("Unknown ph calibration value: ");
    Serial.println(ph_calib);
  }
}

int PH::getVoltage()
{
  this->readVoltage();
  return this->_voltage;
}

float PH::getAcidVoltage()
{
  return this->_acidVoltage;;
}

float PH::getNeutralVoltage() 
{
  return this->_neutralVoltage;
}

void PH::setPin(int pin)
{
  this->_pin = pin;
}

void PH::setCalib(int acidVoltage, int neutralVoltage) {
  this->_acidVoltage = acidVoltage;
  this->_neutralVoltage = neutralVoltage;
}

void PH::setTemperature(float temp)
{
  this->_temperature = temp;
}

// int PH::analogPHRead(int sampleSize)
// {
//   // read sampleSize amount of values
//   int buf[sampleSize];
//   int sumValue = 0;
//   int avgValue = 0;
//   for (int i = 0; i < sampleSize; i++) {
//     for (int i = 0; i < sampleSize; i++) {
//       buf[i] = analogRead(this->_pin);
//       delay(10);
//     }
//   }

//   this->sortArray(buf, sampleSize);

//   // ignore the lowest and highest 20% of the sample
//   int ignore = (int((sampleSize * 20) / 100));
//   for (int i = ignore; i < (sampleSize - ignore); i++) {
//     sumValue += buf[i];
//   }
//   avgValue = sumValue / (sampleSize - (2 * ignore));

//   return avgValue;
// }

// void PH::sortArray(int *array, int size) {
//   int temp;
//   for (int i = 0; i < (size - 1); i++) {
//     for (int j = i + 1; j < size; j++) {
//       if (array[i] > array[j]) {
//         temp = array[i];
//         array[i] = array[j];
//         array[j] = temp;
//       }
//     }
//   }
// }
