const int analogInPin = A0; 
const int sampleSize = 10; 

int sensorValue = 0;
unsigned long int sumValue; 
float avgValue;
int buf[sampleSize],temp;

float calibration_m = -5.70;
float calibration_b = 21.34;

void sortArray(int* array, int size) {
  for(int i=0;i<(size-1);i++)
  {
    for(int j=i+1;j<size;j++)
    {
      if(array[i]>array[j])
      {
        temp=array[i];
        array[i]=array[j];
        array[j]=temp;
      }
    }
  }
}

void calibrate(float ph) {

  Serial.print("Please put Sensor in calibration liquid with pH ");
  Serial.print(ph);
  Serial.println(" and hit any key to start calibration");
  while (Serial.available() <= 0) {}
  float phAnalog = readPhAnalog();

  return phAnalog;
}

void calcCalibration(float ph1, float ph2, float phAnalog1, float phAnalog2) {
  float calibrate_m = (pHValue2 - pHValue1) / (phAnalog2 - phAnalog1);
  float calibrate_b = pH1 - (calibrate_m * phAnalog1);
  Serial.print("Calculated calibrate_b: ");
  Serial.println(calibrate_b);
  Serial.print("Calculated calibrate_b: ");
  Serial.println(calibrate_b);
}

int readPhAnalog() {
  // read sampleSize amount of values
  for(int i=0;i<sampleSize;i++) 
  { 
    buf[i]=analogRead(analogInPin);
    Serial.print("buf[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(buf[i]);
    delay(10);
  }

  sortArray(buf, sampleSize);

  for(int i=0;i<sampleSize;i++) 
  { 
    Serial.print("buf[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(buf[i]);
    delay(10);
  }

  

  // ignore the lowest and highest 20% of the sample
  int ignore=(int((sampleSize*20)/100));

  sumValue=0;
  for(int i=ignore;i<(sampleSize-ignore);i++){
    sumValue+=buf[i];
  }
  avgValue = sumValue/(sampleSize-(2*ignore));
  Serial.print("avgValue = ");
  Serial.println(avgValue);

  return avgValue;

  //float pHVol=(float)avgValue*(3.3/4095.0);
  //float phValue = -5.70 * pHVol + 21.34;
  //Serial.print("sensor = ");
  //Serial.println(phValue);
}

float analog2Voltage(float analogValue) {
  float voltage=(float)analogValue*(3.3/4095.0);
  return voltage;
}

float voltage2Ph(float voltage, float cal_m, float cal_b) {
  float phValue = cal_m * voltage + cal_b;
  return phValue;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int phAnalog = readPhAnalog();
  float pHVoltage = analog2Voltage(phAnalog);
  float phValue = voltage2Ph(pHVoltage, calibration_m, calibration_b);
  Serial.print("sensor = ");
  Serial.println(phValue);
  delay(10000);
}
