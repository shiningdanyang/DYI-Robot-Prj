#include <AFMotor.h>
AF_DCMotor motor_left(4);
AF_DCMotor motor_right(3);

#define front 0
#define left 1
#define back 2
#define right 3

#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
Vector norm;
int offset[] = {173, 299, 25, 102}; //126 274 77 71
int currentDirection = 0;

int turnSpeed = 60;
//float Kp2,
//    Ki2,
//    Kd2,
//    error2,
//    P2, I2, D2, PID_value2;
//float previous_error2;
//int setpoint2;

//void calculate_pid2()
//{
//  P2 = error2;
//  I2 = I2 + error2;
//  D2 = error2 - previous_error2;
//  PID_value2 = (Kp2 * P2) + (Ki2 * I2) + (Kd2 * D2);
//  previous_error2 = error2;
//  turnSpeed = constrain(PID_value2, 0, 255);
//}

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
  //Serial.println(_degree);

  return (_degree);
}

void stop()
{
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
}

void rotaLeft(int _speed)
{
  motor_left.run(BACKWARD);
  motor_right.run(FORWARD);
  motor_left.setSpeed(_speed);
  motor_right.setSpeed(_speed);
}


void turnLeft()
{
  switch (currentDirection)
  {
    case 0:
      {
        int _degree = degree();
        if (offset[0] < offset[1])
        {
          while (_degree < (offset[1] - 10))
          {
            _degree = degree();
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed); //tăng góc yaw
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[1]);
          }
        }
        else  //(offset[0] > offset[1])
        {
          _degree = degree();
          while (_degree < (offset[1] + 360 - 10))
          {
            _degree = degree();
            if (_degree < offset[1])
              _degree += 361;
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed);
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[1] + 360);
          }
        }
        stop();
        currentDirection = 1;
        //        setpoint2 = offset[currentDirection];
        break;
      }
    case 1:
      {
        int _degree = degree();
        if (offset[1] < offset[2])
        {
          while (_degree < (offset[2] - 10))
          {
            _degree = degree();
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed); //tăng góc yaw
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[2]);
          }
        }
        else  //(offset[1] > offset[2])
        {
          while (_degree < (offset[2] + 360 - 10))
          {
            _degree = degree();
            if (_degree < offset[2])
              _degree = _degree + 361;
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed);
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[2] + 360);
          }
        }
        stop();
        currentDirection = 2;
        //        setpoint2 = offset[currentDirection];
        break;
      }
    case 2:
      {
        int _degree = degree();
        if (offset[2] < offset[3])
        {
          while (_degree < (offset[3] - 10))
          {
            _degree = degree();
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed); //tăng góc yaw
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[3]);
          }
        }
        else  //(offset[0] > offset[1])
        {
          while (_degree < (offset[3] + 360 - 10))
          {
            _degree = degree();
            if (_degree < offset[3])
              _degree += 361;
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed);
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[3] + 360);
          }
        }
        stop();
        currentDirection = 3;
        //        setpoint2 = offset[currentDirection];
        break;
      }
    case 3:
      {
        int _degree = degree();
        if (offset[3] < offset[0])
        {
          while (degree() < offset[0] - 10)
          {
            _degree = degree();
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed); //tăng góc yaw
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[0]);
          }
        }
        else  //(offset[0] > offset[1])
        {
          while (_degree < (offset[0] + 360 - 10 ))
          {
            _degree = degree();
            if (_degree < offset[0])
              _degree += 361;
            //            error2 = (_degree - setpoint2);
            //            calculate_pid2();
            rotaLeft(turnSpeed);
            Serial.print(_degree); Serial.print(" "); Serial.println(offset[0] + 360);
          }
        }
        stop();
        currentDirection = 0;
        //        setpoint2 = offset[currentDirection];
        break;
      }
  }
}
void setup()
{
  Wire.begin();
  Serial.begin(9600);
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
  turnLeft();
  delay(1000);
}
