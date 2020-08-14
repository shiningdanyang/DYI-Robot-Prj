#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
Vector norm;
//tăng offset->có xu hướng sang trái
int degree()
{
  norm = compass.readNormalize();
  float heading = atan2(norm.YAxis, norm.XAxis);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;
  if (heading < 0)
  {
    heading += 2 * PI;
  }
  else if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }
  int _degree = ((int)(heading * 180 / M_PI));
    Serial.println(_degree);
    return (_degree);  
}

void setup() 
{
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);

  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }
  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
  compass.setSamples(HMC5883L_SAMPLES_8);
  compass.setOffset(0, 0);
}

void loop() 
{

  degree();
}
