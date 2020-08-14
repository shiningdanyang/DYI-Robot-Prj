#include <AFMotor.h>
#include <Servo.h> // Sử dụng thư viện Servo
const int trigPin = 10;
const int echoPin = 11;
// Khai báo biến thời gian và khoảng cách
long duration;
int distance,distance_r,distance_l;
int calculateDistance();

Servo myServo; // Tạo đối tượng Servo để điều khiển Servo
AF_DCMotor motor_left(1, MOTOR12_64KHZ);
AF_DCMotor motor_right(2, MOTOR12_64KHZ);
void setup() 
{
    pinMode(trigPin, OUTPUT); // Set chân trig là chân OUTPUT
    pinMode(echoPin, INPUT); // Set chân echo INPUT
    myServo.attach(9); // Chân Servo là chân 9
    motor_left.setSpeed(255);     // chọn tốc độ maximum 255`/255
    motor_right.setSpeed(255);     // chọn tốc độ maximum 255`/255
    myServo.write(90);
}

void loop() 
{
  distance = calculateDistance();// Gọi hàm tính khoảng cách
  while (distance>10) 
  {
    motor_left.run(FORWARD);
    motor_right.run(FORWARD);
    distance = calculateDistance();
  }
  
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
  
  myServo.write(45);
  delay(500);
  distance_r = calculateDistance();
  
  myServo.write(135);
  delay(500);
  distance_l = calculateDistance();

  if (distance_r>10 && distance_l<10)
  {
    motor_left.run(FORWARD);
    delay(100);
    motor_left.run(RELEASE);
  } 
  else 
  
  if ((distance_r<10 && distance_l>10)||(distance_r>10 && distance_l>10))
  {
    motor_right.run(FORWARD);
    delay(100);
    motor_right.run(RELEASE);
  } 
  else //(distance_r<10 && distance_l<10)
  {
      motor_left.run(FORWARD);
      motor_right.run(BACKWARD);
      delay(300);
      motor_left.run(RELEASE);
      motor_right.run(RELEASE);
  }
  
  myServo.write(90);
}

int calculateDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Phát xung siêu âm với thời gian là 10 Micro giây
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);// Ngưng phát xung
    duration = pulseIn(echoPin, HIGH); // Tính thời gian xung siêu âm phát ra đập vào vật cản rồi dội lại là bao nhiêu???
    distance= duration*0.034/2;// Tính khoảng cách từ siêu âm đến vật cản ( vì vận tốc của xung siêu âm là 344m/s...và sau khi đập vào vật cản rồi dội lại, nên /2) ==>> khoảng cách = thời gian*vận tốc/2
    return distance;
}
