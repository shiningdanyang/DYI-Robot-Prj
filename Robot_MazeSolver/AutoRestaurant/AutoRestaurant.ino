#include <Key.h>
#include <Keypad.h>
#define S0 31
#define S1 33
#define S2 35
#define S3 37
#define sensorOut 39

const int TriggerPin = 10;
const int EchoPin = 11;
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 8
#define ENA 3
#define ENB 9

#define RED 1
#define BLUE 2
#define NUM_BAN 9
#define TIME_STOP 5000
#define TIME_RE_TRAI 1000

int soBan = 0;
int count = 0;
int color = 0;
int state = 0;

char customKey;
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = 
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {36, 34, 32, 30}; // Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = {28, 26, 22, 24}; //Connect keypad COL0, COL1 and COL2 to these Arduino pins.
Keypad kpd = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup ()
{  //đầu tiên ta cần phải cấu hình các chân điều khiển như đầu ra.Với động cơ 1, 
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
 //communication freq (sensitivity) selection
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  //color mode selection
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  //color response pin (only actual input from taos)
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 100%
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  dungLai();
}

void loop() 
{
//  Serial.println(readColor());
  char key = kpd.getKey();
  if(key) 
  {
    Serial.println(key);
    if (key == '#') {
      if(soBan > 0 && soBan <= NUM_BAN) 
      {
        count = 0;
        while(count < soBan || color != RED) 
        {
          int c = readColor();
          if(c == BLUE && color == 0) 
          {
            count++;
            Serial.print("Den ban ");
            Serial.println(count);
            if(count == soBan) 
            {
              dungLai();
              delay(TIME_STOP);
              diCham();
            }
          }
          int distance = readDistance();
          if(distance < 17) 
          {
            reTrai();
            delay(TIME_RE_TRAI);
            diCham();
          } 
          else if(distance < 35) 
          {
            diCham();
          } 
          else 
          {
            diNhanh();
          }
          color = c;
          delay(50);
        }
        soBan = 0;
        dungLai();
        Serial.println("Ve diem xuat phat");
      }
    }
    else 
    {
      if(key >= '0' && key <= '9') 
      {
        soBan = key - '0';
      } 
      else 
      {
        key = 0;
      }
    }
  }
  delay(30);
}  

int readDistance()
{
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  int distance = pulseIn(EchoPin, HIGH) / 58;
//  Serial.print(distance);
//  Serial.println(" cm");
  return distance;
}

int readColor() 
{
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int R = pulseIn(sensorOut, LOW);
  delay(10);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int G = pulseIn(sensorOut, LOW);
  delay(10);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int B = pulseIn(sensorOut, LOW);
//  Serial.print("R= ");
//  Serial.println(R);
//  Serial.print("G= ");
//  Serial.println(G);
//  Serial.print("B= ");
//  Serial.println(B);
//  Serial.println("\n==========\n");
  if(G - R > 400 && B - R > 300 && R < 1300) {
    Serial.println("Do");
    return RED;
  }
  if(R - B > 300 && G - B > 300 && B < 1800) {
    Serial.println("Xanh");
    return BLUE;
  }
  return 0;
}

void dongco(int b, int a) 
{
  if(a > 0) 
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } 
  else 
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  if(b > 0) 
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } 
  else 
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  analogWrite(ENA, a);
  analogWrite(ENB, b);
}

void diNhanh() 
{
  dongco(150, 150);
  if(state != 2) 
  {
    state = 2;
    Serial.println("Di Nhanh");
  }
}

void diCham() 
{
  dongco(60, 60);
  if(state != 1) 
  {
    state = 1;
    Serial.println("Di Cham");
  }
}

void reTrai() 
{
  dongco(0, 100);
  if(state != 3) 
  {
    state = 3;
    Serial.println("Re Trai");
  }
}

void dungLai() 
{
  dongco(0, 0);
  if(state != 0) 
  {
    state = 0;
    Serial.println("Dung Lai");
  }
}

