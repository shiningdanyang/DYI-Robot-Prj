#include <AFMotor.h>
AF_DCMotor motor_left(4);
AF_DCMotor motor_right(3);

#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
Vector norm;
int offset = 187; //tăng offset->có xu hướng sang trái

#include <SimpleKalmanFilter.h>
SimpleKalmanFilter filter(2, 2, 0.9);
#define trig A1
#define echo A0
#define left_threshold 30

int setpoint = 180;
int currentDirection = 0;

int error,
    P,
    I,
    D,
    PID_value;
float previous_error;
int initial_motor_speed_left = 60;
int initial_motor_speed_right = 60;//75

float Kp = 5, //5
      Ki = 0,
      //Kd = 120;
      Kd = 100;

int distance()
{
  digitalWrite(trig, 0);  // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig, 1);  // phát xung từ chân trig
  delayMicroseconds(10);  // xung có độ dài 10 microSeconds (độ dài xung tiêu chuẩn của SR05)
  digitalWrite(trig, 0);  // tắt chân trig
  int duration = pulseIn(echo, HIGH);
  unsigned int _distance =  int(duration / 2 / 29.412);
  _distance = filter.updateEstimate(_distance);
  //  Serial.println(_distance);
  return (_distance);
}

void calculate_pid()
{
  error =  degree(offset) - setpoint;
  P = error;
  I = I + error;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  //  Serial.println(error);
  previous_error = error;
}

void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed_left + PID_value;
  int right_motor_speed = initial_motor_speed_right - PID_value;

  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  motor_left.run(FORWARD);
  motor_right.run(FORWARD);
  motor_left.setSpeed(left_motor_speed);
  motor_right.setSpeed(right_motor_speed);
  //  Serial.print(" right"); Serial.println(right_motor_speed);
  //  Serial.print("left"); Serial.print(left_motor_speed);
}

int degree(int _offset)
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
  int _degree = ((int)(heading * 180 / M_PI)) - offset;
  Serial.println(_degree);
  if (_degree < 0)
  {
    return (_degree += 360);
  }
  else
  {
    return _degree;
  }
}

void rotaLeft()
{
  motor_left.run(BACKWARD);
  motor_right.run(FORWARD);
  motor_left.setSpeed(60);
  motor_right.setSpeed(60);
}

void stop()
{
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
}

void brake()
{
  motor_left.run(BACKWARD);
  motor_right.run(BACKWARD);
  motor_left.setSpeed(60);
  motor_right.setSpeed(60);
  delay(50);
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);

}
void turnLeft()
{
  if (currentDirection == 0)
  {
    while (degree(offset) < 265)
    {
      rotaLeft();
    }
  }
  stop();
}

void setup()
{
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
  if (distance() > left_threshold)
  {
    brake();
    delay(80);
    motor_left.run(RELEASE);
    motor_right.run(RELEASE);
    delay(1000);
    turnLeft();
  }
  else
  {
    calculate_pid();
    motor_control();
  }

  //  calculate_pid();
  //  motor_control();

}
