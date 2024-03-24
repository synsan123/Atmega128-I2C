/******************************************************************/
/* Project : kyj_1120_Project2.c                                  */
/* Name : Kim Yong Je                                             */
/* Date : 2023-11-20 ���� 12:25:43                                 */
/* Description : Project2                                         */
/******************************************************************/

#include <mega128a.h>
#include <delay.h>

typedef unsigned char U8; /* �ڷ��� ���ϰ� ������ ���� */
typedef unsigned int U32;

U8 Reception_ch; /* Ű���� �Է� ���� Atmega128 CPU�� �۽�. �޸� ���ϸ� CPU�� �Է� ���� ���� ���� */ 
U8 string1[] = "The UART Program Mode\r"; /* ���� ���ڿ� */
U8 string2[] = "External Interrupt4 : No 4\r"; /* ���� ���ڿ� */
U8 string3[] = "External Interrupt5 : No 5\r"; /* ���� ���ڿ� */
U8 string4[] = "External Interrupt6 : No 6\r"; /* ���� ���ڿ� */
U8 string5[] = "External Interrupt7 : No 7\r"; /* ���� ���ڿ� */
U8 string6[] = "Please Press The Number : "; /* ���� ���ڿ� */
U8 string7[] = "\r";
U8 string8[] = "\r\rADC Temperature Sending\r\r";
U8 string9[] = "\r\rADC Voltage Sending\r\r";
U8 string10[] = "\r\rFast Pwm wave Showing\r\r";
U8 string11[] = "\r\rPWM Phase Correct Showing\r\r";
U8 *pStr; // ���ڿ� ������

U8 seg_stop = 0; /* seg_stop = 0�̸� My_birthday ����. */
U8 LED_ON = 0xFE; /* LED_ON �ʱⰪ ����                */
U8 seg_array[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

U8 EXT_INT4_BIT = 0; /* INTERRUT4 Flag Bit */
U8 EXT_INT5_BIT = 0; /* INTERRUT4 Flag Bit */
U8 EXT_INT6_BIT = 0; /* INTERRUT4 Flag Bit */
U8 EXT_INT7_BIT = 0; /* INTERRUT4 Flag Bit */

U8 TIM2_OVF_COUNT_BIT = 0; /* TIM2_OVF_COUNT_BIT�� 10�� �Ǿ��� �� ADC_temperature ���� */
U8 TIM1_COMP_COUNT_BIT = 0; /* TIM1_COMP_COUNT_BIT�� 10�� �Ǿ��� �� ADC_voltage ����   */

U32 N4, N3, N2, N1, buffer = 0; /* Birthday �� 1000 , 100, 10, 1 �ڸ� �� �� ���� ���� */
U32 NT3, NT2, NT1, buf = 0; /* ADC_temp �� �ڸ� ��                                 */
U32 NV3, NV2, NV1, buff = 0; /* ADC_voltage �� �ڸ� ��                             */

U32 TIM2_OVF_BIT = 0; /* TIMER2 Flag Bit  */
U32 TIM1_COMP_BIT = 0; /* TIMER1 Flag Bit */

U32 ADC_vol = 0; /* ADC6���� Voltage ����     */
U32 ADC_temp = 0;/* ADC7���� Temperate ����  */

U32 My_year = 1996; 
U32 My_birth = 316;  

void My_birthday(U32);              /* Birthday�� segment�� ��Ÿ���� �Լ�           */
void ADC_temperature(U32 ADC_data); /* ADC_temp���� segment�� ��Ÿ���� �Լ�         */
void ADC_voltage(U32 ADC_val);      /* ADC_vol���� ��Ÿ���� �Լ�                   */
void UART_voltage_print(void);      /* ADC_vol���� UART �۽��� ���� PC�� ��Ÿ���� �Լ� */
void UART_temp_print(void);         /* �ð� �� 1���� �� 10�� ADC_Temp�� ��µǴ��� Ȯ���ϴ� �뵵 */

void ADC_6_Register(void);          /* ADC6 �������� ����                         */
void ADC_7_Register(void);          /* ADC7 �������� ����                         */
void Timer1_Register(void);         /* Timer1 �������� ����                       */
void Timer2_Register(void);         /* Timer2 �������� ����                       */
void Timer3_Register_Phase_Correct_mode2(void); /* �������� ����                  */
void Timer3_Register_PWM_mode6(void);          /*  �������� ����                  */

void Putch(U8 Send_data);           /* �۽� function */
void Str_func_main(void);
void new_line(void);
void ADC_Temperature_line(void);
void ADC_Voltage_line(void);
void Fast_PWM_line(void);
void PWM_Phase_Correct_line(void);
 
void main(void)
{
    DDRC = 0xFF; /* PORTC ��� ����. LED             */
    DDRB = 0xF0; /* PORTB ��� ����. B,D,G = Segment */ 
    DDRD = 0xF0; /* PORTD ��� ����                  */
    DDRG = 0x0F; /* PORTG ��� ����                  */
    DDRE = 0x00; /* PORTE �Է� ���� */
    
    PORTC = LED_ON; /* PORTC�� �ʱⰪ ����            */
    
    EIMSK = 0xF0; /* INT 4,5,6,7 enable           */
    EICRB = 0xBB; /* INT 4, 6 = rising edge, INT 5, 7 = falling edge  */

    My_birthday(My_year); /* Birth year ���         */
    My_birthday(My_birth); /* Birth month, day ���  */
    
    Str_func_main();   /* Main Menu ��� */
    
    SREG |= 0x80; /* Global interrupt enable */
    
    while(1)
    {
       switch(Reception_ch)
       {
       case '4' : 
            EIMSK = 0x10;   /* INT4 enable */
            EICRB = 0x03;   /* rising edge */
            ADC_Temperature_line();
            Reception_ch = 0;
            break;
       
       case '5' :
            EIMSK = 0x20;   /* INT5 enable */
            EICRB = 0x08;   /* falling edge */
            ADC_Voltage_line();
            Reception_ch = 0;
            break;
            
       case '6' :
            EIMSK = 0x40;   /* INT6 enable */
            EICRB = 0x30;   /* rising edge */
            Fast_PWM_line();
            delay_ms(2000);
            Str_func_main();
            Reception_ch = 0;
            break;
       
       case '7' :
            EIMSK = 0x80;   /* INT7 enable */
            EICRB = 0x80;   /* falling edge */
            PWM_Phase_Correct_line();
            delay_ms(2000);
            Str_func_main();
            Reception_ch = 0;
            break;
       
       default:
            break;
       }
       
       if(seg_stop == 0) My_birthday(My_birth); /* seg_stop = 0�̸� ����                    */
       else ADC_temperature(ADC_temp); /* seg_stop = 1�̸� My_birthday ���� �� ADC_value ��� */
        
       if(EXT_INT4_BIT == 1) /* Interrupt4�� �߻� �� ����ġ4�� ������ TIMER2�� �۵� */
       {
            EXT_INT4_BIT = 0;
            Timer2_Register(); /* Timer2 ���� */
       }
       if(EXT_INT5_BIT == 1)
       {
            EXT_INT5_BIT = 0;
            Timer1_Register(); /* Timer1 ���� */
       }
       if(EXT_INT6_BIT == 1)
       {
            EXT_INT6_BIT = 0;
            Timer3_Register_PWM_mode6();
       }
       if(EXT_INT7_BIT == 1)
       {
            EXT_INT7_BIT = 0;
            Timer3_Register_Phase_Correct_mode2();
       }     
       if(TIM2_OVF_BIT >= 79) /* 3160ms�� ���� �� ���� ADC_temp�� ����ϰ� COUNT�� 10�̻��� �Ǿ��� �� �ٽ� My_birthday�� ����Ѵ� */
       {
            TIM2_OVF_BIT = 0;
            TIM2_OVF_COUNT_BIT++;
            seg_stop = 1;      /* My_birthday ���� */
            ADC_7_Register(); /* ADC7 Temperature ���� */
            UART_temp_print();
            if(TIM2_OVF_COUNT_BIT >= 10)
            {
                TIM2_OVF_COUNT_BIT = 0;
                delay_ms(100);
                new_line();
                Str_func_main();
                TIMSK &= 0xBF;   /* timer2 exit */
                TCCR2 = 0x00;   /* timer2 exit */
                EIMSK &= 0xEF; /* INT4 exit   */
                seg_stop = 0; /* ADC_temperature ����, My_birthday ��� */
            }    
       }
       if(TIM1_COMP_BIT >= 10) /* 10000ms�� ���� �� ���� ADC_voltage�� �����ϰ� UART�� ���� PC�� ��� */
       {
            TIM1_COMP_BIT = 0;
            ADC_6_Register();
            UART_voltage_print();
            TIM1_COMP_COUNT_BIT++;
            if(TIM1_COMP_COUNT_BIT >= 10) 
            {   
                TIM1_COMP_COUNT_BIT = 0;
                delay_ms(300);
                new_line();
                Str_func_main();
                TCCR1B = 0x00; /* timer1 exit */
                TIMSK &= 0xEF; /* timer1 exit */
                EIMSK &= 0xDF; /* INT5 exit   */
            }               
       }      
    }
}

/*********************************** My_birthday, ADC_temperature, ADC_Voltage, UART Function****************************/

void My_birthday(U32 Birthday_data)
{
    U8 i = 0;
    
    N4 = Birthday_data / 1000;
    buffer = Birthday_data % 1000;
    
    N3 = buffer / 100;
    buffer = buffer % 100;
    
    N2 = buffer / 10;
    N1 = buffer % 10;
    
    for(i = 0; i < 50; i++)
    {    
        PORTG = 0x08;
        PORTD = ((seg_array[N1] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_array[N1] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    
        PORTG = 0x04;
        PORTD = ((seg_array[N2] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_array[N2] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    
        PORTG = 0x02;
        PORTD = ((seg_array[N3] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_array[N3] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    
        PORTG = 0x01;
        PORTD = ((seg_array[N4] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_array[N4] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    } 
}

void ADC_temperature(U32 ADC_data)
{ 
    float fval;
    int ival;
     
    fval = (float)ADC_data * 5.0 / 1023.0; /* ���а����� ��ȯ */
    ival = (int)(fval * 1000.0 + 0.5); /* �ݿø� �� ����ȭ    */
        
    NT3 = ival / 100; /* ������ ���� */
    buf = ival % 100;
     
    NT2 = buf / 10; /* �Ҽ� ù° �ڸ� ���� */
    NT2 = buf % 10; /* �Ҽ� ��° �ڸ� ���� */

    PORTG = 0x08; /* PG3=1, �Ҽ� ��° �ڸ� */
    PORTD = ((seg_array[NT1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_array[NT1] & 0x70 ) | (PORTB & 0x0F);
    delay_ms(10);
    
    PORTG = 0x04; /* PG2=1, �Ҽ� ù° �ڸ� */
    PORTD = ((seg_array[NT2] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_array[NT2] & 0x70 ) | (PORTB & 0x0F);
    PORTB = PORTB | 0x80; /* DP on(�Ҽ���) */ 
    delay_ms(10); 
    
    PORTG = 0x02; /* PG1=1, ������ */
    PORTD = ((seg_array[NT3] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_array[NT3] & 0x70 ) | (PORTB & 0x0F);  
    delay_ms(10);
}

void ADC_voltage(U32 ADC_val)
{ 
    float FVAL;
    int IVAL;
     
    FVAL = (float)ADC_val * 5.0 / 1023.0; /* ���а����� ��ȯ */
    IVAL = (int)(FVAL * 100.0 + 0.5); /* �ݿø� �� ����ȭ */
        
    NV3 = IVAL / 100; /* ������ ���� */
    buff = IVAL % 100;
     
    NV2 = buff / 10; /* �Ҽ� ù° �ڸ� ���� */
    NV1 = buff % 10; /* �Ҽ� ��° �ڸ� ���� */
}

void UART_voltage_print(void)
{
    UCSR0A = 0x00; /* UART �ʱ�ȭ */
    UCSR0B = 0x08; /* �۽� enable */
    UCSR0C = 0x06; /* �񵿱�, 8bit data, no parity, stop 1bit */
    UBRR0H = 0x00;
    UBRR0L = 0x67; /* Baud rate 9600 */
    
    while((UCSR0A & 0x20) == 0x00); /* buffer�� data�� ��� �� ������ ��� */
    UDR0 = NV3 + 0x30;             /* read�� ���� PC�� write = ����, ASCII �ڵ� ������ �۽�. */
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = '.'; 
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = NV2 + 0x30; 
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = NV1 + 0x30; 
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = 'V';
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = '\r'; // new line   
}


void UART_temp_print(void) /* 10�� ��µǴ��� Ȯ�ο� */
{
    UCSR0A = 0x00; /* UART �ʱ�ȭ */
    UCSR0B = 0x08; /* �۽� enable */
    UCSR0C = 0x06; /* �񵿱�, 8bit data, no parity, stop 1bit */
    UBRR0H = 0x00;
    UBRR0L = 0x67; /* Baud rate 9600 */
    
    while((UCSR0A & 0x20) == 0x00); /* buffer�� data�� ��� �� ������ ��� */
    UDR0 = NT3 + 0x30;             /* read�� ���� PC�� write = ����, ASCII �ڵ� ������ �۽�. */
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = NT2 + 0x30;
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = '.';  
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = NT1 + 0x30; 
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = 'V';
    while((UCSR0A & 0x20) == 0x00); 
    UDR0 = '\r'; /* new line */   
}       

/*********************************** TIMER, ADC Register Function **********************************************/

void Timer3_Register_PWM_mode6(void)
{
    TCCR3A = 0x82; /* 9bit PWM mode 6, OC3A(PE3) ��� */ 
    TCCR3B = 0x0C; /* 9bit PWM mode 6, 256 divider, 122.0703Hz */
    TCCR3C = 0x00;
    TCNT3L = 0x00;
    TCNT3H = 0x00;
    OCR3AL = 0x52; /* 82 = 0x52, duty rate = 16%, 511(9bit TOP) * 0.16 = 81.76 */
    OCR3AH = 0x00;   
    DDRE |= 0x08; /* PWM������ ����ϱ� ���� ��� */
}

void Timer3_Register_Phase_Correct_mode2(void)
{
    TCCR3A = 0x82; /* 9bit Phase correct mode 2, OC3A(PE3) ��� */ 
    TCCR3B = 0x04; /* 9bit Phase correct mode 2, 256 divider, 122.0703Hz / 2 = 61.03515Hz */
    TCCR3C = 0x00;
    TCNT3L = 0x00;
    TCNT3H = 0x00;
    OCR3AL = 0x52; /* 82 = 0x52, duty rate = 16%, 511(9bit TOP) * 0.16 = 81.76 */
    OCR3AH = 0x00;   
    DDRE |= 0x08; /* PWM������ ����ϱ� ���� ��� */
}
void Timer2_Register(void)
{
    TIMSK |= 0x40; /* TIMER2 Overflow mode */
    TCCR2 = 0x04; /* 256 divider, 4ms */
    TCNT2 = 0x06; /* decade = 6 */
}

void Timer1_Register(void)
{
    TIMSK |= 0x10; /* TIMER1 CTC mode, OCRA ��� */
    TCCR1A = 0x00; /* TIMER1 CTC mode           */
    TCCR1B = 0x0B; /* TIMER1 CTC mode, 64 divider */
    TCCR1C = 0x00;
    TCNT1L = 0x00; /* TCNT1 �ʱⰪ ���� */
    TCNT1H = 0x00;
    OCR1AH = 0x61;
    OCR1AL = 0xA7; /* 24999 = 0x61A7, 100ms */
}

void ADC_6_Register(void) /* ADC_Voltage */
{
    ADMUX = 0x06; /* ADC6 �ܱؼ� �Է� */
    /* ADCSRA = 0x86; */ /* �����ص� �ǰ� ���ص� ������ �ʿ���� */
    ADCSRA = 0xC6; /* ADSC = 1 ��ȯ ����. ADEN = 1, 128 divider */
    while((ADCSRA & 0x10) == 0); /* single mode ���, ADIF = 1�� �� �� ����(write �� �� ����) */   
    ADC_vol = (int)ADCL + ((int)ADCH << 8);
    ADC_voltage(ADC_vol);
}

void ADC_7_Register(void) /* ADC_temperature */
{
    ADMUX = 0x07; /* ADC7 �ܱؼ� */
    ADCSRA = 0xE7; /* Free Running Mode, 128 divider */
    //delay_us(100); /* ADC ���� ����, Free Running mode�� ���� �ʿ����, while((ADCSRA & 0x10) == 0)�� ���� �� �� ADCSRA�� 4�� ��Ʈ�� �׾������ wthile���� �� �������� */ 
    ADC_temp = (int)ADCL + ((int)ADCH << 8); /* A/D�� �б� */
}

/********************************* Interrupt, TIMER Interrupt Function **********************************************/

interrupt [EXT_INT4] void external_int4(void)
{
    SREG &= 0x7F;
    EXT_INT4_BIT = 1;    
    SREG |= 0x80;
}

interrupt [EXT_INT5] void external_int5(void)
{
    SREG &= 0x7F;
    EXT_INT5_BIT = 1;    
    SREG |= 0x80;
}
    
interrupt [EXT_INT6] void external_int6(void)
{
    SREG &= 0x7F;
    EXT_INT6_BIT = 1;    
    SREG |= 0x80;
}

interrupt [EXT_INT7] void external_int7(void)
{
    SREG &= 0x7F;
    EXT_INT7_BIT = 1;    
    SREG |= 0x80;
}

interrupt [TIM2_OVF] void Timer2_OVF(void)
{
    TCNT2 = 0x06; /* 4ms */
    SREG &= 0x7F;
    TIM2_OVF_BIT++; /* 4ms */
    SREG |= 0x80;
}

interrupt [TIM1_COMPA] void timer1_comp(void) 
{ 
    TCNT1L = 0x00; /* TCNT1 �ʱⰪ ���� */
    TCNT1H = 0x00;
    SREG &= 0x7F;
    TIM1_COMP_BIT++;
    SREG |= 0x80;
}

interrupt [USART0_RXC] void usart0_rx(void) // ���� �Ϸ� ���ͷ�Ʈ, keyboard �Է� �� �Է� ���� atmega128 ���� ���ۿ� ���Եǰ�, atmega128�� �� ���� �д´�.
{
    SREG &= 0x7F; // All Interrupt disable 
    Reception_ch = UDR0; // ����
    //while((UCSR0A & 0x20) == 0x0); // UERE0=1 �� ������ ��� => atmega128�� PC�� �����͸� �۽�(����)��(= ���� 0���� 1�� �ٲ�鼭 while�� Ż��). Terminal�� ��Ÿ��. 
    //UDR0 = ch; // �۽�
    SREG |= 0x80; // All Interrupt enable
}

/*************************************** UART �۽�, Main Menu �۽� Function ******************************************/

void Putch(U8 Send_data)
{
   while((UCSR0A & 0x20) == 0x00);  /* data ���� �� ���� ��� */
   UDR0 = Send_data;               /* data �۽� */
}

void Str_func_main(void)
{
    UCSR0A = 0x00; /* UART �ʱ�ȭ */
    UCSR0B = 0x98; /* ���� �Ϸ� Interrupt enable, ���� enable, �۽� enable */ 
    UCSR0C = 0x06; /* �񵿱�, 8bit data, no parity, stop 1bit */
    UBRR0H = 0x00;
    UBRR0L = 0x67; /* Baud rate 9600 */
    
    pStr = string1;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string2;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string3;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string4;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string5;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string6;
    while(*pStr != 0) Putch(*pStr++);
}

void new_line(void)
{
    pStr = string7;
    while(*pStr != 0) Putch(*pStr++);
}

void ADC_Temperature_line(void)
{
    pStr = string8;
    while(*pStr != 0) Putch(*pStr++);
}

void ADC_Voltage_line(void)
{
    pStr = string9;
    while(*pStr != 0) Putch(*pStr++);
}

void Fast_PWM_line(void)
{
    pStr = string10;
    while(*pStr != 0) Putch(*pStr++);
}

void PWM_Phase_Correct_line(void)
{
    pStr = string11;
    while(*pStr != 0) Putch(*pStr++);
}    

