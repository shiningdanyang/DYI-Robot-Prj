#define left 0
#define right 1
#define front 2
// /////////////////////////////////////////////////////////
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter filter(2, 2, 0.99);
// /////////////////////////////////////////////////////////
#include <NewPing.h>
NewPing sonarLeft(TRIG_PINL, ECHO_PINL, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIG_PINR, ECHO_PINR, MAX_DISTANCE);
NewPing sonarFront(TRIG_PINF, ECHO_PINF, 15);
int rawSensor[3];
int oldSensor[3];
int sensor_Wall[3];
int error_Wall[2], P_Wall[2], I_Wall[2], D_Wall[2], PID_Wall[2], previousError_Wall[2];

int readSensors_Wall()
{
  // rawSensor[left]  = sonarLeft.ping_cm(); //ping in cm
  // rawSensor[right] = sonarRight.ping_cm();
  // rawSensor[front] = sonarFront.ping_cm();
  // sensor_Wall[left]  = (rawSensor[left]  + oldSensor[left])  / 2; //average distance between old & new readings to make the change smoother
  // sensor_Wall[right] = (rawSensor[right] + oldSensor[right]) / 2;
  // sensor_Wall[front] = (rawSensor[front] + oldSensor[front]) / 2;
  // oldSensor[left]  = sensor_Wall[left]; // save old readings for movment
  // oldSensor[right] = sensor_Wall[right];
  // oldSensor[front] = sensor_Wall[front];
  // /////////////////////////////////////////////////////////////
  sensor_Wall[left] = sonarLeft.ping_cm();
  sensor_Wall[right] = sonarRight.ping_cm();
  sensor_Wall[front] = sonarFront.ping_cm();
  sensor_Wall[left] = filter.updateEstimate(sensor_Wall[left]);
  sensor_Wall[right] = filter.updateEstimate(sensor_Wall[right]);
  sensor_Wall[front] = filter.updateEstimate(sensor_Wall[front]);
}

void calculatePID_Wall(short _mode) // _mode == leftHand, rightHand
{
  error_Wall[_mode] = sensor_Wall[_mode] - setpoint_Wall[_mode];
  error_Wall[_mode] = constrain(error_Wall[_mode], minError_Wall, maxError_Wall);
  P_Wall[_mode] = error_Wall[_mode];
  I_Wall[_mode] = I_Wall[_mode] + error_Wall[_mode];
  D_Wall[_mode] = error_Wall[_mode] - previousError_Wall[_mode];
  PID_Wall[_mode] = (Kp_Wall[_mode] * P_Wall[_mode]) + (Ki_Wall[_mode] * I_Wall[_mode]) + (Kd_Wall[_mode] * D_Wall[_mode]);
  previousError_Wall[_mode] = error_Wall[_mode];
}

void motorControl_Wall(short _mode)
{
  calculatePID_Wall(_mode);
  int leftSpeed = initSpeed_Wall[left] - pow(-1, _mode) * PID_Wall[_mode];
  int rightSpeed = initSpeed_Wall[right] + pow(-1, _mode) * PID_Wall[_mode];
  leftSpeed = constrain(leftSpeed, minSpeed_Wall, maxSpeed_Wall);
  rightSpeed = constrain(rightSpeed, minSpeed_Wall, maxSpeed_Wall);

  controlled(leftSpeed, rightSpeed);
}

void leftFollow_Wall()
{
  readSensors_Wall();
  if (sensor_Wall[front] > frontThreshold_Wall)
  {
    motorControl_Wall(left);
  }
  else
  {
    rotaSpeed = defaultRotaSpeed;
    while (sensor_Wall[front] < frontThreshold_Wall)
    {
      readSensors_Wall();
      turnRight();
    }
    rotaSpeed = 0;
  }
}

// void rightFollow_Wall()
// {
//   readSensors_Wall();
//   if (sensor_Wall[front] > frontThreshold_Wall)
//   {
//     motorControl_Wall(right);
//   }
//   else
//   {
//     rotaSpeed = defaultRotaSpeed;
//     while (sensor_Wall[front] < frontThreshold_Wall)
//     {
//       readSensors_Wall();
//       turnLeft();
//     }
//     rotaSpeed = 0;
//   }
// }


void rightFollow_Wall()
{
  readSensors_Wall();
  if (sensor_Wall[front] == 0)
  {
    motorControl_Wall(right);
  }
  else
  {
    rotaSpeed = defaultRotaSpeed;
    while (sensor_Wall[front] != 0)
    {
      readSensors_Wall();
      turnLeft();
    }
    rotaSpeed = 0;
  }
}