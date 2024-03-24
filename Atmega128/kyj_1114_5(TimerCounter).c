/*
 * kyj_1114_5(TimerCounter).c
 *
 * Created: 2023-11-14 오후 2:46:37
 * Author: user
 */

#include <mega128a.h>

unsigned char INT_BIT4 = 0; // Interrupt 발생
unsigned char INT_BIT5 = 0;
unsigned char INT_BIT6 = 0;
unsigned char INT_BIT7 = 0;
unsigned char TIM2_COMP_FLAG_BIT = 0;
unsigned char TIM0_OVF_FLAG_BIT = 0;
unsigned char led4 = 0xAA; // Timer2 LED
unsigned char led6 = 0xF0; // Timer0 LED
unsigned char count = 0; // Timer2 count
unsigned char A_count = 0; // Timer0 count
 
void main(void)
{
    DDRC = 0xFF;
    PORTC = 0xFF;
    
    EIMSK = 0xF0; // 4,5,6,7 INT
    EICRB = 0xEE; // 7,5 = rising. 6,4 = falling
    
   /* TIMSK = 0x81; // timer2 compare, timer1 overflow
    TCNT0 = 0x00; // 초기값 0
    
    OCR2 =  0xFF; // 255
    TCNT2 = 0x00; // 초기값 0  */
    
    SREG |= 0x80; // Global INT enable I set
    
    while (1)
    {
        if(INT_BIT4 == 1) // 704ms, 16.384 * 43번, Falling Edge, Timer2    // end 조건. Error
        {
            TIMSK = 0x80; // timer2 compare match
            OCR2 = 0xFF; // 255
            TCNT2 = 0x00;
            TCCR2 = 0x0D; //compare match, 1024 divide
            INT_BIT4 = 0;    
        }
        if(INT_BIT5 == 1) // Rising Edge, Timer2 Exit
        {
            TCCR2 = 0x00;
            INT_BIT5 = 0;
            PORTC = 0xFF;
        }
        if(INT_BIT6 == 1) // 130ms, 16.384ms * 8번, Falling Egde, Timer0 시작
        {
            TIMSK = 0x01; // timer1 overflow
            TCNT0 = 0x00;
            TCCR0 = 0x07; // overflow, 1024 divide
            INT_BIT6 = 0; 
        }
        if(INT_BIT7 == 1) // Rising Edge, Timer0 Exit
        {
            TCCR0 = 0x00;
            INT_BIT7 = 0;
            PORTC = 0xFF;
        }
        if(TIM2_COMP_FLAG_BIT == 1)
        {
            TIM2_COMP_FLAG_BIT = 0;
            count++; 
            if(count == 43)
            {
                led4 = led4 ^ 0xFF;
                PORTC = led4;
                count = 0;
            }
        }
        if(TIM0_OVF_FLAG_BIT == 1)
        {
            TIM0_OVF_FLAG_BIT = 0;
            A_count++; 
            if(A_count == 5)
            {
                led6 = led6 ^ 0xFF;
                PORTC = led6;
                count = 0;
            }
        }
    }
}

//Interrupt 4, 5, 6 ,7 선언.
interrupt [EXT_INT4] void INT_4 (void)
{
    SREG &= 0x7F;
    INT_BIT4 = 1;
    SREG |= 0x80;
}

interrupt [EXT_INT5] void INT_5 (void)
{
    SREG &= 0x7F;
    INT_BIT5 = 1;
    SREG |= 0x80;    
}

interrupt [EXT_INT6] void INT_6 (void)
{
    SREG &= 0x7F;
    INT_BIT6 = 1;
    SREG |= 0x80;
}

interrupt [EXT_INT7] void INT_7 (void)
{
    SREG &= 0x7F;
    INT_BIT7 = 1;
    SREG |= 0x80;
}

interrupt [TIM2_COMP] void timer_comp (void) // Timer2_Compare Match Interrupt 선언 
{
    TCNT2 = 0x00;
    SREG &= 0x7F;
    TIM2_COMP_FLAG_BIT = 1;
    SREG |= 0x80;
}

interrupt [TIM0_OVF] void timer_ovf (void) // Timer0_Overflow Interrupt 선언
{
    TCNT0 = 0x00;
    SREG &= 0x7F;
    TIM0_OVF_FLAG_BIT = 1;
    SREG |= 0x80;
}


