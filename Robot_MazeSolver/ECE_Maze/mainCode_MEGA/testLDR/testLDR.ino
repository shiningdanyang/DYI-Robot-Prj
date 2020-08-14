int LDR_Line[8] = {A8, A9, A10, A11, A12, A13, A14, A15};
int OUT[8] = {22, 26, 30, 34, 38, 42, 46, 50};//////////////////////////////////////////////////////////////////////;
void setup()
{
	Serial.begin(9600);
}
void loop()
{
	for(int i = 0; i< 8;++i)
	{
		Serial.print(" ");
		Serial.print(analogRead(LDR_Line[i]));
	}
	Serial.println(" ");
	delay(100);
}
