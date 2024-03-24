#include <mega128.h> 
#include <delay.h> 

#define CLK_HIGH    PORTD |= 0x01   // PD.0=1 Ŭ�� 1
#define CLK_LOW     PORTD &= 0xFE   // PD.0=0 Ŭ�� 0
#define DAT_HIGH    PORTD |= 0x02   // PD.1=1 ������ 1
#define DAT_LOW     PORTD &= 0xFD   // PD.1=0 ������ 0

#define SCL_OUT     DDRD |= 0x01    // PD.0=1 Ŭ�� ��¸��
#define SDA_IN      DDRD &= 0xFD    // PD.1=0 ������ �Է¸��
#define SDA_OUT     DDRD |= 0x02    // PD.1=1 ������ ��¸��

typedef unsigned char U8;           // ������ Ÿ�� ���ȭ

U8 DEV_ADD_W = 0xA0;                // ����̽� ��巹�� ������
U8 DEV_ADD_R = 0xA1;                // ����̽� ��巹�� �б���
U8 IIC_ADD = 0xAA;                  // ��巹�� ����
U8 IIC_DAT = 0x55;                  // �� ������ ��
U8 READ_DATA = 0;                   // �о�� ������ ��

U8 EINT4_FLAG_BIT=0; 
U8 EINT5_FLAG_BIT=0;  
U8 EINT6_FLAG_BIT=0;  
U8 EINT7_FLAG_BIT=0;  

U8 TIMER2_COMPARE_INT_BIT =0;
U8 TIMER0_OVF_INT_BIT =0;

U8 BLINK2=0;
U8 BLINK0=0;
                                    
void IIC_START(void);               // START �Լ�
void IIC_STOP(void);                // STOP �Լ�
void ACK_write(void);               // ACK �Լ�  master -> slave
void no_ACK(void);                  // NACK �Լ� master -> slave
void ACK_read(void);                // ACK �Լ�  slave -> master
void Process_8data(U8 value);       // Bytewrite 8��Ʈ ������ ����
void Process_8data_read(void);      // Randomread 8��Ʈ ������ �ޱ� 
void Byte_write(void);              // Bytewrite �Լ� 
void Random_read(void);             // Randomread �Լ�

void main(void)
{ 
    //while�� �������� Global interrupt�� �����ϴ� ���� ���� ����.
    DDRC = 0xFF; // ��Ʈ C ��� ����
    PORTC = 0xFF; // ��Ʈ C�� �ʱⰪ ���
    
    SCL_OUT;     // Ŭ�� ���
    SDA_OUT;    // ������ ���          

    EIMSK = 0b111110000; // �ܺ� ���ͷ�Ʈ4 enable
    EICRB = 0b10101110; // �ܺ� ���ͷ�Ʈ4 level trigger
    SREG |= 0x80; // ���� ���ͷ�Ʈ enable-bit set
    
    while(1){
        // ����͵� ������.
       // SCL_OUT;     // Ŭ�� ���
       // SDA_OUT;    // ������ ���          
    
        
        if(EINT4_FLAG_BIT==1) {
            EINT4_FLAG_BIT = 0; 
            TCCR2 = 0x0D; // compare match 0704-7.04, 1024 divider
            OCR2 = 155; // 9.984ms 
            TCNT2 = 0x0; // Ÿ�̸�/ī����0 �������� �ʱⰪ
            TIMSK |= 0x80;
        }
        if(EINT5_FLAG_BIT==1) {
            EINT5_FLAG_BIT=0;
			TCCR2 &= 0xF8;
        }
        if(EINT6_FLAG_BIT==1) 
        {   
            EINT6_FLAG_BIT = 0; 
            Byte_write();
        }
        if(EINT7_FLAG_BIT==1) {
            EINT7_FLAG_BIT=0;
            TCCR0 = 0x07; // OVF 0130- 1.30, 1024 divier 
            TCNT0 = 0xB2; // Ÿ�̸�/ī����0 �������� �ʱⰪ, 4.992ms, 178
            TIMSK |= 0x01;
        }        
        
        if(TIMER2_COMPARE_INT_BIT==100) {
            TIMER2_COMPARE_INT_BIT=0;
            if(BLINK2==0) {
                PORTC=0xAA;
                BLINK2=1;
            }
            else {
                PORTC=0x55;
                BLINK2=0;                
            }
        }
        if(TIMER0_OVF_INT_BIT==100) {
            TIMER0_OVF_INT_BIT=0;
            Byte_write(); 
            if(BLINK0==0) {
                PORTC=0xF0;
                BLINK0=1;
            }
            else {
                PORTC=0x0F;
                BLINK0=0;                
            }    
        }        
    }
}

interrupt [EXT_INT4] void external_int4(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT4_FLAG_BIT=1;
    
    SREG |= 0x80; // All Interrupt enable
}
interrupt [EXT_INT5] void external_int5(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT5_FLAG_BIT=1;
    
    SREG |= 0x80; // All Interrupt enable
}
interrupt [EXT_INT6] void external_int6(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT6_FLAG_BIT=1;
    
    SREG |= 0x80; // All Interrupt enable
}
interrupt [EXT_INT7] void external_int7(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT7_FLAG_BIT=1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [TIM2_COMP] void timer_comp2(void) 
{ 
    TCNT2 = 0x00;
    SREG &= 0x7F; // All Interrupt disable
    TIMER2_COMPARE_INT_BIT++;
    SREG |= 0x80; // All Interrupt enable

}
interrupt [TIM0_OVF] void timer_ovf0(void) 
{ 
    TCNT0 = 0xB2;
    SREG &= 0x7F; // All Interrupt disable
    TIMER0_OVF_INT_BIT++;
    SREG |= 0x80; // All Interrupt enable
}

void IIC_START(void){               //START �Լ�
    DAT_HIGH;                       //8MHZ 8000KHz-> 100Khz 1clock = 0.00001s -> 10us 
    delay_us(6);                    // delay_us(3)*4 =12us ������� ������ ���Ƿν������� 7-8us�� ������ ������ �߰�.
    CLK_HIGH;   
    delay_us(6);
    DAT_LOW;    
    delay_us(6);
    CLK_LOW;    
    delay_us(6);
}//end of IIC_START

void IIC_STOP(void){               //STOP �Լ�
    SDA_OUT;                       //������ ���
    DAT_LOW;  
    delay_us(6);
    CLK_HIGH;   
    delay_us(6);
    DAT_HIGH;   
    delay_us(6);
    CLK_LOW;    
    delay_us(6);
}//end of IIC_STOP

void Process_8data(U8 value){    // Bytewrite 8��Ʈ ������ ����
    U8 i_8bit;                   // 8��Ʈ ������ ���ֱ����� �ݺ��� ����
    U8 value_buffer;             // �� ������ ���۰�     
    value_buffer=value;
    SDA_OUT;                     //������ ���
    for(i_8bit=0;i_8bit<7;i_8bit++){ // 7��Ʈ ������ ���ֱ����� �ݺ���

        if((value_buffer&0x80)==0x80)DAT_HIGH; // ���� ���� �ֻ��� ��Ʈ�� 1�̸� ������ HIGH
        else DAT_LOW;                          // �ƴϸ� LOW
        value_buffer<<=1;                      // MSB-> LSB���� �ϳ���  shift
        delay_us(6);
        CLK_HIGH;  
        delay_us(12);
        CLK_LOW; 
        delay_us(6);       
    }//end of for      
    if((value_buffer&0x80)==0x80)DAT_HIGH;     // LSB ���� ���� ���� �ݺ�������  value_buffer�� MSB�� ������ LSB���� ����           
    else DAT_LOW;            
    delay_us(6);
    CLK_HIGH;  
    delay_us(12);
    CLK_LOW;  
    if(value_buffer==0x80) DAT_LOW;           // data high �̸� ACK���� clk�� high�϶� �Է¹����Ƿ�  
    delay_us(6);                              // clk low �϶� �����Ͱ� ����Ǵ� ��������� ���� clk high �Ǳ��� data ���������η� low ����     
}//end of Process_8data

void Process_8data_read(void){                // Randomread 8��Ʈ ������ �ޱ� 
    
    U8 i, TEMP_READ_DATA = 0;
    SDA_IN;

    for (i = 0; i < 8; i++){                  // 8��Ʈ������ �б�
        CLK_LOW;
        delay_us(6);
        CLK_HIGH;
        delay_us(12);
        CLK_LOW;  
        delay_us(6);
        TEMP_READ_DATA = (PIND & 0x02 == 0x00) ? 0x00 : 0x80;// ������ ���� ���� 0�̸� 0x00 �ƴϸ� 0x80
        TEMP_READ_DATA >>= i;                                // MSB���� �ڸ� ���� shift
        READ_DATA |= TEMP_READ_DATA;                         // ���� �����Ͱ� ����
    }//end of for  
    if(IIC_DAT==READ_DATA)PORTC=0xf0;        // write ���� read ���� ��ġ�ϸ� led ����    
}//end of Process_8data_read

void ACK_read(void){                         // ACK �Լ�  slave -> master
    U8 i = 0;                                // ACK Ȯ�ιޱ����� �ݺ��� ����

    SDA_IN;                                  // ������ �Է�
    CLK_HIGH;                                // ACK ��ȣ �ޱ��غ�
    for(i=10;i>0;i--){                       // 10�� �ݺ���  ACK ������ �ݺ��� ���� ����
        if((PIND & 0x02) == 0x00){i=1;
        PORTC=0xf1;
        }
        else PORTC=0x00;                     // NACK �޾Ҵٸ� LED �� ����                           
    }//end of for
    delay_us(12);
    CLK_LOW;        
    SDA_OUT;                                 //������ ���
    delay_us(6);      
}//end of ACK_read

void ACK_write(void){     // ACK �Լ�  master -> slave
    SDA_OUT;              // ������ ���
    DAT_LOW;    
    CLK_HIGH;  
    delay_us(12);
    CLK_LOW; 
    delay_us(6);   
}//end of ACK_write

void no_ACK(void){       // NACK �Լ�  master -> slave
    SDA_OUT;  
    DAT_HIGH;    
    CLK_HIGH;  
    delay_us(12);
    CLK_LOW; 
    delay_us(6);  
}//end of no_ACK

void Byte_write(void){        // Bytewrite �Լ� 
    IIC_START();              // start
    Process_8data(DEV_ADD_W); // device address 7bit�� write 
    ACK_read();               // ack
    Process_8data(IIC_ADD);   // address 8bits
    ACK_read();               // ack
    Process_8data(IIC_DAT);   // data 8bits
    ACK_read();               // ack
    IIC_STOP();               // stop  
}//end of Byte_write   

void Random_read(void){      // Randomread �Լ�
    IIC_START();             // start
    Process_8data(DEV_ADD_W);// device address 7bit�� write 0
    ACK_read();              // ack
    Process_8data(IIC_ADD);  // address 8bits
    ACK_read();              // ack
    IIC_START();             // start
    Process_8data(DEV_ADD_R);// device address 7bit�� read 1
    ACK_read();              // ack
    Process_8data_read();    // data 8bits read
    no_ACK();                // nack ������ �׸� �ֱ� master->slave
    IIC_STOP();              // stop 
}//end of Random_read