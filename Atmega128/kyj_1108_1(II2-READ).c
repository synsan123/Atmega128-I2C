/*
 * kyj_1108_1(II2-READ).c
 *
 * Created: 2023-11-08 오전 11:43:51
 * Author: user
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
//U8 IIC_DAT = 0xC5;
//U8 DEV_ADD_R = 0xA1;
//U8 READ_DATA = 0;
U8 Array_Write[10] = {0xA5, 0xA6, 0xA7, 0xB5, 0xB6, 0xB7, 0xC5, 0xC6, 0xC7, 0x5A};

void START(void);
void STOP(void);
void Process_8data_Write(U8 value);
void ACK(void);                     // ACK 함수(write, read 할 때 사용하는 것)
//void One_Byte_Write(void);
void Process_10byte_Write(U8* write_value);
void Ten_Byte_Write(void);
/*void no_ACK(void);                  // NACK 함수 master -> slave
void ACK_Seq(void); */                 // sequential write, read할 때 사용
void ACK_10Byte(void);
/*void Process_8data_Read(void);      // 8bit data 받기
void Random_read(void); */            // READ함

void main(void)
{
    DDRC = 0xFF;  // PORTC 출력
    PORTC = 0xAA; // LED 1010 1010 ON

    SCL_OUT;  //START함수에 넣어도 괜찮음. PORTD 0번
    SDA_OUT;  // PORTD 1번    
         
    while(1) // 1Byte Write
    {   
        Ten_Byte_Write();
        delay_ms(100);
    }    
}

void START(void) // Falling Edge
{
    DAT_HIGH;
    delay_us(12);
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
    delay_us(12);
    CLK_HIGH;
    delay_us(12);
    DAT_HIGH;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
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
    delay_us(12);
    
    if(Data == 0x80) DAT_LOW;  // 파형 오류 방지 작업.
    delay_us(12);

}

void Process_10byte_Write(U8* write_value)
{
    U8 i = 0;
    U8 Data[10] = {0};
    
    for(i = 0; i < 10; i++)
    {
        Data[i] = write_value[i]; 
    }
    
    for(i =0; i < 80; i++)
    {
        if((Data[i] & 0x80) == 0x80)
            DAT_HIGH;
        else
            DAT_LOW;
            
        CLK_HIGH;
        delay_us(20);
        CLK_LOW;
        delay_us(20);
    }
    
    CLK_HIGH;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
}

void ACK(void) // SDA_INPUT, Slave -> Master에게 보내는 신호
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
    
    PORTC = 0xFD;
    
    SDA_OUT;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
    
}

/*id no_ACK(void)  // Master -> Slave에게 명령
{
    SDA_OUT;
    DAT_HIGH;
    delay_us(12);
    CLK_HIGH;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
}*/

/*id ACK_Seq(void)
{
    SDA_OUT;
    DAT_LOW;
    delay_us(12);
    CLK_HIGH;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
}*/

void ACK_10Byte(void) // SDA_INPUT, Slave -> Master에게 보내는 신호
{   
    U8 i = 0;
    
    SDA_IN;   // OUTPUT에서 INPUT으로 변경, DDRD &= 0xFD, PIND 1번핀 입력                             
    CLK_HIGH;
    
    for(i = 0; i < 80; i++) // 8번 동안 신호를 검사(8bit라서 8bit 검사), 50번 검사했을 때는 오류나서 i값 변경하였음.
    {
        if((PIND & 0x02) == 0x00)
        {
            i = 79;
            PORTC = 0xFE; // LED 0번 ON
        }
        else 
        {
            PORTC = 0xFF; // ALL OFF
        }  
    }
    
    PORTC = 0xFD;
    
    SDA_OUT;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
}

/*id One_Byte_Write(void)
{
        START();
        Process_8data_Write(DEV_ADD_W); // 0xA0, Device_Address
        ACK();
        
        Process_8data_Write(IIC_ADD); // Address
        ACK();
        
        Process_8data_Write(IIC_DAT); // Data
        ACK();
        STOP();
}*/
void Ten_Byte_Write(void)
{
    START();
    Process_8data_Write(DEV_ADD_W);
    ACK();    
    
    Process_8data_Write(IIC_ADD);
    ACK();
    
    Process_10byte_Write(Array_Write);
    ACK_10Byte();
    STOP();
}
