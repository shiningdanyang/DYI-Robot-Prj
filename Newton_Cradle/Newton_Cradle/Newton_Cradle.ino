#include <Servo.h>
#include "SR04.h"

// Define Pins to control the RGB leds for each of the 5 balls
#define BLUE 30
#define GREEN 31
#define RED 32

#define BLUE2 33
#define GREEN2 34
#define RED2 35

#define BLUE3 36
#define GREEN3 37
#define RED3 38

#define BLUE4 39
#define GREEN4 40
#define RED4 41

#define BLUE5 42
#define GREEN5 43
#define RED5 44

// Define the pins for the button and the servos
#define BUTTON 7
#define SERVO1_PIN 8
#define SERVO2_PIN 9
const int POTENTIOMETER = A0;


Servo myservo1;
Servo myservo2;
int last_button = 1;
int mode_run = 0;
boolean pending_change = false;
int delayTime = 100;


int ValRed = random(0, 256);
int ValGreen = random(0, 256);
int ValBlue = random(0, 256);

void ChooseColor(void) {
  // Obtain the RGB values for a especific color
  // The Red Green and Blue color values could be 0 or 255
  // The RGB values are randomly choosen
  
  int indexRed = random(1, 3);
  int indexGreen = random(1, 3);
  int indexBlue = random(1, 3);

  if (indexRed == 1) ValRed = 0;
  else ValRed = 255;
  if (indexGreen == 1) ValGreen = 0;
  else ValGreen = 255;
  if (indexBlue == 1) ValBlue = 0;
  else ValBlue = 255;

  // If all the ramdom values are 255 (led off) the color will be Red
  if ((indexRed == 2) and (indexGreen == 2) and (indexBlue == 2)) {
    ValRed = 0;
    ValGreen = 255;
    ValBlue = 255;
  }
}

void SetColorBall(int ball, int redValue, int greenValue, int blueValue) {
  // Switch on a specific ball with the RGB values specified
  switch (ball) {
    case 1:
      analogWrite(RED, redValue);
      analogWrite(BLUE, blueValue);
      analogWrite(GREEN, greenValue);
      break;
    case 2:
      analogWrite(RED2, redValue);
      analogWrite(BLUE2, blueValue);
      analogWrite(GREEN2, greenValue);
      break;
    case 3:
      analogWrite(RED3, redValue);
      analogWrite(BLUE3, blueValue);
      analogWrite(GREEN3, greenValue);
      break;
    case 4:
      analogWrite(RED4, redValue);
      analogWrite(BLUE4, blueValue);
      analogWrite(GREEN4, greenValue);
      break;
    case 5:
      analogWrite(RED5, redValue);
      analogWrite(BLUE5, blueValue);
      analogWrite(GREEN5, greenValue);
      break;
  }
}

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(BLUE2, OUTPUT);
  pinMode(RED3, OUTPUT);
  pinMode(GREEN3, OUTPUT);
  pinMode(BLUE3, OUTPUT);
  pinMode(RED4, OUTPUT);
  pinMode(GREEN4, OUTPUT);
  pinMode(BLUE4, OUTPUT);
  pinMode(RED5, OUTPUT);
  pinMode(GREEN5, OUTPUT);
  pinMode(BLUE5, OUTPUT);
  
  pinMode(BUTTON, INPUT);

  myservo1.attach(SERVO1_PIN);
  myservo2.attach(SERVO2_PIN);

  Serial.begin(9600);

}


// main loop
void loop()
{
  #define stepangle 5         // The step in degrees of the servo movement
  #define delaymotor 20       // The dely time needed to the servo to reach to a specific position
  #define min_delay 30        // see comment POTENTIOMETER

  // INIT POTENTIOMETER
  // Read the potentiometer value to stablish the delayTime between actions only if
  // that value is greater than a specific min value (min_delay)
  // I mean we are changing the frecuency of the movement
  int value = analogRead(POTENTIOMETER);        
  int vposition = map(value, 0, 1023, 0, 100);
  if (vposition >= min_delay) delayTime = vposition*2;
  // END POTENTIOEMETER

  // INIT BUTTON
  // Read the button value to stablish the run mode
  // Every time we press the button, the pendulum will change the run mode (mode 0 or mode 1)
  int current_button = digitalRead(BUTTON); 
  if (not (current_button == last_button)) {
    if (not pending_change) {
      pending_change = true;
    } else {
      pending_change = false;
      if (mode_run == 0) mode_run = 1;
      else mode_run = 0;
      //Serial.println(mode_run);
    }
    last_button = current_button;    
  }
  else {
    pending_change = false;
  }
  // END BUTTON

  // INIT mode_run 1
  // In this mode both servos are moving at the same time
  // The code below switch on and off the leds and move the servos
  // SetColorBall -> switch on the led with a random color (ValRed, ValGreen, ValBlue) or off(255,255,255) of the specific ball
  // ChooseColor -> stablish the random values ValRed, ValGreen, ValBlue for a specific color
  if (mode_run == 1) {
    for (int angle = 90; angle >= 45; angle -= stepangle) {
      myservo1.write(angle);              
      myservo2.write(angle);              
      delay(delaymotor);                  
    }
    SetColorBall(1, ValRed, ValGreen, ValBlue);
    SetColorBall(5, ValRed, ValGreen, ValBlue);
    for (int angle = 45; angle <= 90; angle += stepangle) {
      myservo1.write(angle);              
      myservo2.write(angle);              
      delay(delaymotor);                  
    }
    SetColorBall(1, 255, 255, 255);
    SetColorBall(5, 255, 255, 255);
    
    SetColorBall(4, ValRed, ValGreen, ValBlue);
    SetColorBall(2, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(2, 255, 255, 255);
    SetColorBall(4, 255, 255, 255);
    ChooseColor();
    SetColorBall(3, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(3, 255, 255, 255);
    SetColorBall(4, ValRed, ValGreen, ValBlue);
    SetColorBall(2, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(2, 255, 255, 255);
    SetColorBall(4, 255, 255, 255);
    SetColorBall(1, ValRed, ValGreen, ValBlue);
    SetColorBall(5, ValRed, ValGreen, ValBlue);
  }
  // END mode_run 1

  // INIT mode_run 0
  // In this mode only one of the servo is movement at the same time
  // The code below switch on and off the leds and move the servos
  // SetColorBall -> switch on the led with a random color (ValRed, ValGreen, ValBlue) or off(255,255,255) of the specific ball
  // ChooseColor -> stablish the random values ValRed, ValGreen, ValBlue for a specific color
  if (mode_run == 0) {
    SetColorBall(1, 255, 255, 255);
    SetColorBall(2, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(2, 255, 255, 255);
    SetColorBall(3, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(3, 255, 255, 255);
    SetColorBall(4, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(4, 255, 255, 255);
  
    SetColorBall(5, ValRed, ValGreen, ValBlue);
    // Red color right side
    for (int angle = 90; angle >= 45; angle -= stepangle) {
      myservo2.write(angle);              
      delay(delaymotor);                  
    }
    ChooseColor();
    SetColorBall(5, ValRed, ValGreen, ValBlue);
    for (int angle = 45; angle <= 90; angle += stepangle) {
      myservo2.write(angle);             
      delay(delaymotor);                 
    }
    SetColorBall(5, 255, 255, 255);
  
    SetColorBall(4, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(4, 255, 255, 255);
    SetColorBall(3, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(3, 255, 255, 255);
    SetColorBall(2, ValRed, ValGreen, ValBlue);
    delay(delayTime);
    SetColorBall(2, 255, 255, 255);
  
    // Blue color left side
    SetColorBall(1, ValRed, ValGreen, ValBlue);
    for (int angle = 90; angle >= 45; angle -= stepangle) {
      myservo1.write(angle);              
      delay(delaymotor);                  
    }
  
    for (int angle = 45; angle <= 90; angle += stepangle) {
      myservo1.write(angle);            
      delay(delaymotor);               
    }
  }
  // END mode_run 0  
}
