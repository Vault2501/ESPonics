#include "ph.h"

Ph::Ph()
{
  this->pin = PH_PIN;
  this->temperature = 25.0;
  this->analog = 0;
  this->analog_686 = -1;
  this->analog_401 = -1;
  this->calibrated = PH_CALIBRATED;
  this->calibration_b = PH_CALIBRATION_B;
  this->calibration_m = PH_CALIBRATION_M;
  this->ph = 7;
  this->sampleSize = PH_SAMPLESIZE;
}

Ph::~Ph()
{
}

void Ph::begin()
{
  pinMode(this->pin,INPUT);
}

void Ph::setPin(int pin)
{
	this->pin = pin;
}

void Ph::setTemperature(float temp)
{
	this->temperature = temp;
}

float Ph::analog2Voltage(float analogValue) {
  float voltage = (float)analogValue * (3.3 / 4095.0);
  return voltage;
}

float Ph::voltage2Ph(float voltage, float cal_m, float cal_b) {
  float phValue = cal_m * voltage + cal_b;
  return phValue;
}

bool Ph::isCalibrated() {
  if (this->calibration_m != 1 && this->calibration_b != 0) {
    return true;
  } else {
    return false;
  }
}

void Ph::update() {
  this->analog = this->readAnalog(this->sampleSize, this->pin);
  float ph_voltage = this->analog2Voltage(this->analog);
  this->ph = this->voltage2Ph(ph_voltage, this->calibration_m, this->calibration_b);

  D_PH_PRINT("  [Ph::update analog]: ");
  D_PH_PRINTLN(analog);
  D_PH_PRINT("  [Ph::update voltage]: ");
  D_PH_PRINTLN(ph_voltage);
  D_PH_PRINT("  [Ph::update ph]: ");
  D_PH_PRINTLN(ph);
}

float Ph::getPh() {
  return this->ph;
}

int Ph::getAnalogValue() {
  return this->analog;
}

float Ph::getCalibB() {
  return this->calibration_b;
}
    
float Ph::getCalibM() {
  return this->calibration_m;
}

void Ph::setCalib(float calib_b, float calib_m) {
  this->calibration_b = calib_b;
  this->calibration_m = calib_m;
}

int Ph::readAnalog(int sampleSize, int pin) {
  // read sampleSize amount of values
  int buf[this->sampleSize];
  int sumValue = 0;
  int avgValue = 0;
  for (int i = 0; i < this->sampleSize; i++) {
    for (int i = 0; i < this->sampleSize; i++) {
      buf[i] = analogRead(this->pin);
      delay(10);
    }
  }

  this->sortArray(buf, sampleSize);

  // ignore the lowest and highest 20% of the sample
  int ignore = (int((this->sampleSize * 20) / 100));
  for (int i = ignore; i < (this->sampleSize - ignore); i++) {
    sumValue += buf[i];
  }
  avgValue = sumValue / (this->sampleSize - (2 * ignore));

  return avgValue;
}

void Ph::setCalibration(float ph1, float ph2, float phAnalog1, float phAnalog2) {
  this->calibration_m = (ph2 - ph1) / (phAnalog2 - phAnalog1);
  this->calibration_b = ph1 - (this->calibration_m * phAnalog1);
  D_PH_PRINT("  [Ph::calcCalibration] this->calibration_b: ");
  D_PH_PRINTLN(this->calibration_b);
  D_PH_PRINT("  [Ph::calcCalibration] this->calibration_m: ");
  D_PH_PRINTLN(this->calibration_m);
}

void Ph::calibrate(int ph_calib) {
  if (ph_calib == 401) {
    this->analog_401 = this->readAnalog(this->sampleSize, this->pin);
    D_PH_PRINT("  [Ph::calibratePh] this->analog_401: ");
    D_PH_PRINTLN(this->analog_401);
  } else if (ph_calib == 686) {
    this->analog_686 = this->readAnalog(this->sampleSize, this->pin);
    D_PH_PRINT("  [Ph::calibratePh] this->analog_686: ");
    D_PH_PRINTLN(this->analog_686);
  } else {
    Serial.print("Unknown ph calibration value: ");
    Serial.println(ph_calib);
  }
  if (this->analog_401 != -1 && this->analog_686 != -1) {
    this->setCalibration(4.01, 6.86, this->analog_401, this->analog_686);
    //preferences.begin("garden", false);
    //preferences.putULong("ph_calibration_b", settings.ph_calibration_b);
    //preferences.putULong("ph_calibration_m", settings.ph_calibration_m);
    //preferences.end();
    this->analog_401 = -1;
    this->analog_686 = -1;
  }
}

void Ph::sortArray(int *array, int size) {
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

