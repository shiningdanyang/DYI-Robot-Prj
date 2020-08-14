#include <Wire.h>
#include <AFMotor.h>

AF_DCMotor motor_left(1);
AF_DCMotor motor_right(2);
float Kp=15,
      Ki=0.02,
      Kd=1;
int error, 
    P, 
    I, 
    D, 
    PID_value;
float previous_error;
int initial_motor_speed = 100;

void I2C_read()
{
  Wire.requestFrom(8, 1);    // request 1 byte from slave device #8
  while (Wire.available()) 
  { // slave may send less than requested
    error = Wire.read()-5; // receive a byte as character
    //error = Wire.read();
  }
}

void calculate_pid()
{
  P = error;
  I = I + error;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  //Serial.print("error"); 
  Serial.println(error);
  previous_error = error;
}

void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  Serial.print(" left:"); Serial.print(left_motor_speed);
  Serial.print(" right:"); Serial.println(right_motor_speed);
  motor_left.run(FORWARD);
  motor_left.setSpeed(left_motor_speed);
  motor_right.run(FORWARD);
  motor_right.setSpeed(right_motor_speed);
}

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600); //Enable Serial Communications
  motor_right.run(RELEASE);
  motor_left.run(RELEASE);
}

void loop()
{  
  I2C_read();
  calculate_pid();
  motor_control();
}
