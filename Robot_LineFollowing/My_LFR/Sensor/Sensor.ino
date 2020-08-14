byte sensor[5] = {0, 0, 0, 0, 0};
int error = 0;
byte flag_start = 1;
void read_sensor_values()
{
  sensor[0] = digitalRead(2);
  sensor[1] = digitalRead(3);
  sensor[2] = digitalRead(4);
  sensor[3] = digitalRead(5);
  sensor[4] = digitalRead(6);
  
  if     (((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)) && flag_start == 1)
    {
      error = 5;
      flag_start = 0;
    }
  else 

  if      ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))
    error = 5;
  else  if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))
    error = 9;
  else  if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1))
    error = 8;
  else  if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0))
    error = 7;
  else  if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0))
    error = 6;
  else  if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 5;
  else  if((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 4;
  else  if((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 3;
  else  if((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 2;
  else  if((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 1;
  else  if((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    {
      if (error == 1 || error == 0) 
        error = 0;
      else 
      if (error == 9)
        error = 10;
      else
      if (error == 5)
        error = 5;
    }
}

#include <Wire.h>
float i;
void setup() 
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
}

void loop() 
{
  read_sensor_values();
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
//  read_sensor_values();
  Wire.write(error);  // respond with message of 6 bytes
                      // as expected by master
  Serial.println(error);  //Debug
}
