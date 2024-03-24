/*
 * kyj_1108_II2_1Byte_Write(Kim Yong Je).c
 * II2_BUS_1Byte_Write_Project.
 * Created: 2023-11-08 ���� 11:31:20
 * Author: �����
 */

#include <mega128.h>
#include <delay.h>

#define SCL_OUT DDRD |= 0x01 // PD.0=1, PORTD 0���� ���
#define SDA_IN DDRD &= 0xFD // PD.1=0, PIND 1���� �Է�
#define SDA_OUT DDRD |= 0x02 // PD.1=1, PORTD 1���� ���

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

    DDRC = 0xFF;  // PORTC ���
    PORTC = 0xAA; // LED 1010 1010 ON

    
    SCL_OUT;  //START�Լ��� �־ ������. PORTD 0��
    SDA_OUT;  // PORTD 1��    
     
        
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

void Process_8data_Write(U8 value) // DATA�� ���� �Է��Ѵ�(�޸� ���ϸ� DATA�� ���� �����Ѵ�). CLK HIGH�� DATA�� �����Ѵ�. �׸��� CLK�� LOW�� �� DATA�� ��ȭ��Ų��.
{                                  // Data_Address, Address, Data 8bits �Է�.
    U8 i = 0;                       // SDA_OUT
    U8 Data;
    
    SDA_OUT;     
    
    Data = value;
    
    for(i = 0; i < 8; i++) // 7~0, 8�� �ݺ�.
    {
       if((Data & 0x80) == 0x80) // 7bit(MSB)���� 0bit���� DATA�˻�.
            DAT_HIGH;  //PORTD 1������ high(1)
       else 
            DAT_LOW; //�ݴ�(0)
       
       Data <<= 1; // ���� ����Ʈ (�׷��� 6bit ~ 0bit���� �˻�)
       
       CLK_HIGH;
       delay_us(12);
       CLK_LOW;
       delay_us(12);   
    }
    
    //if((Data & 0x80) == 0x80) DAT_HIGH; //���� i�� 7�� ��� ������ 0�� ��Ʈ�� ����ϱ� ���� ���.
    //else DAT_LOW;
    
    CLK_HIGH;
    delay_us(12);
    CLK_LOW;
    
    if(Data == 0x80) DAT_LOW;  // ���� ���� ���� �۾�.
    delay_us(12);

}

void ACK_write(void) // SDA_INPUT, Slave -> Master���� ������ ��ȣ
{   
    U8 i = 0;
    
    SDA_IN;   // OUTPUT���� INPUT���� ����, DDRD &= 0xFD, PIND 1���� �Է�                             
    CLK_HIGH;
    
    for(i = 0; i < 8; i++) // 8�� ���� ��ȣ�� �˻�(8bit�� 8bit �˻�), 50�� �˻����� ���� �������� i�� �����Ͽ���.
    {
        if((PIND & 0x02) == 0x00)
        {
            i = 7;
            PORTC = 0xFE; // LED 0�� ON
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
