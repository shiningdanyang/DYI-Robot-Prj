/*****************************************************************************
  *Ten Tep          :     Canh Tay RoBot V1.0
  *Ngay             :     10/06/2018
  *Tac Gia          :     MCA TEAM
  *Cong Ty          :     MCA
  *Fanpage          :     https://www.facebook.com/MicroControllerAcademY
  *Phien Ban        :     V1.0
  *Tom Tat          :     Chương trình mô phỏng đọc các động tác tay người 
  *                       để điều khiển cánh tay Robot.
  *****************************************************************************/
#include <Servo.h>//Thư viện điều khiển servo

Servo servo1; //Tạo đối tượng servo để điều khiển servo (Ngón cái)
Servo servo2; //Tạo đối tượng servo để điều khiển servo (Ngón trỏ)
Servo servo3; //Tạo đối tượng servo để điều khiển servo (Ngón giữa)
Servo servo4; //Tạo đối tượng servo để điều khiển servo (Ngón áp út)
Servo servo5; //Tạo đối tượng servo để điều khiển servo (Ngón út)
Servo servo6; //Tạo đối tượng servo để điều khiển servo (Khủy tay)

int pos[6] = {0, 0, 0, 0, 0, 0}; // Khởi tạo mảng chứa  góc quay cho từng servo
//int Value1,Value2,Value3,Value4,Value5,Value6 = 0;    // Khởi tạo biến góc quay cho từng servo

int scanOrder[6] = {A0, A1, A2, A3, A4, A5};//Khai báo mảng các chân đọc ADC
int ValueADC[6] = {0, 0, 0, 0, 0, 0}; //Khai báo mảng để lưu giá trị ADC tiwf Flexsensor
int Value;//Biến tạm dùng để so sáng giá trị ADC
const int Count_Scan = 5; //Khai báo số kênh Scan ADC
char str[80];//Khai báo mảng để in ra cổng Serial
void setup() {
  Serial.begin(115200);//Mở cổng Serial để debug (baud 115200)
  servo1.attach(3);   // gán chân 3 điều khiển servo 1
  servo2.attach(5);   // gán chân 5 điều khiển servo 2
  servo3.attach(6);   // gán chân 6 điều khiển servo 3
  servo4.attach(9);   // gán chân 9 điều khiển servo 4
  servo5.attach(10);  // gán chân 10 điều khiển servo 5
  servo6.attach(11);  // gán chân 11 điều khiển servo 6
}

void loop() {
  for (int i = 0; i <= Count_Scan; i++) //Dùng vong for để đọc giá trị ADC
  {
    Value = (analogRead(scanOrder[i]) - ValueADC[i]);//Gán biến Value bằng giá trị ADC hiện tại trừ đi giá trị ADC trước đó
    if (abs(Value) > 20) 
    {//Nếu giá trị thay đổi lớn hơn 20 THÌ :
      ValueADC[i] = analogRead(scanOrder[i]);//ghi giá trị ADC vào mảng
      pos[i] = map(ValueADC[i], 630, 1023, 0, 180);//Chuyển đổi giá trị ADC sang giá trị góc quay của servo
      sprintf(str, "GtriADC = %d, Gtripos = %d", ValueADC[i], pos[i]);
      Serial.println(str);//In các giá trị ra Moniter để debug
      delay(1);
    }
  }
  servo1.write(pos[0] * 1.3);//
  servo2.write(pos[1] * 1.3);//Ghi giá trị góc quay ra cho các servo
  servo3.write(pos[2] * 1.3);//Tùy vào độ nhạy của cảm biến mà hệ số nhân sẽ khác nhau và góc ban đầu cũng khác nhau
  servo4.write(pos[3] * 1.4);
  servo5.write((pos[4] * 1.3) + 38);

}
