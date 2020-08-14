#include "motorControl.h"
#include "ParaLineMode.h"
#include "FuncLineMode.h"
#include "ParaWallMode.h"
#include "FuncWallMode.h"

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // readSensors_Wall();
  
  // Serial.println(sensor_Wall[right]); Serial.print(" ");
  Serial.println(sensor_Wall[front]); Serial.println(" ");
	  readSensors_Wall();
// Serial.println(sensor_Wall[left]); Serial.print(" ");
	motorControl_Wall(right);

}
