#define left 0
#define right 1
#define front 2
/////////////////////////////////////////////////////
#define getSampleButton_Line 53
int LDR_Line[8] = {A8, A9, A10, A11, A12, A13, A14, A15};
int OUT[8] = {22, 26, 30, 34, 38, 42, 46, 50};//////////////////////////////////////////////////////////////////////;
int sensitive = 80;
/////////////////////////////////////////////////////
const float Kp_Line = 15; 
const float Ki_Line = 0; 
const float Kd_Line = 300;
int initSpeed_Line[2] =
{
  130, 130
};
/////////////////////////////////////////////////////
int minSpeed_Line = 0;
int maxSpeed_Line = 150;