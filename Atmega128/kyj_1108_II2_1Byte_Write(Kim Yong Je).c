/*
 * kyj_1108_II2_1Byte_Write(Kim Yong Je).c
 * II2_BUS_1Byte_Write_Project.
 * Created: 2023-11-08 오전 11:31:20
 * Author: 김용제
 */

#include <mega128.h>
#include <delay.h>

#define SCL_OUT DDRD |= 0x01 // PD.0=1, PORTD 0번핀 출력
#define SDA_IN DDRD &= 0xFD // PD.1=0, PIND 1번핀 입력
#define SDA_OUT DDRD |= 0x02 // PD.1=1, PORTD 1번핀 출력

#define CLK_HIGH PORTD |= 0x01 // PD.0=1 
#define CLK_LOW PORTD &= 0xFE // PD.0=0

#define DAT_HIGH PORTD |= 0x02 // PD.1=1
#define DAT_LOW PORTD &= 0xFD // PD.1=0

typedef unsigned char U8;

U8 DEV_ADD_W = 0xA0;
U8 IIC_ADD = 0xBA;
U8 IIC_DAT = 0xC5;


void Process_8data_Write(U8 value);
void START(void);
void STOP(void);
void ACK_write(void);
void One_Byte_Write(void);


void main(void)
{

    DDRC = 0xFF;  // PORTC 출력
    PORTC = 0xAA; // LED 1010 1010 ON

    
    SCL_OUT;  //START함수에 넣어도 괜찮음. PORTD 0번
    SDA_OUT;  // PORTD 1번    
     
        
    while(1) // 1Byte Write
    {   
        One_Byte_Write();
        delay_ms(100);
    }    
}

void START(void) // Falling Edge
{
    DAT_HIGH;
    CLK_HIGH;
    delay_us(12);
    DAT_LOW;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
}   

void STOP(void) // Rising Edge
{
    SDA_OUT;
    DAT_LOW;
    CLK_HIGH;
    delay_us(12);
    DAT_HIGH;
    delay_us(12);
    CLK_LOW;
}

void Process_8data_Write(U8 value) // DATA를 먼저 입력한다(달리 말하면 DATA를 먼저 세팅한다). CLK HIGH가 DATA를 유지한다. 그리고 CLK이 LOW일 때 DATA를 변화시킨다.
{                                  // Data_Address, Address, Data 8bits 입력.
    U8 i = 0;                       // SDA_OUT
    U8 Data;
    
    SDA_OUT;     
    
    Data = value;
    
    for(i = 0; i < 8; i++) // 7~0, 8번 반복.
    {
       if((Data & 0x80) == 0x80) // 7bit(MSB)부터 0bit까지 DATA검사.
            DAT_HIGH;  //PORTD 1번핀을 high(1)
       else 
            DAT_LOW; //반대(0)
       
       Data <<= 1; // 왼쪽 시프트 (그래야 6bit ~ 0bit까지 검사)
       
       CLK_HIGH;
       delay_us(12);
       CLK_LOW;
       delay_us(12);   
    }
    
    //if((Data & 0x80) == 0x80) DAT_HIGH; //위의 i가 7일 경우 마지막 0번 비트를 출력하기 위해 사용.
    //else DAT_LOW;
    
    CLK_HIGH;
    delay_us(12);
    CLK_LOW;
    
    if(Data == 0x80) DAT_LOW;  // 파형 오류 방지 작업.
    delay_us(12);

}

void ACK_write(void) // SDA_INPUT, Slave -> Master에게 보내는 신호
{   
    U8 i = 0;
    
    SDA_IN;   // OUTPUT에서 INPUT으로 변경, DDRD &= 0xFD, PIND 1번핀 입력                             
    CLK_HIGH;
    
    for(i = 0; i < 8; i++) // 8번 동안 신호를 검사(8bit라서 8bit 검사), 50번 검사했을 때는 오류나서 i값 변경하였음.
    {
        if((PIND & 0x02) == 0x00)
        {
            i = 7;
            PORTC = 0xFE; // LED 0번 ON
        }
        else 
        {
            PORTC = 0xFF; // ALL OFF
        }  
    }
    
    SDA_OUT;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
    PORTC = 0xF7;
}


void One_Byte_Write(void)
{
        START();
        Process_8data_Write(DEV_ADD_W); // 0xA0, Device_Address
        ACK_write();
        
        Process_8data_Write(IIC_ADD); // Address
        ACK_write();
        
        Process_8data_Write(IIC_DAT); // Data
        ACK_write();
        STOP();
}
