#include <AFMotor.h>
#include <Arduino.h>
const int defaultRotaSpeed = 80;
float rotaSpeed;

// #define EN_LEFT 10
// #define IN1_LEFT 13
// #define IN2_LEFT 12
// #define EN_RIGHT 11
// #define IN1_RIGHT 8
// #define IN2_RIGHT 9
// L298N motorLeft(EN_LEFT, IN1_LEFT, IN2_LEFT);
// L298N motorRight(EN_RIGHT, IN1_RIGHT, IN2_RIGHT);

AF_DCMotor motorLeft(4);
AF_DCMotor motorRight(3);

void controlled(int _leftSpeed, int _rightSpeed)
{
	motorLeft.run(FORWARD);
	motorRight.run(FORWARD);
	motorLeft.setSpeed(_leftSpeed);
	motorRight.setSpeed(_rightSpeed);
}

void stop()
{
	motorLeft.run(RELEASE);
	motorRight.run(RELEASE);
}

void runSkip(int _speed, int _time)
{
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(_speed);
  motorRight.setSpeed(_speed);
  delay(_time);
}

void turnRight()
{ //for left hand following
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  rotaSpeed += 1.5;
  motorLeft.setSpeed(rotaSpeed);
  motorRight.setSpeed(rotaSpeed);
}

void turnLeft()
{ //for right hand following
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  rotaSpeed += 1.5;
  motorLeft.setSpeed(rotaSpeed);
  motorRight.setSpeed(rotaSpeed);
}