// Vesion 1.0
#include <mega8.h>
#include <delay.h>
#include <stdio.h>

#define ADC_VREF_TYPE 0x20 //

unsigned char read_adc(unsigned char adc_input)
{
    ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
    delay_us(10);
    ADCSRA|=0x40;
    while ((ADCSRA & 0x10)==0);
    ADCSRA|=0x10;
    return ADCH;
}
                                                  
unsigned char temp_adc[8]={0,0,0,0,0,0,0};  // mang RAM doc ADC dung de so sanh
unsigned char w[8] = {1,2,4,8,16,32,64,128};
// 00000001
// 00000010
// 00000100
// 00001000
// 00010000
// 00100000
// 01000000
// 10000000
eeprom unsigned char mau_adc[8] ;  // mang rom luu gia tri lay ma so sanh
unsigned char sensor = 0x00;

#define button PINB.0
#define out_sensor PORTD

void ghi_adc_in_rom (void) // chuong trinh thuc hien ghi ADC vao ROM
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        mau_adc[i] = read_adc(i);
        delay_ms(20);    // thuc hien tre thoi gian ghi
    }
}

void doc_cambien(void)    // thuc hien viec doc
{
    unsigned char j;
    sensor = 0x00;
    temp_adc[0] = read_adc(0);
    temp_adc[1] = read_adc(1);
    temp_adc[2] = read_adc(2);
    temp_adc[3] = read_adc(3);
    temp_adc[4] = read_adc(4);
    temp_adc[5] = read_adc(5);
    temp_adc[6] = read_adc(6);
    temp_adc[7] = read_adc(7);
    for(j=0;j<8;j++)
    {
        if((mau_adc[j]-3) < temp_adc[j])  // kiem tra so sanh voi gia tri ADC mau
        {
            sensor |= w[7-j];  // thuc hien cong don gia tri
        }              
        else sensor |= 0;
    }
}

void main(void)
{
PORTD = 0xFF;
DDRD = 0xFF;

PORTB = 0XFF;
DDRB = 0x00;
// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 500.000 kHz
// ADC Voltage Reference: AREF pin
// Only the 8 most significant bits of
// the AD conversion result are used
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x84;
while (1)
      {
        if(!button)
        {                    
            while(!button);
            out_sensor = 0x00;
            ghi_adc_in_rom();
            delay_ms(100);  
        }
        doc_cambien();
        out_sensor = sensor; // dua gia tri do duoc ra PORTD
                
      }
}