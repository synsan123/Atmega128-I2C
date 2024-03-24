/******************************************************************/
/* Project : kyj_1120_Project2.c                                  */
/* Name : Kim Yong Je                                             */
/* Date : 2023-11-20 오후 12:25:43                                 */
/* Description : Project2                                         */
/******************************************************************/

#include <mega128a.h>
#include <delay.h>

typedef unsigned char U8; /* 자료형 편하게 쓸려고 선언 */
typedef unsigned int U32;

U8 Reception_ch; /* 키보드 입력 값이 Atmega128 CPU로 송신. 달리 말하면 CPU가 입력 값을 수신 받음 */ 
U8 string1[] = "The UART Program Mode\r"; /* 전송 문자열 */
U8 string2[] = "External Interrupt4 : No 4\r"; /* 전송 문자열 */
U8 string3[] = "External Interrupt5 : No 5\r"; /* 전송 문자열 */
U8 string4[] = "External Interrupt6 : No 6\r"; /* 전송 문자열 */
U8 string5[] = "External Interrupt7 : No 7\r"; /* 전송 문자열 */
U8 string6[] = "Please Press The Number : "; /* 전송 문자열 */
U8 string7[] = "\r";
U8 string8[] = "\r\rADC Temperature Sending\r\r";
U8 string9[] = "\r\rADC Voltage Sending\r\r";
U8 string10[] = "\r\rFast Pwm wave Showing\r\r";
U8 string11[] = "\r\rPWM Phase Correct Showing\r\r";
U8 *pStr; // 문자열 포인터

U8 seg_stop = 0; /* seg_stop = 0이면 My_birthday 종료. */
U8 LED_ON = 0xFE; /* LED_ON 초기값 설정                */
U8 seg_array[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

U8 EXT_INT4_BIT = 0; /* INTERRUT4 Flag Bit */
U8 EXT_INT5_BIT = 0; /* INTERRUT4 Flag Bit */
U8 EXT_INT6_BIT = 0; /* INTERRUT4 Flag Bit */
U8 EXT_INT7_BIT = 0; /* INTERRUT4 Flag Bit */

U8 TIM2_OVF_COUNT_BIT = 0; /* TIM2_OVF_COUNT_BIT가 10이 되었을 때 ADC_temperature 정지 */
U8 TIM1_COMP_COUNT_BIT = 0; /* TIM1_COMP_COUNT_BIT가 10이 되었을 때 ADC_voltage 종료   */

U32 N4, N3, N2, N1, buffer = 0; /* Birthday 값 1000 , 100, 10, 1 자리 수 및 버퍼 설정 */
U32 NT3, NT2, NT1, buf = 0; /* ADC_temp 값 자리 수                                 */
U32 NV3, NV2, NV1, buff = 0; /* ADC_voltage 값 자리 수                             */

U32 TIM2_OVF_BIT = 0; /* TIMER2 Flag Bit  */
U32 TIM1_COMP_BIT = 0; /* TIMER1 Flag Bit */

U32 ADC_vol = 0; /* ADC6단자 Voltage 변수     */
U32 ADC_temp = 0;/* ADC7단자 Temperate 변수  */

U32 My_year = 1996; 
U32 My_birth = 316;  

void My_birthday(U32);              /* Birthday를 segment에 나타내는 함수           */
void ADC_temperature(U32 ADC_data); /* ADC_temp값을 segment에 나타내는 함수         */
void ADC_voltage(U32 ADC_val);      /* ADC_vol값을 나타내는 함수                   */
void UART_voltage_print(void);      /* ADC_vol값을 UART 송신을 통해 PC에 나타내는 함수 */
void UART_temp_print(void);         /* 시간 당 1번씩 총 10번 ADC_Temp가 출력되는지 확인하는 용도 */

void ADC_6_Register(void);          /* ADC6 레지스터 선언                         */
void ADC_7_Register(void);          /* ADC7 레지스터 선언                         */
void Timer1_Register(void);         /* Timer1 레지스터 선언                       */
void Timer2_Register(void);         /* Timer2 레지스터 선언                       */
void Timer3_Register_Phase_Correct_mode2(void); /* 레지스터 선언                  */
void Timer3_Register_PWM_mode6(void);          /*  레지스터 선언                  */

void Putch(U8 Send_data);           /* 송신 function */
void Str_func_main(void);
void new_line(void);
void ADC_Temperature_line(void);
void ADC_Voltage_line(void);
void Fast_PWM_line(void);
void PWM_Phase_Correct_line(void);
 
void main(void)
{
    DDRC = 0xFF; /* PORTC 출력 설정. LED             */
    DDRB = 0xF0; /* PORTB 출력 설정. B,D,G = Segment */ 
    DDRD = 0xF0; /* PORTD 출력 설정                  */
    DDRG = 0x0F; /* PORTG 출력 설정                  */
    DDRE = 0x00; /* PORTE 입력 설정 */
    
    PORTC = LED_ON; /* PORTC에 초기값 설정            */
    
    EIMSK = 0xF0; /* INT 4,5,6,7 enable           */
    EICRB = 0xBB; /* INT 4, 6 = rising edge, INT 5, 7 = falling edge  */

    My_birthday(My_year); /* Birth year 출력         */
    My_birthday(My_birth); /* Birth month, day 출력  */
    
    Str_func_main();   /* Main Menu 출력 */
    
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
       
       if(seg_stop == 0) My_birthday(My_birth); /* seg_stop = 0이면 유지                    */
       else ADC_temperature(ADC_temp); /* seg_stop = 1이면 My_birthday 종료 및 ADC_value 출력 */
        
       if(EXT_INT4_BIT == 1) /* Interrupt4번 발생 시 스위치4를 누르면 TIMER2가 작동 */
       {
            EXT_INT4_BIT = 0;
            Timer2_Register(); /* Timer2 동작 */
       }
       if(EXT_INT5_BIT == 1)
       {
            EXT_INT5_BIT = 0;
            Timer1_Register(); /* Timer1 동작 */
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
       if(TIM2_OVF_BIT >= 79) /* 3160ms이 지날 때 마다 ADC_temp를 출력하고 COUNT가 10이상이 되었을 때 다시 My_birthday를 출력한다 */
       {
            TIM2_OVF_BIT = 0;
            TIM2_OVF_COUNT_BIT++;
            seg_stop = 1;      /* My_birthday 끄기 */
            ADC_7_Register(); /* ADC7 Temperature 동작 */
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
                seg_stop = 0; /* ADC_temperature 종료, My_birthday 출력 */
            }    
       }
       if(TIM1_COMP_BIT >= 10) /* 10000ms가 지날 때 마다 ADC_voltage를 측정하고 UART를 통해 PC에 출력 */
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
     
    fval = (float)ADC_data * 5.0 / 1023.0; /* 전압값으로 변환 */
    ival = (int)(fval * 1000.0 + 0.5); /* 반올림 후 정수화    */
        
    NT3 = ival / 100; /* 정수부 추출 */
    buf = ival % 100;
     
    NT2 = buf / 10; /* 소수 첫째 자리 추출 */
    NT2 = buf % 10; /* 소수 둘째 자리 추출 */

    PORTG = 0x08; /* PG3=1, 소수 둘째 자리 */
    PORTD = ((seg_array[NT1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_array[NT1] & 0x70 ) | (PORTB & 0x0F);
    delay_ms(10);
    
    PORTG = 0x04; /* PG2=1, 소수 첫째 자리 */
    PORTD = ((seg_array[NT2] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_array[NT2] & 0x70 ) | (PORTB & 0x0F);
    PORTB = PORTB | 0x80; /* DP on(소수점) */ 
    delay_ms(10); 
    
    PORTG = 0x02; /* PG1=1, 정수부 */
    PORTD = ((seg_array[NT3] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_array[NT3] & 0x70 ) | (PORTB & 0x0F);  
    delay_ms(10);
}

void ADC_voltage(U32 ADC_val)
{ 
    float FVAL;
    int IVAL;
     
    FVAL = (float)ADC_val * 5.0 / 1023.0; /* 전압값으로 변환 */
    IVAL = (int)(FVAL * 100.0 + 0.5); /* 반올림 후 정수화 */
        
    NV3 = IVAL / 100; /* 정수부 추출 */
    buff = IVAL % 100;
     
    NV2 = buff / 10; /* 소수 첫째 자리 추출 */
    NV1 = buff % 10; /* 소수 둘째 자리 추출 */
}

void UART_voltage_print(void)
{
    UCSR0A = 0x00; /* UART 초기화 */
    UCSR0B = 0x08; /* 송신 enable */
    UCSR0C = 0x06; /* 비동기, 8bit data, no parity, stop 1bit */
    UBRR0H = 0x00;
    UBRR0L = 0x67; /* Baud rate 9600 */
    
    while((UCSR0A & 0x20) == 0x00); /* buffer에 data가 들어 올 때까지 대기 */
    UDR0 = NV3 + 0x30;             /* read된 값을 PC에 write = 전송, ASCII 코드 값으로 송신. */
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


void UART_temp_print(void) /* 10번 출력되는지 확인용 */
{
    UCSR0A = 0x00; /* UART 초기화 */
    UCSR0B = 0x08; /* 송신 enable */
    UCSR0C = 0x06; /* 비동기, 8bit data, no parity, stop 1bit */
    UBRR0H = 0x00;
    UBRR0L = 0x67; /* Baud rate 9600 */
    
    while((UCSR0A & 0x20) == 0x00); /* buffer에 data가 들어 올 때까지 대기 */
    UDR0 = NT3 + 0x30;             /* read된 값을 PC에 write = 전송, ASCII 코드 값으로 송신. */
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
    TCCR3A = 0x82; /* 9bit PWM mode 6, OC3A(PE3) 사용 */ 
    TCCR3B = 0x0C; /* 9bit PWM mode 6, 256 divider, 122.0703Hz */
    TCCR3C = 0x00;
    TCNT3L = 0x00;
    TCNT3H = 0x00;
    OCR3AL = 0x52; /* 82 = 0x52, duty rate = 16%, 511(9bit TOP) * 0.16 = 81.76 */
    OCR3AH = 0x00;   
    DDRE |= 0x08; /* PWM파형을 출력하기 위해 사용 */
}

void Timer3_Register_Phase_Correct_mode2(void)
{
    TCCR3A = 0x82; /* 9bit Phase correct mode 2, OC3A(PE3) 사용 */ 
    TCCR3B = 0x04; /* 9bit Phase correct mode 2, 256 divider, 122.0703Hz / 2 = 61.03515Hz */
    TCCR3C = 0x00;
    TCNT3L = 0x00;
    TCNT3H = 0x00;
    OCR3AL = 0x52; /* 82 = 0x52, duty rate = 16%, 511(9bit TOP) * 0.16 = 81.76 */
    OCR3AH = 0x00;   
    DDRE |= 0x08; /* PWM파형을 출력하기 위해 사용 */
}
void Timer2_Register(void)
{
    TIMSK |= 0x40; /* TIMER2 Overflow mode */
    TCCR2 = 0x04; /* 256 divider, 4ms */
    TCNT2 = 0x06; /* decade = 6 */
}

void Timer1_Register(void)
{
    TIMSK |= 0x10; /* TIMER1 CTC mode, OCRA 사용 */
    TCCR1A = 0x00; /* TIMER1 CTC mode           */
    TCCR1B = 0x0B; /* TIMER1 CTC mode, 64 divider */
    TCCR1C = 0x00;
    TCNT1L = 0x00; /* TCNT1 초기값 설정 */
    TCNT1H = 0x00;
    OCR1AH = 0x61;
    OCR1AL = 0xA7; /* 24999 = 0x61A7, 100ms */
}

void ADC_6_Register(void) /* ADC_Voltage */
{
    ADMUX = 0x06; /* ADC6 단극성 입력 */
    /* ADCSRA = 0x86; */ /* 선언해도 되고 안해도 되지만 필요없음 */
    ADCSRA = 0xC6; /* ADSC = 1 변환 시작. ADEN = 1, 128 divider */
    while((ADCSRA & 0x10) == 0); /* single mode 사용, ADIF = 1이 될 때 까지(write 될 때 까지) */   
    ADC_vol = (int)ADCL + ((int)ADCH << 8);
    ADC_voltage(ADC_vol);
}

void ADC_7_Register(void) /* ADC_temperature */
{
    ADMUX = 0x07; /* ADC7 단극성 */
    ADCSRA = 0xE7; /* Free Running Mode, 128 divider */
    //delay_us(100); /* ADC 오류 방지, Free Running mode라서 딱히 필요없음, while((ADCSRA & 0x10) == 0)을 쓰게 될 시 ADCSRA의 4번 비트가 죽어버려서 wthile문을 못 빠져나옴 */ 
    ADC_temp = (int)ADCL + ((int)ADCH << 8); /* A/D값 읽기 */
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
    TCNT1L = 0x00; /* TCNT1 초기값 설정 */
    TCNT1H = 0x00;
    SREG &= 0x7F;
    TIM1_COMP_BIT++;
    SREG |= 0x80;
}

interrupt [USART0_RXC] void usart0_rx(void) // 수신 완료 인터럽트, keyboard 입력 시 입력 값이 atmega128 수신 버퍼에 삽입되고, atmega128은 그 값을 읽는다.
{
    SREG &= 0x7F; // All Interrupt disable 
    Reception_ch = UDR0; // 수신
    //while((UCSR0A & 0x20) == 0x0); // UERE0=1 될 때까지 대기 => atmega128이 PC에 데이터를 송신(전송)함(= 값이 0에서 1로 바뀌면서 while문 탈출). Terminal에 나타남. 
    //UDR0 = ch; // 송신
    SREG |= 0x80; // All Interrupt enable
}

/*************************************** UART 송신, Main Menu 송신 Function ******************************************/

void Putch(U8 Send_data)
{
   while((UCSR0A & 0x20) == 0x00);  /* data 들어올 때 까지 대기 */
   UDR0 = Send_data;               /* data 송신 */
}

void Str_func_main(void)
{
    UCSR0A = 0x00; /* UART 초기화 */
    UCSR0B = 0x98; /* 수신 완료 Interrupt enable, 수신 enable, 송신 enable */ 
    UCSR0C = 0x06; /* 비동기, 8bit data, no parity, stop 1bit */
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

