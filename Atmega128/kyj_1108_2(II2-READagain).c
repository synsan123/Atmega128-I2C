#include <mega128.h>
#include <delay.h>

#define SCL_OUT DDRD |= 0x01 // PD.0=1, PORTD 0번핀 출력
#define SDA_IN DDRD &= 0xFD // PD.1=0, PIND 1번핀 입력
#define SDA_OUT DDRD |= 0x02 // PD.1=1, PORTD 1번핀 출력

#define CLK_HIGH PORTD |= 0x01 // PD.0=1 
#define CLK_LOW PORTD &= 0xFE // PD.0=0

#define DAT_HIGH PORTD |= 0x02 // PD.1=1
#define DAT_LOW PORTD &= 0xFD // PD.1=0

#define N 10
#define postpone 20

typedef unsigned char U8;

U8 DEV_ADD_W = 0xA0;
U8 DEV_ADD_R = 0xA1;
U8 IIC_ADD = 0xAA;
U8 IIC_DAT = 0x55;
U8 READ_DATA = 0;
U8 Array_Write[10] = {0xA5, 0xA6, 0xA7, 0xB5, 0xB6, 0xB7, 0xC5, 0xC6, 0xC7, 0x5A};

void START(void);
void STOP(void);
void Process_8data_Write(U8 value);
void ACK(void);                     // ACK 함수(write, read 할 때 사용하는 것)
void One_Byte_Write(void);
void Process_10byte_Write(U8* write_value);
void Ten_Byte_Write(void);
void no_ACK(void);                  // NACK 함수 master -> slave
//void ACK_Seq(void);                 // sequential write, read할 때 사용
void Process_8data_Read(void);      // 8bit data 받기
void Random_Read(void);            // READ함

void main(void)
{
    U8 old_sw4, new_sw4;
    U8 old_sw5, new_sw5;
    U8 old_sw6, new_sw6;
    
    DDRE = 0x00;   // PORTE 입력 => PINE 가져오기

    DDRC = 0xFF;  // PORTC 출력
    PORTC = 0xAA; 

    SCL_OUT;  //START함수에 넣어도 괜찮음. PORTD 0번
    SDA_OUT;  // PORTD 1번
    
    old_sw4 = PINE & 0x10; // 스위치4 OFF     
    old_sw5 = PINE & 0x20; // 스위치5 OFF
    old_sw6 = PINE & 0x40; // 스위치6 OFF
    
    while (1) // 1Byte Write
    {   
        new_sw4 = PINE & 0x10;
        new_sw5 = PINE & 0x20;
        new_sw6 = PINE & 0x40;
        
        if((old_sw4 == 0x10) && (new_sw4 == 0x00))
        {   
            One_Byte_Write();
            delay_ms(100);
        }
        old_sw4 = new_sw4;
         
        if((old_sw5 == 0x20) && (new_sw5 == 0x00))
        {
            Random_Read();
            delay_ms(100);
        }
        old_sw5 = new_sw5; 
                
        if((old_sw6 != 0x40) && (new_sw6 == 0x00))  
        {
            Ten_Byte_Write();
            delay_ms(100);
        }
        old_sw6 = new_sw6;
    }
}

void START(void) // Falling Edge
{
    DAT_HIGH;
    delay_us(postpone);
    CLK_HIGH;
    delay_us(postpone);
    DAT_LOW;
    delay_us(postpone);
    CLK_LOW;
    delay_us(postpone);
}

void STOP(void) // Rising Edge
{
    SDA_OUT;
    DAT_LOW;
    delay_us(postpone);
    CLK_HIGH;
    delay_us(postpone);
    DAT_HIGH;
    delay_us(postpone);
    CLK_LOW;
    delay_us(postpone);
}

void Process_8data_Write(U8 value) // DATA를 먼저 입력한다(달리 말하면 DATA를 먼저 세팅한다). CLK HIGH가 DATA를 유지한다. 그리고 CLK이 LOW일 때 DATA를 변화시킨다.
{                                  // Data_Address, Address, Data 8bits 입력.
    U8 i = 0;                       // SDA_OUT
    U8 Data;

    SDA_OUT;

    Data = value;

    for (i = 0; i < 8; i++) // 7~0, 8번 반복.
    {
        if ((Data & 0x80) == 0x80) // 7bit(MSB)부터 0bit까지 DATA검사.
            DAT_HIGH;  //PORTD 1번핀을 high(1)
        else
            DAT_LOW; //반대(0)

        Data <<= 1; // 왼쪽 시프트 (그래야 6bit ~ 0bit까지 검사)

        CLK_HIGH;
        delay_us(postpone);
        CLK_LOW;
        delay_us(postpone);
    }

    //if((Data & 0x80) == 0x80) DAT_HIGH; //위의 i가 7일 경우 마지막 0번 비트를 출력하기 위해 사용.
    //else DAT_LOW;

    CLK_HIGH;
    delay_us(postpone);
    CLK_LOW;
    delay_us(postpone);

    if (Data == 0x80) DAT_LOW;  // 파형 오류 방지 작업.
    delay_us(20);

}

void Process_10byte_Write(U8* write_value)
{
    U8 i, j;
    U8 Data[10] = {0};

    for(i = 0; i < 10; i++)
    {
        Data[i] = write_value[i];

        for(j =0; j < 8; i++)
        {
            if((Data[i] & 0x80) == 0x80)
                DAT_HIGH;
            else
                DAT_LOW;

            CLK_HIGH;
            delay_us(12);
            CLK_LOW;
            delay_us(12);
        }
        
        ACK();
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

    for (i = 0; i < 50; i++) // 8번 동안 신호를 검사(8bit라서 8bit 검사), 50번 검사했을 때는 오류나서 i값 변경하였음.
    {
        if ((PIND & 0x02) == 0x00)
        {
            i = 49;
            PORTC = 0xFE; // LED 0번 ON
        }
        else
        {
            PORTC = 0xFF; // ALL OFF
        }
    }
    
    PORTC = 0xFD;
    SDA_OUT;
    delay_us(postpone);
    CLK_LOW;
    delay_us(postpone);

}

void no_ACK(void)  // Master -> Slave에게 명령
{
    SDA_OUT;
    DAT_HIGH;
    delay_us(postpone);
    CLK_HIGH;
    delay_us(postpone);
    CLK_LOW;
    delay_us(postpone);
}

void ACK_Seq(void)
{
    SDA_OUT;
    DAT_LOW;
    delay_us(12);
    CLK_HIGH;
    delay_us(12);
    CLK_LOW;
    delay_us(12);
}

void One_Byte_Write(void)
{
    START();
    Process_8data_Write(DEV_ADD_W); // 0xA0, Device_Address
    ACK();

    Process_8data_Write(IIC_ADD); // Address
    ACK();

    Process_8data_Write(IIC_DAT); // Data
    ACK();
    STOP();
}

void Ten_Byte_Write(void)
{
    START();
    Process_8data_Write(DEV_ADD_W);
    ACK();

    Process_8data_Write(IIC_ADD);
    ACK();

    Process_10byte_Write(Array_Write);
    STOP();
}

void Process_8data_Read(void)
{
    U8 i, Temp_data = 0;

    SDA_IN; // Slave -> Master에게 신호를 보냄

    for (i = 0; i < 8; i++)
    {
        CLK_LOW;
        delay_us(postpone);
        CLK_HIGH;
        delay_us(postpone);
        CLK_LOW;
        delay_us(postpone);


        Temp_data = (PIND & 0x02 == 0x00) ? 0x00 : 0x80; //PIND가 0x00이면 0x00, 0x02이면 0x80. MSB때문에 0x80사용
        Temp_data >>= i; // MSB부터 자리 정렬 shift
        READ_DATA |= Temp_data;

        CLK_HIGH;
        delay_us(postpone);
        CLK_LOW;
        delay_us(postpone);
    }
    if (IIC_DAT == READ_DATA)
    {
        PORTC = 0xF0;
    }
}

void Random_Read(void)
{
    START();
    Process_8data_Write(DEV_ADD_W);
    ACK();

    Process_8data_Write(IIC_ADD);
    ACK();

    START();
    Process_8data_Write(DEV_ADD_R);
    ACK();
    
    Process_8data_Read();
    no_ACK();
    STOP();

}