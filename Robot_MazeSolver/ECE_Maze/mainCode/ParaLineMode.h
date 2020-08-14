#define left 0
#define right 1
#define front 2
/////////////////////////////////////////////////////
#define LEFT_FAR A0
#define LEFT_NEAR A0
#define CENTER A0
#define RIGHT_NEAR A0
#define RIGHT_FAR A0
/////////////////////////////////////////////////////
const float Kp_Line = 0; 
const float Ki_Line = 0; 
const float Kd_Line = 0;
int initSpeed_Line[2] =
{
  140, 140
};
/////////////////////////////////////////////////////
int minSpeed_Line = 0;
int maxSpeed_Line = 255;