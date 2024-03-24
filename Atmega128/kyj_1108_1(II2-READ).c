/*
 * kyj_1108_1(II2-READ).c
 *
 * Created: 2023-11-08 ���� 11:43:51
 * Author: user
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
//U8 IIC_DAT = 0xC5;
//U8 DEV_ADD_R = 0xA1;
//U8 READ_DATA = 0;
U8 Array_Write[10] = {0xA5, 0xA6, 0xA7, 0xB5, 0xB6, 0xB7, 0xC5, 0xC6, 0xC7, 0x5A};

void START(void);
void STOP(void);
void Process_8data_Write(U8 value);
void ACK(void);                     // ACK �Լ�(write, read �� �� ����ϴ� ��)
//void One_Byte_Write(void);
void Process_10byte_Write(U8* write_value);
void Ten_Byte_Write(void);
/*void no_ACK(void);                  // NACK �Լ� master -> slave
void ACK_Seq(void); */                 // sequential write, read�� �� ���
void ACK_10Byte(void);
/*void Process_8data_Read(void);      // 8bit data �ޱ�
void Random_read(void); */            // READ��

void main(void)
{
    DDRC = 0xFF;  // PORTC ���
    PORTC = 0xAA; // LED 1010 1010 ON

    SCL_OUT;  //START�Լ��� �־ ������. PORTD 0��
    SDA_OUT;  // PORTD 1��    
         
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
    delay_us(12);
    
    if(Data == 0x80) DAT_LOW;  // ���� ���� ���� �۾�.
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

void ACK(void) // SDA_INPUT, Slave -> Master���� ������ ��ȣ
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
    
    PORTC = 0xFD;
    
    SDA_OUT;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
    
}

/*id no_ACK(void)  // Master -> Slave���� ���
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

void ACK_10Byte(void) // SDA_INPUT, Slave -> Master���� ������ ��ȣ
{   
    U8 i = 0;
    
    SDA_IN;   // OUTPUT���� INPUT���� ����, DDRD &= 0xFD, PIND 1���� �Է�                             
    CLK_HIGH;
    
    for(i = 0; i < 80; i++) // 8�� ���� ��ȣ�� �˻�(8bit�� 8bit �˻�), 50�� �˻����� ���� �������� i�� �����Ͽ���.
    {
        if((PIND & 0x02) == 0x00)
        {
            i = 79;
            PORTC = 0xFE; // LED 0�� ON
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
