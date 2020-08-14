#include <EEPROM.h>
#include "motorControl.h"
#include "ParaLineMode.h"
#include "FuncLineMode.h"
#include "ParaWallMode.h"
#include "FuncWallMode.h"
#define startButton 14
#define GND 16
void setup()
{
  Serial.begin(9600);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  pinMode(getSampleButton_Line, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  for (int i = 0; i < 8; ++i)
  {
    pinMode(OUT[i], OUTPUT);
  }
  // ///////////////////////////////////////////////////////////////
  checkGetSampleButton_Line();
  // ///////////////////////////////////////////////////////////////
  getValueFromMemory_Line();
  // ///////////////////////////////////////////////////////////////
  while (digitalRead(startButton) == HIGH)
  {//khi chưa nhấn nút start
    for (int i = 0; i < 8; ++i)
    {
      // chớp đèn
      digitalWrite(OUT[i], !digitalRead(OUT[i]));
    }
    delay(50);
  }
}

void loop()
{
  // for(int i = 0; i< 8; ++i)
  // {
  //   digitalWrite(OUT[i], !digitalRead(OUT[i]));
  // }
  // delay(500);
  // readSensors_Line();
  // readSensors_Wall();
  // // for(int i = 0 ; i<3;++i)
  // // {
  // //   Serial.print(sensor_Wall[i]);
  // //   Serial.print(" ");
  // // }
  // Serial.println(sensor_Wall[right]);
  // delay(100);
  // readSensors_Wall();
  // motorControl_Wall(left)
  // motorLeft.run(FORWARD);
  // motorRight.run(FORWARD);
  // motorLeft.setSpeed(150);
  // motorRight.setSpeed(150);
  calculatePID_Line();
  motorControl_Line();

// leftFollow_Wall();
// readSensors_Wall();
// Serial.print(sensor_Wall[left]); Serial.println(" ");
// Serial.print(sensor_Wall[right]); Serial.print(" ");
// Serial.print(sensor_Wall[front]); Serial.println(" ");
}

