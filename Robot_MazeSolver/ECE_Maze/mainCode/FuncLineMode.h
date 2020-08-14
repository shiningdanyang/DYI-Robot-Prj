#include <Arduino.h>
#define left 0
#define right 1

float error_Line = 0, P_Line = 0, I_Line = 0, D_Line = 0, PID_Line = 0, previousError_Line = 0;
int sensor_Line[5] =
{
  0, 0, 0, 0, 0
};

void readSensors_Line()
{
  sensor_Line[0] = digitalRead(LEFT_FAR);
  sensor_Line[1] = digitalRead(LEFT_NEAR);
  sensor_Line[2] = digitalRead(CENTER);
  sensor_Line[3] = digitalRead(RIGHT_NEAR);
  sensor_Line[4] = digitalRead(RIGHT_FAR);
  if ((sensor_Line[0] == 0) && (sensor_Line[1] == 0) && (sensor_Line[2] == 0) && (sensor_Line[3] == 0) && (sensor_Line[4] == 1))
    error_Line = 4;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 0) && (sensor_Line[2] == 0) && (sensor_Line[3] == 1) && (sensor_Line[4] == 1))
      error_Line = 3;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 0) && (sensor_Line[2] == 0) && (sensor_Line[3] == 1) && (sensor_Line[4] == 0))
      error_Line = 2;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 0) && (sensor_Line[2] == 1) && (sensor_Line[3] == 1) && (sensor_Line[4] == 0))
      error_Line = 1;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 0) && (sensor_Line[2] == 1) && (sensor_Line[3] == 0) && (sensor_Line[4] == 0))
      error_Line = 0;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 1) && (sensor_Line[2] == 1) && (sensor_Line[3] == 0) && (sensor_Line[4] == 0))
      error_Line = -1;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 1) && (sensor_Line[2] == 0) && (sensor_Line[3] == 0) && (sensor_Line[4] == 0))
      error_Line = -2;
  else
    if ((sensor_Line[0] == 1) && (sensor_Line[1] == 1) && (sensor_Line[2] == 0) && (sensor_Line[3] == 0) && (sensor_Line[4] == 0))
      error_Line = -3;
  else
    if ((sensor_Line[0] == 1) && (sensor_Line[1] == 0) && (sensor_Line[2] == 0) && (sensor_Line[3] == 0) && (sensor_Line[4] == 0))
      error_Line = -4;
  else
    if ((sensor_Line[0] == 0) && (sensor_Line[1] == 0) && (sensor_Line[2] == 0) && (sensor_Line[3] == 0) && (sensor_Line[4] == 0))
    {
      if (error_Line == -4)
        error_Line = -5;
      else
        if (error_Line == 4)
          error_Line = 5;
        else
          if(error_Line == 0)
            error_Line = 0;
    }
}

void calculatePID_Line()
{
  P_Line = error_Line;
  I_Line = I_Line + error_Line;
  D_Line = error_Line - previousError_Line;
  PID_Line = (Kp_Line * P_Line) + (Ki_Line * I_Line) + (Kd_Line * D_Line);
  previousError_Line = error_Line;
}

void motorControl_Line()
{
  // Calculating the effective motor speed:
  calculatePID_Line();
  int leftSpeed = initSpeed_Line[left] - PID_Line;
  int rightSpeed = initSpeed_Line[right] + PID_Line;
  // The motor speed should not exceed the max PWM value
  constrain(leftSpeed, minSpeed_Line, maxSpeed_Line);
  constrain(rightSpeed, minSpeed_Line, maxSpeed_Line);
 
  controlled(leftSpeed, rightSpeed);

}
  