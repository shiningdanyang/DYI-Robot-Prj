#include <NewPing.h>
#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define TRIGGER_PINL  9  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PINL     10  // Arduino pin tied to echo pin on ping sensor.
#define TRIGGER_PINR  A3  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PINR     A2  // Arduino pin tied to echo pin on ping sensor.
#define TRIGGER_PINF  A5  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PINF     A4  // Arduino pin tied to echo pin on ping sensor.
NewPing sonarLeft(TRIGGER_PINL, ECHO_PINL, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PINR, ECHO_PINR, MAX_DISTANCE);
NewPing sonarFront(TRIGGER_PINF, ECHO_PINF, MAX_DISTANCE);

#include <AFMotor.h>
AF_DCMotor motorLeft(3);
AF_DCMotor motorRight(4);

////////////////....Sensor//////////////////////////
#define left 0
#define right 1
#define front 2
//float rawSensor[3];
//float oldSensor[3];
//float sensor[3];

int rawSensor[3];
int oldSensor[3];
int sensor[3];
////////////////////////////////////////////////////





/////////////////Environment Variables///////////////
#define maxError 15
#define minError -15//5
#define frontThreshold 10//10//5//9
#define leftThreshold 40
#define rightThreshold 40
//const int defaultRotaSpeed = 30;
#define defaultRotaSpeed 80
float rotaSpeed;
int brakeTime = 40;
int _speedSkip1 = 200;
int _speedSkip2 = 200;
int _timeSkip1 = 1000;
int _timeSkip2 = 200;
int _left90 = 600;
int _right90 = 600;
/////////////////////////////////////////////////////



///////////////System Variable////////////////////////
#define leftHand 0
#define rightHand 1
const int setpoint[2] = {10, 10};//7//9
const int initSpeed[2] = {180, 180}; //230//180//210//140
const float Kp[2] = {20, 20},//30//20,20
                    Ki[2] = {0 , 0 },
                            Kd[2] = {800, 800};//bám trái ->>>>>800 ok  //runskip scanning
//const float KiTurn = 0.2;
int error[2], P[2], I[2], D[2], PID[2], previousError[2];
int PIDTurn;
#define minSpeed 0// 50 ok//40//20//20//35//25//15
#define maxSpeed 255// 90 ok//80 ok//180//230//200//230
//////////////////////////////////////////////////////



///////////////////////Function////////////////////
int readSensors()
{
  rawSensor[left]  = sonarLeft.ping_cm(); //ping in cm
  rawSensor[right] = sonarRight.ping_cm();
  rawSensor[front] = sonarFront.ping_cm();

  sensor[left]  = (rawSensor[left]  + oldSensor[left])  / 2; //average distance between old & new readings to make the change smoother
  sensor[right] = (rawSensor[right] + oldSensor[right]) / 2;
  sensor[front] = (rawSensor[front] + oldSensor[front]) / 2;

  oldSensor[left]  = sensor[left]; // save old readings for movment
  oldSensor[right] = sensor[right];
  oldSensor[front] = sensor[front];
}

void calculatePID(short _mode)  //_mode == leftHand, rightHand
{
  error[_mode] = sensor[_mode] - setpoint[_mode];
  error[_mode] = constrain(error[_mode], minError, maxError);
  P[_mode] = error[_mode];
  I[_mode] = I[_mode] + error[_mode];
  D[_mode] = error[_mode] - previousError[_mode];
  PID[_mode] = (Kp[_mode] * P[_mode]) + (Ki[_mode] * I[_mode]) + (Kd[_mode] * D[_mode]);
  previousError[_mode] = error[_mode];
}
void motorControl(short _mode)
{
  // Calculating the effective motor speed
  //  int leftSpeed = initSpeed[left] - PID[_mode];
  //  int rightSpeed = initSpeed[right] + PID[_mode];
  int leftSpeed = initSpeed[left] - pow(-1, _mode) * PID[_mode];
  int rightSpeed = initSpeed[right] + pow(-1, _mode) * PID[_mode];
  leftSpeed = constrain(leftSpeed, minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(leftSpeed);
  motorRight.setSpeed(rightSpeed);
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

void stop()
{
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}

void brake(int _time)
{
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  motorLeft.setSpeed(50);
  motorRight.setSpeed(50);
  delay(_time);
}

void runSkip(int _speed, int _time)
{
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(_speed);
  motorRight.setSpeed(_speed);
  delay(_time);
}

void rotateLeft(int _speed, int _time)
{
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(_speed);
  motorRight.setSpeed(_speed);
  delay(_time);
}

void printValue()
{
  Serial.print(sensor[left]);
  Serial.print(" ");
  Serial.print(sensor[right]);
  Serial.print(" ");
  Serial.println(sensor[front]);
}
/////////////////////////////////////////////////////


void turnLeft2(int _time)
{
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  motorLeft.setSpeed(defaultRotaSpeed);
  motorRight.setSpeed(defaultRotaSpeed);
  delay(_time);
  stop();
}

void turnRight2(int _time)
{
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  motorLeft.setSpeed(defaultRotaSpeed);
  motorRight.setSpeed(defaultRotaSpeed);
  delay(_time);
  stop();
}


void setup()
{
  Serial.begin(9600); //Enable Serial Communications
  stop();
  //delay(2000);
  runSkip(140,200);
}
int timeStart;
void loop()
{
  timeStart = millis();
  while (millis() - timeStart < 2300)
  {
    /////////////////////Left Hand//////////////////////////////////
    readSensors();
    printValue(); //debug
    if (sensor[front] > frontThreshold)
    {
      calculatePID(leftHand);
      motorControl(leftHand);
    }
    else
    {
      rotaSpeed = defaultRotaSpeed;
      while (sensor[front] < frontThreshold)
      {
        readSensors();
        turnRight();
      }
    }
  }

  timeStart = millis();
  while (millis() - timeStart < 500)
  {
    /////////////////////Left Hand//////////////////////////////////
    readSensors();
    calculatePID(rightHand);
    motorControl(rightHand);
  }

  readSensors();
  while (sensor[front] > frontThreshold)
  {
    motorLeft.run(FORWARD);
    motorRight.run(FORWARD);
    motorLeft.setSpeed(140);//160
    motorRight.setSpeed(255);
    readSensors();
  }

//stop();
//while(1);
  timeStart = millis();
  while (millis() - timeStart < 2000)
  {
    /////////////////////Left Hand//////////////////////////////////
    readSensors();
    printValue(); //debug
    if (sensor[front] > frontThreshold)
    {
      calculatePID(rightHand);
      motorControl(rightHand);
    }
    else
    {
      rotaSpeed = defaultRotaSpeed;
      while (sensor[front] < frontThreshold)
      {
        readSensors();
        turnLeft();
      }
    }
  }

  timeStart = millis();
  while (millis() - timeStart < 2000)
  {
    readSensors();
    calculatePID(leftHand);
    motorControl(leftHand);
  }

  timeStart = millis();
 // minSpeed = 0;
//  maxSpeed = 255;
  while (1)
  {
    /////////////////////Left Hand//////////////////////////////////
    readSensors();
    if(sensor[right]==0) sensor[right] = 30;/////////////////////
    printValue(); //debug
    if (sensor[front] > frontThreshold)
    {
      calculatePID(rightHand);
      motorControl(rightHand);
    }
    else
    {
      rotaSpeed = defaultRotaSpeed;
      while (sensor[front] < frontThreshold)
      {
        readSensors();
        turnLeft();
      }
    }
  }

  stop();
  while (1);
}
