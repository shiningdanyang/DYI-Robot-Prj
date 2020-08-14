#include <Arduino.h>
#define left 0
#define right 1
int error_Line = 0;
float P_Line = 0, I_Line = 0, D_Line = 0, PID_Line = 0, previousError_Line = 0;
int sensor_Line[8] =
{
  0, 0, 0, 0, 0, 0, 0, 0
};

int sensorValue_Line[8];
int sampleBlack_Line[8];
int sampleWhite_Line[8];
// int sample_Line[8];
int thresholdBlack[8];
int thresholdWhite[8];
int result_Line;
// ADC nền đen > ADC nền trắng
void getValueFromMemory_Line()
{
  for (int i = 0; i < 8; ++i)
  {
    sampleBlack_Line[i] = EEPROM.read(i);
    delay(10);
  }
  for (int i = 0; i < 8; ++i)
  {
    sampleWhite_Line[i] = EEPROM.read(i + 8);
    delay(10);
  }
  for (int i = 0; i < 8; ++i)
  {
    thresholdBlack[i] = sampleBlack_Line[i] - (sampleBlack_Line[i] - sampleWhite_Line[i]) * 4/10;
    // thresholdBlack[i] = sampleBlack_Line[i] - 20;
    thresholdWhite[i] = sampleWhite_Line[i] + (sampleBlack_Line[i] - sampleWhite_Line[i]) * 4/10;
    // thresholdWhite[i] = sampleWhite_Line[i] + 20;
  }
}

void getBlackSample_Line()
{
  for (int i = 0; i < 8; i++)
  {
    // xóa EEPROM
    EEPROM.write(i, 0);
    delay(5); // Phải có delay tối thiểu 5 mili giây giữa mối lần write
  }
  for (int i = 0; i < 8; ++i)
  {
    int valBlack = analogRead(LDR_Line[i]) /4;
    EEPROM.write(i, valBlack);
    digitalWrite(OUT[i], HIGH);
    delay(100);
  }
  delay(100);
}

void getWhiteSample_Line()
{
  for (int i = 0; i < 8; ++i)
  {
    // xóa EEPROM
    EEPROM.write(i + 8, 0);
    delay(5); // Phải có delay tối thiểu 5 mili giây giữa mối lần write
  }
  for (int i = 0; i < 8; ++i)
  {
    int valWhite = analogRead(LDR_Line[i]) /4;
    EEPROM.write(i + 8, valWhite);
    digitalWrite(OUT[i], HIGH);
    delay(100);
  }
  delay(100);
}

void checkGetSampleButton_Line()
{
  if (digitalRead(getSampleButton_Line) == HIGH)
  {
    // nếu như rút jump
    while (digitalRead(getSampleButton_Line) == HIGH)
    // trong khi chưa cắm jump, chớp đèn 500, chờ lấy mẫu nền
    {
      // trong khi chưa cắm lại jump, chờ để lấy mẫu đen
      for (int i = 0; i < 8; ++i)
      // chớp đèn 500
      {
        // chớp đèn 500
        digitalWrite(OUT[i], !digitalRead(OUT[i]));
      }
      delay(500);
    }
    // khi đã cắm lại jump
    for (int i = 0; i < 8; ++i)
    // sáng tất cả các led
    {
      // sáng tất cả
      digitalWrite(OUT[i], HIGH);
    }
    for (int i = 0; i < 8; ++i)
    // lần lượt tắt led
    {
      // lướt tắt qua các đèn
      digitalWrite(OUT[i], LOW);
      delay(200);
    }
    delay(200);
    // lấy mẫu đen
    getBlackSample_Line(); // (trong quá trình lấy mẫu led lần lượt sáng)
    // kết thúc lấy mẫu
    delay(200);
    // ///////////////////////
    while (digitalRead(getSampleButton_Line) == LOW)
    // trong khi chưa rút jump ra
    {
      // khi jump còn cắm, chờ để lấy mẫu trắng
      for (int i = 0; i < 8; ++i)
      {
        // chớp đèn 300
        digitalWrite(OUT[i], !digitalRead(OUT[i]));
      }
      delay(300);
    }
    // khi đã rút jump
    for (int i = 0; i < 8; ++i)
    // sáng tất cả các led
    {
      // sáng tất cả
      digitalWrite(OUT[i], HIGH);
    }
    for (int i = 0; i < 8; ++i)
    // lần lượt tắt led
    {
      // lướt tắt qua các đèn
      digitalWrite(OUT[i], LOW);
      delay(200);
    }
    delay(200);
    // lấy mẫu trắng
    getWhiteSample_Line(); // (trong quá trình lấy mẫu led lần lượt sáng)
    // kết thúc lấy mẫu
    delay(200);
    // ///////////////////////////////////////////
    for (int i = 0; i < 8; ++i)
    // sáng tất cả các led
    {
      // sáng tất cả
      digitalWrite(OUT[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < 8; ++i)
    // sáng tất cả các led
    {
      // sáng tất cả
      digitalWrite(OUT[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < 8; ++i)
    // sáng tất cả các led
    {
      // sáng tất cả
      digitalWrite(OUT[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < 8; ++i)
    // sáng tất cả các led
    {
      // sáng tất cả
      digitalWrite(OUT[i], HIGH);
    }
    delay(500);
    // //////////////////////
  }
  while (digitalRead(getSampleButton_Line) == HIGH)
  {
    digitalWrite(OUT[0], !digitalRead(OUT[0]));
    digitalWrite(OUT[7], !digitalRead(OUT[7]));
    delay(500);
  }
}

void readSensors_Line()
{
  // result_Line = 0;
  for (int i = 0; i < 8; ++i)
  {
    int value = analogRead(LDR_Line[i]) / 4;
    if (value < thresholdWhite[i])
    // nếu như giá trị LDR bé hơn ngưỡng
    {
      digitalWrite(OUT[7 - i], HIGH);
      // result_Line |= 1 << (7 - i);
      // result_Line |= 1 << i;
      result_Line |= (1UL << i);
      // sensorValue_Line[i] = 1;
    }
    if (value > thresholdBlack[i])
    {
      digitalWrite(OUT[7 - i], LOW);
      // result_Line &= ~(1 << (7 - i));
      // result_Line &= ~(1 << i);
      result_Line &= ~(1UL << i);
      // sensorValue_Line[i] = 0;
    }
  }
  switch (result_Line)
  {
    case 0b00000001:
      error_Line = 7;
      break;
    case 0b00000011:
      error_Line = 6;
      break;
    case 0b00000010:
      error_Line = 5;
      break;
    case 0b00000110:
      error_Line = 4;
      break;
    case 0b00000100:
      error_Line = 3;
      break;
    case 0b00001100:
      error_Line = 2;
      break;
    case 0b00001000:
      error_Line = 1;
      break;
    case 0b00011000:
      error_Line = 0;
      break;
    case 0b00010000:
      error_Line = -1;
      break;
    case 0b00110000:
      error_Line = -2;
      break;
    case 0b00100000:
      error_Line = -3;
      break;
    case 0b01100000:
      error_Line = -4;
      break;
    case 0b01000000:
      error_Line = -5;
      break;
    case 0b11000000:
      error_Line = -6;
      break;
    case 0b10000000:
      error_Line = -7;
      break;
    case 0b00111100:
    {
      if (error_Line == 0)
      {
        error_Line = 0;
      }
      break;
    }
    // case 0b11111111://ngã tư chữ T
    case 0b00000000://mất line
      {
        switch (error_Line)
        {
          case 7:
            error_Line = 8;
            break;
          case 8:
            error_Line = 8;
            break;
          case - 7:
            error_Line = -8;
            break;
          case - 8:
            error_Line = -8;
            break;
          // case 0:
          //   error_Line = 0;
          //   break;
            // default:
            // ;
        }
      }
      // default:
      // ;
  }
}

void calculatePID_Line()
{
  readSensors_Line();
  P_Line = error_Line;
  I_Line = I_Line + error_Line;
  D_Line = error_Line - previousError_Line;
  PID_Line = (Kp_Line * P_Line) + (Ki_Line * I_Line) + (Kd_Line * D_Line);
  previousError_Line = error_Line;
}

void motorControl_Line()
{
  calculatePID_Line();
  int leftSpeed = initSpeed_Line[left] - PID_Line;
  int rightSpeed = initSpeed_Line[right] + PID_Line;
  leftSpeed = constrain(leftSpeed, minSpeed_Line, maxSpeed_Line);
  rightSpeed = constrain(rightSpeed, minSpeed_Line, maxSpeed_Line);
  controlled(leftSpeed, rightSpeed);
}
