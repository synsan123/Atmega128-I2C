/*
 * kyj_1114_1(TimerCounter).c
 *
 * Created: 2023-11-14 오전 11:47:09
 * Author: user
 */

#include <mega128a.h>

unsigned char led = 0xFE;
unsigned char INT_BIT = 0;

void main(void)
{
    DDRC = 0xFF;
    PORTC = led;
    
    TIMSK = 0x01; // TOIE0 = 1 (overflow interrupt enable)
    TCCR0 = 0x07; // Overflow mode = normal mode, 1024 divide
    TCNT0 = 0x25; // Timer/Counter0 Register initial value, main에서 한 번 선언, Interrupt에서 다시 한 번 선언.
    SREG |= 0x80;
    
    while (1)
    {   
        if(INT_BIT == 1)
        {
        INT_BIT = 0;
        led = led << 1;
        led = led | 0x01;
        if(led == 0xFF) led = 0xFE;
        PORTC = led;
        }             
    }
}

interrupt [TIM0_OVF] void timer_int0 (void)
{
    TCNT0 = 0x25; // initial value 재설정, decade = 37
    SREG &= 0x7F;
    INT_BIT = 1;
    SREG |= 0x80;
}
