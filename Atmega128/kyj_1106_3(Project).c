/*
 * kyj_1106_3(Project).c
 *
 * Created: 2023-11-06 오전 9:34:33
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
//U8 DEV_ADD_R = 0xA1;
// IIC_ADD = 0;
//U8 IIC_DAT = 0;
//U8 READ_DATA = 0;

void Process_8data_Write(U8 value);
void START(void);
void STOP(void);
void ACK_write(void);
//void no_ACK(void);
//void ACK_read(void);
//void proc_data_read(U8 tt);
//void write_8byte(void);
void One_Byte_Write(void);

void main(void)
{
    U8 sw;
    
    DDRC = 0xFF;  // PORTC 출력
    PORTC = 0xAA; // LED 1010 1010 ON
    
    DDRE = 0x00; // PINE 입력, Switch 위해서
    
    SCL_OUT;  //START함수에 넣어도 괜찮음. PORTD 0번
    SDA_OUT;  // PORTD 1번    
        
    while(1) // 1Byte Write
    {
        sw = PINE & 0x10; // 4번 switch OFF상
        if(sw == 0) // 스위치 입력 시
        { 
            One_Byte_Write(); // 1Byte_Write 실행
        }    
        else PORTC = 0xAF; // 스위치 OFF 일 때, LED 1010 1111, 6, 4 ON
                
        /*START();
        delay_us(12)
        Process_8data_Write(DEV_ADD_W); // 0xA0, Data_Address
        delay_us(12);
        ACK_write();
        delay_us(12)
        Process_8data_Write(0xC0); // Address
        delay_us(12)
        ACK_write();
        delay_us(12)
        Process_8data_Write(0xE0); // Data
        delay_us(12)
        ACK_write();
        delay_us(12)
        STOP();
        */   
    }    
}

void START(void)
{
    DAT_HIGH;
    CLK_HIGH;
    DAT_LOW;
    CLK_LOW;
}   

void STOP(void)
{
    DAT_LOW;
    CLK_HIGH;
    DAT_HIGH;
    CLK_LOW;
}

void Process_8data_Write(U8 value) // DATA를 먼저 입력한다(달리 말하면 DATA를 먼저 세팅한다). CLK HIGH가 DATA를 유지한다. 그리고 CLK이 LOW일 때 DATA를 변화시킨다.
{                                  // Data_Address, Address, Data 8bits 입력.
    U8 i = 0;                       // SDA_OUT
    U8 Data;     
    
    Data = value;
    
    for(i = 0; i < 8; i++) // 7~0, 8번 반복.
    {
       if((Data & 0x80) == 0x80) // 7bit(MSB)부터 0bit까지 DATA검사.
       {
        DAT_HIGH;  //PORTD 1번핀을 high(1)
       } 
       else DAT_LOW; //반대(0)
       
       CLK_HIGH;
       CLK_LOW;
       
       Data <<= 1; // 왼쪽 시프트 (그래야 6bit ~ 0bit까지 검사)
    }

}

void ACK_write(void) // SDA_INPUT, Slave -> Master에게 보내는 신호
{   
    U8 i = 0;
    
    SDA_IN;   // OUTPUT에서 INPUT으로 변경, DDRD &= 0xFD, PIND 1번핀 입력                             
    CLK_HIGH;
    
    for(i = 0; i < 50; i++) // 50번 동안 신호를 
    {
        if((PIND & 0x02) == 0x00)
        {
            i = 49;
            PORTC = 0xFE; // LED 0번 ON
        }
        else 
        {
            PORTC = 0xFF; // ALL OFF
        }  
    }
    delay_us(12);
    CLK_LOW;
    SDA_OUT;
    delay_us(12);
}

/*void no_ACK(void)
{
    U8 i = 0;
    
    SDA_IN;
    CLK_HIGH;
    
    for(i = 0; i < 50; i++)
    {
        if((PIND & 0x02) == 0x01))
        {
            i =49;
            PORTC = 0xFC; // LED 0,1 ON
        }
        else PORTC = 0x00; // LED ALL ON
    }
    delay_us(12);
    CLK_LOW;
    SDA_OUT;
    delay_us(12);
}
*/

void One_Byte_Write(void)
{
        START();
        delay_us(12);
        Process_8data_Write(DEV_ADD_W); // 0xA0, Device_Address
        delay_us(12);
        ACK_write();
        delay_us(12);
        Process_8data_Write(0xB0); // Address
        delay_us(12);
        ACK_write();
        delay_us(12);
        Process_8data_Write(0xF0); // Data
        delay_us(12);
        ACK_write();
        delay_us(12);
        STOP();
}

/*void proc_data_read(U8 tt)
{
    U8 i = 0;
    U8 Data_read;
    
    START();
    
    Data_read = tt;
    
    for(i = 0; i < 8; i++)
    {
        if((Data_read & 0x80)) == 0x01)
            {
                DAT_HIGH;
            }
        else DAT_LOW;
        
        CLK_HIGH;
        CLK_LOW;
        
        Data_read <<= 1;
    }

}
*/




