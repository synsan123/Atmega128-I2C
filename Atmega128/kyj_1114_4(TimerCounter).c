/*
 * kyj_1114_4(TimerCounter).c
 *
 * Created: 2023-11-14 오후 2:07:23
 * Author: user
 */

#include <mega128a.h>

unsigned char led = 0xFE;
int count = 0;
unsigned char INT_BIT = 1;

void main(void) // 0.5초 = 500ms임. 16.384 * 31번 반복.
{
    DDRC = 0xFF;
    PORTC = led;
    
    TIMSK = 0x01;
    TCCR0 = 0x06;
    TCNT0 = 0x3A;
    
    SREG |= 0x80;

    while (1)
    {
        if(INT_BIT == 1)
        {   
            INT_BIT = 0;
            count++;
            if(count == 630)
            {
                led = led ^ 0x01;
                PORTC = led;
                count = 0;
            }
        }   
    }
    
}

interrupt [TIM0_OVF] void timer_int0 (void)
{
    TCNT0 = 0x3A;
    SREG &= 0x7F;
    INT_BIT = 1;
    SREG |= 0x80;
}
