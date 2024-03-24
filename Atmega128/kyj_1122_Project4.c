/*
 * kyj_1122_Project4.c
 * Kim_Yong_Je
 * Created: 2023-11-22 오후 2:26:24
 * Author: Project4
 */

#include <mega128a.h>
#include <delay.h>

#define SCL_OUT DDRD |= 0x01 // PD.0=1, PORTD 0번핀 출력
#define SDA_OUT DDRD |= 0x02 // PD.1=1, PORTD 1번핀 출력
#define SLD_OUT DDRD |= 0x04 // PD.2=2, PORTD 2번핀 출력

#define CLK_HIGH PORTD |= 0x01 // PD.0=1 
#define CLK_LOW  PORTD &= 0xFE // PD.0=0

#define DAT_HIGH PORTD |= 0x02 // PD.1=1
#define DAT_LOW  PORTD &= 0xFD // PD.1=0

#define LD_HIGH PORTD |= 0x04 // PD.2 = 1
#define LD_LOW  PORTD &= 0xFB // PD.2 = 0

typedef unsigned char U8;
typedef unsigned short U16;

U8 i, j = 0;
U8 led = 0xF0;
U8 EXT_INT_BIT4 = 0;
U16 DAC_data = 0;
U16 Temp_DAC_value = 0;

U16 DAC_Array[21] = {0x000, 0x00F, 0x03D, 0x05C, 0x08A, 0x0A8, 0x0C0, 0x0E1, 0x0E6, 0x0F5, 0x0FF, 0x0F5, 0x0E6, 0x0E1, 0x0C0, 0x0A8, 0x08A, 0x05C, 0x03D, 0x00F, 0x000};

void DAC(U16* DAC_value);
void Dont_move(void);   // 시작하기 전 움직임이 없는 함수 
void START(void);       // 시작 함수

void main(void)
{
    SCL_OUT;
    SDA_OUT;
    SLD_OUT;
    
    DDRC = 0xFF; // PORTC 출력 설정
    PORTC = 0xFF; // INT4 확인용
    
    EIMSK = 0x10; // INT4 enable
    EICRB = 0x03; // INT4 rising edge
    
    SREG |= 0x80; // Global INT enable
       
    while (1)
    {
        Dont_move();
        
        if(EXT_INT_BIT4 == 1)
        {
            EXT_INT_BIT4 = 0;
            PORTC = led;    // 실행되면 켜짐
            DAC(DAC_Array);  // DAC data write start
            delay_ms(20);   // stop과 start 구분 및 오류 방지
            PORTC = 0xFF;  // 종료되면 LED ALL turn off
        }

    }
}

void DAC(U16* DAC_value)
{    
    for(i = 0; i < 21; i++)
    {
        DAC_data = DAC_value[i];
        
        START();
        Temp_DAC_value = DAC_data;
        
        for(j = 0; j < 10; j++)
        {    
            if((Temp_DAC_value & 0x200) == 0x200) 
                DAT_HIGH;
            else 
                DAT_LOW;
        
            Temp_DAC_value <<= 1; // 9 ~ 0 bit 까지 검사
            delay_us(10);
            CLK_HIGH;   // CLK High Level Time
            delay_us(10);
            CLK_LOW;    // CLK Low Level Time
            delay_us(10);
        } 
        LD_HIGH; // 종료
        delay_us(10);
    }   
}

void Dont_move(void)
{
    CLK_HIGH;
    delay_us(6);
    LD_HIGH;
    delay_us(6);
}

void START(void)
{
    CLK_LOW;
    delay_us(6);
    LD_LOW;
    delay_us(6);
}       

interrupt [EXT_INT4] void External_Interrupt4(void)
{
    SREG &= 0x7F; // All Interrupt disable
    EXT_INT_BIT4 = 1;    
    SREG |= 0x80; // All Interrupt enable
}

