#include <SimpleKalmanFilter.h>

SimpleKalmanFilter filter(2,2,0.05);


#define RIGHT_HAND
#include <AFMotor.h>
AF_DCMotor motorLeft(4);
AF_DCMotor motorRight(3);

float rightKp = 8,
      rightKd = 7,//1.1
      rightKi = 0.1; //float Kp = 15 , Kd =0.75 , Ki = 0;

float leftKp = 13,
      leftKd = 3,//1.1
      leftKi = 0.1; //float Kp = 15 , Kd =0.75 , Ki = 0;

int maxSpeed = 100;

int frontThreshold = 400;
int leftSetpoint = 420;
int rightSetpoint = 700;

int initSpeedLeft = 40;
int initSpeedRight = 40;
int turnSpeed = 75;

int leftValue;
int frontValue;
int rightValue;

float leftSpeed, rightSpeed;

void readSensor()
{
  rightValue = filter.updateEstimate(analogRead(A3));
  leftValue = filter.updateEstimate(analogRead(A4));
  frontValue = filter.updateEstimate(analogRead(A5));

  Serial.print(" left "); //70
  Serial.print(leftValue);
  Serial.print(" front "); //50
  Serial.print(frontValue);
  Serial.print(" right "); //40
  Serial.println(rightValue);
}

void turnLeft()
{
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  motorLeft.setSpeed(20); //110   // giá trị đo thực tế
  motorRight.setSpeed(20);
  delay(50);

  while (frontValue < frontThreshold)
  {
    motorRight.run(FORWARD);
    motorLeft.run(BACKWARD);
    motorLeft.setSpeed(turnSpeed);
    motorRight.setSpeed(turnSpeed);
    readSensor();
  }
}

void turnRight()
{
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  motorLeft.setSpeed(20); //110   // giá trị đo thực tế
  motorRight.setSpeed(20);
  delay(10);

  while (frontValue < frontThreshold)
  {
    motorLeft.run(FORWARD);
    motorRight.run(BACKWARD);
    motorLeft.setSpeed(turnSpeed); //110   // giá trị đo thực tế
    motorRight.setSpeed(turnSpeed);
    readSensor();
  }
}

void rightHand() // bám phải trái cộng, phải trừ
{
  float P, I, D;
  static int previousError;
  int PID_value;
  float samplingTime = 0.01;
  int error = rightSetpoint - rightValue;//677
  P = rightKp * error;
  I = rightKi * (I + error) * samplingTime;
  D = (rightKd * (error - previousError )) / samplingTime;
  PID_value = P + I + D;
  previousError = error;
  leftSpeed = initSpeedLeft - PID_value; //leftSpeed = 50 - PID_value;
  rightSpeed = initSpeedRight + PID_value; //rightSpeed = 50 + PID_value;
  leftSpeed = constrain(leftSpeed, 0, maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, maxSpeed);
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(leftSpeed);//100
  motorRight.setSpeed(rightSpeed);//100
}

void leftHand() // bám trái là trái trừ,phải cộng (leftSpeed,rightSpeed)
{
  float P, I, D;
  static int previousError;
  int PID_value;
  float samplingTime = 0.01;
  int error = leftSetpoint - leftValue;
  P = leftKp * error ;
  I = leftKi * (error + I) * samplingTime;
  D = (leftKd * (error - previousError )) / samplingTime;
  PID_value = P + I + D;
  previousError = error;
  int rightSpeed = initSpeedRight - PID_value; // rightSpeed = 50 - PID_value;
  int leftSpeed = initSpeedLeft + PID_value; // leftSpeed = 50 + PID_value;

  leftSpeed = constrain(leftSpeed, 0, maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, maxSpeed);

  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(leftSpeed);//100
  motorRight.setSpeed(rightSpeed);//100
}

void setup()
{
  Serial.begin(9600);
}

#ifdef RIGHT_HAND
void loop()
{
  readSensor();
  if (frontValue < frontThreshold)
  {
    turnLeft();
  }
  else
    rightHand();
}
#endif

#ifdef LEFT_HAND
void loop()
{
  readSensor();
  if (frontValue < frontThreshold)
  {
    turnRight);
  }
  else
    leftHand();
}
#endif
