#include <AFMotor.h>
#define left 0
#define front 1
#define right 2

#define leapTime 200
#define turnAroundDelay 200

#define thresholdLeft 20
#define thresholdRight 20
#define thresholdFront 8

#define whiteSensor 2

#define setpointLeft 12

int trig[3] = {A5, A3, A1};
int echo[3] = {A4, A2, A0};

int leftReading;
int frontReading;
int rightReading;
int whiteReading;

int replaystage;

char path[30] = {};
int pathLength;
int readLength;

float Kp=4,//5
      Ki=0,
      Kd=8;
int error, 
    P, 
    I, 
    D, 
    PID_value;
float previous_error;
int initial_motor_speed_left = 100;
int initial_motor_speed_right = 120;

AF_DCMotor motor_left(2);
AF_DCMotor motor_right(1);

int distance(int pos)
{
    digitalWrite(trig[pos],0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig[pos],1);   // phát xung từ chân trig
    delayMicroseconds(10);  // xung có độ dài 10 microSeconds (độ dài xung tiêu chuẩn của SR05)
    digitalWrite(trig[pos],0);   // tắt chân trig
    int duration = pulseIn(echo[pos],HIGH);  
    Serial.println(int(duration/2/29.412));
    return int(duration/2/29.412);
}
void readSensors()
{
  leftReading = distance(left);
  frontReading = distance(front);
  rightReading = distance(right);
  whiteReading = digitalRead(whiteSensor);
}



void setup()
{
  pinMode(A5,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A4,INPUT);
  pinMode(A2,INPUT);
  pinMode(A0,INPUT);
  pinMode(whiteSensor,INPUT);
  delay(1000);
}

void loop()
{
  readSensors();
  if(leftReading<thresholdLeft && frontReading>thresholdFront)
  {
    straight();
  }
  else
  {
    leftHandWall();
  }
}

void leftHandWall()
{
  if(leftReading>thresholdLeft && rightReading>thresholdRight)  //gặp ngã 3 chữ T hoặc ngã tư chữ T
  {
    run_straight(leapTime);
    readSensors();
    if(whiteReading == HIGH)
    {
      done();
    }
    else
    {
      turnLeft();
    }
  }

  //else
  if(leftReading>thresholdLeft) //nếu gặp ngã rẽ bên trái
  {
    run_straight(leapTime);
    readSensors();
    if(whiteReading == HIGH)
    {
      done();
    }
    else
    {
      turnLeft();
    }
  }
  
  else  //nếu như bên trái không có ngã rẽ nhưng bên phải có ngã rẽ
  if(rightReading>thresholdRight)
  {
    run_straight(leapTime);
    readSensors();
    if(leftReading>thresholdLeft)
    {
      delay(leapTime-30);
      readSensors();
      if(whiteReading == HIGH)
      {
        done();
      }
      else
      {
        turnLeft();
        return;
      }
    }
    
    delay(leapTime - 30);
    readSensors();
    if(whiteReading == HIGH)
    {
      done();
    }
    else
    {
      turnRight();
      return;
    }
    path[pathLength] = 'S';
    pathLength++;
    if(path[pathLength - 2] == 'B')
    {
      shortPath();
    }
    straight();
  }
  
  readSensors();
  if(leftReading<thresholdLeft && frontReading<thresholdFront && rightReading<thresholdRight)
  turnAround();
}

void done()
{
  //tắt tất cả động cơ
  replaystage = 1;
  path[pathLength] = 'D';
  pathLength++;
  while (digitalRead(whiteSensor)==HIGH);
  delay(500);
  replay();
}

void turnLeft()
{
  //phải tiến, trái lui 
  delay(20);
  if(replaystage == 0)
  {
    path[pathLength] = 'L';
    pathLength++;
    if(path[pathLength - 2] = 'B')
    {
      shortPath();
    }
  }
}

void turnRight()
{
  //trái tiến phải lui
  delay(90);
  if(replaystage == 0)
  {
    path[pathLength] = 'R';
    pathLength++;
    if(path[pathLength - 2] == 'B')
    {
      shortPath();
    }
  }
}

void straight()
{
  calculate_PID();
  motor_control();
}

void calculate_PID()
{
  error =  leftReading - setpointLeft;
  if(error >= setpointLeft) error = setpointLeft;
  else if(error <= -setpointLeft) error = -setpointLeft;
    
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
  int left_motor_speed = initial_motor_speed_left - PID_value;
  int right_motor_speed = initial_motor_speed_right + PID_value;

  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  Serial.print("left"); Serial.print(left_motor_speed); Serial.print("    right"); Serial.println(right_motor_speed);

  motor_left.run(FORWARD);
  motor_left.setSpeed(left_motor_speed);
  motor_right.run(FORWARD);
  motor_right.setSpeed(right_motor_speed);
}


void run_straight(int _time)
{
  motor_left.run(FORWARD);
  motor_right.run(FORWARD);
  motor_left.setSpeed(initial_motor_speed_left);
  motor_right.setSpeed(initial_motor_speed_right);
  delay(_time);
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
}

void turnAround()
{
  motor_left.run(FORWARD);
  motor_right.run(BACKWARD);
  motor_left.setSpeed(initial_motor_speed_left);
  motor_right.setSpeed(initial_motor_speed_right);
  delay(turnAroundDelay);
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
  
  path[pathLength] = 'B';
  pathLength++;
  straight();
}

void shortPath() 
{
  int shortDone = 0;
  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'R') 
  {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test1");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'S' && shortDone == 0) 
  {
    pathLength -= 3;
    path[pathLength] = 'R';
    //Serial.println("test2");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'L' && shortDone == 0) 
  {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test3");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'L' && shortDone == 0) 
  {
    pathLength -= 3;
    path[pathLength] = 'R';
    //Serial.println("test4");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'S' && shortDone == 0) 
  {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test5");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'L' && shortDone == 0) 
  {
    pathLength -= 3;
    path[pathLength] = 'S';
    //Serial.println("test6");
    shortDone = 1;
  }

  path[pathLength + 1] = 'D';
  path[pathLength + 2] = 'D';
  pathLength++;
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
  //printPath();
}

void replay() 
{
  readSensors();
  if (leftReading < thresholdLeft && rightReading < thresholdRight) 
  {
    straight();
  }
  else 
  {
    if (path[readLength] == 'D') 
    {
      //tiến
      delay(100);
      //tắt
      endMotion();
    }
    if (path[readLength] == 'L') 
    {
      //tiến
      delay(leapTime);
      turnLeft();
    }
    if (path[readLength] == 'R') 
    {
      //tiến
      delay(leapTime);
      turnRight();
    }
    if (path[readLength] == 'S') 
    {
      //đi thẳng
      delay(leapTime);
      straight();
    }
    readLength++;
  }
  replay();
}

void endMotion()
{
  
}

