/*
 * kyj_1107_1(II2_BUS).c
 *
 * Created: 2023-11-07 ���� 2:10:35
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

//U8 DEV_ADD_W[10] = {1, 1, 1, 0, 1, 0, 0, 0, 0, 0};
U8 DEV_ADD_W = 0xA0;
//U8 DEV_ADD_R = 0xA1;
U8 IIC_ADD = 0xBA;
U8 IIC_DAT = 0xC5;
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
//void Process_10data_Write(U8* write_value);
//void One_Byte_Read(void);
//void Process_8data_read(void);

void main(void)
{
    //U8 old_sw1, new_sw1;
    //U8 old_sw2, new_sw2;
    //U8 old_sw3, new_sw3;
    
    DDRC = 0xFF;  // PORTC ���
    PORTC = 0xAA; // LED 1010 1010 ON
    
    DDRE = 0x00; // PINE �Է�, Switch ���ؼ�
    
    SCL_OUT;  //START�Լ��� �־ ������. PORTD 0��
    SDA_OUT;  // PORTD 1��    
     
        
    while(1) // 1Byte Write
    {   
        One_Byte_Write();
        delay_ms(100);
    
    
        //new_sw1 = PINE & 0x10; // 4�� switch OFF ����
        //sw2 = PINE & 0x20; // 5�� switch OFF ����
        //sw3 = PINE & 0x40; // 6�� switch OFF ����
        
       
        //if((old_sw1 != 0) && (new_sw1 == 0)) // ����ġ �Է� ��
        //{ 
           // One_Byte_Write(); // 1Byte_Write ����
            //delay_ms(100);
       // }
            
        //old_sw1 = new_sw1;
          
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
        Process_8data_Write(DEV_ADD_W); // 0xA0, Device_Address
        ACK_write();
        
        Process_8data_Write(IIC_ADD); // Address
        ACK_write();
        
        Process_8data_Write(IIC_DAT); // Data
        ACK_write();
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


/*void Process_10data_Write(U8* write_value)
{
    U8 i;

    for(i = 0; i < 10; i++)
    {
        if(*(write_value + i) & 1) == 1)
        {
            DAT_HIGH;
        }
        else DAT_LOW;
        
        CLK_HIGH;
        CLK_LOW;
        
    } 
}

*/

/*
void One_Byte_Read(void)
{
    START();
    Process_8data_Write(DEV_ADD_W);
    ACK_write();
    Process_8data_Write(0xB0);
    ACK_write();
    START();
    Process_8data_read();
    ACK_read();
    Process_8data_read();
    ACK_read();
    Process_8data_read();
    no_ACK();
    STOP();

}

/*void Process_8data_read(void)
{
    U8 i, TEMP_READ_DATA = 0; //���� �����͸� �����صδ� ��.
    SDA_IN; // SDA�� OUTPUT���� INPUT���� ����

    for (i = 0; i < 8; i++)
    {
        CLK_LOW;
        CLK_HIGH;
        delay_us(12);
        CLK_LOW;

        TEMP_READ_DATA = (PIND & 0x02 == 0x00) ? 0x00 : 0x80; 
        TEMP_READ_DATA >>= i;
        READ_DATA |= TEMP_READ_DATA;
    }
}

 */



/*void ACK_read(void) //Slave�� Master�κ��� ACK�� �޾ƾ� ��.
{
    SDA_IN;
    CLK_HIGH;
    CLK_LOW;
    SDA_OUT;    
}
*/
    
     
    
    
    
    


   
