#define left 0
#define right 1
#define front 2
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define FRONT_THRESHOLD 100
#define TRIG_PINL A0
#define ECHO_PINL A1
#define TRIG_PINR A2
#define ECHO_PINR A3
#define TRIG_PINF A4
#define ECHO_PINF A5
// /////////////System Variable////////////////////////
const int setpoint_Wall[2] =
{
  10, 10
}; // 7//9

const float Kp_Wall[2] =
{
  20, 20
}; // 30//20,20

const float Ki_Wall[2] =
{
  0, 0
};

const float Kd_Wall[2] =
{
  800, 800
}; // bám trái ->>>>>800 ok  //runskip scanning

// const float KiTurn = 0.2;
const int initSpeed_Wall[2] =
{
  70, 70
}; // 230//180//210

// ////////////////////////////////////////////////
int PIDTurn;
int minSpeed_Wall = 10; // 50 ok//40//20//20
int maxSpeed_Wall = 200; // 90 ok//80 ok//180//230
// ////////////////////////////////////////////////////
// ///////////////Environment Variables///////////////
const int maxError_Wall = 15;
const int minError_Wall = -15; // 5
const int frontThreshold_Wall = 7; // 10
const int leftThreshold_Wall = 40;
const int rightThreshold_Wall = 40;


// int brakeTime = 40;
// int _speedSkip1 = 200;
// int _speedSkip2 = 200;
// int _timeSkip1 = 1000;
// int _timeSkip2 = 200;
// int _left90 = 600;
// int _right90 = 600;
// ///////////////////////////////////////////////////