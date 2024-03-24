/*
 * kyj_1115_3(TimerCounter16bits).c
 *
 * Created: 2023-11-15 ¿ÀÈÄ 2:15:08
 * Author: user
 */

#include <mega128a.h>

unsigned char TIM3_OVF_INT_BIT = 0;
unsigned char led = 0xFE;

void main(void)
{
    DDRC = 0xFF;
    PORTC = 0xFF;
    
    ETIMSK = 0x04; // Timer3 overflow
    TCCR3A = 0x00; // overflow mode                    
    TCCR3B = 0x06; // overflow mode, falling edge counting
    TCCR3C = 0x00;
    
    TCNT3H = 0xFF; // start point 0xFFFF
    TCNT3L = 0xFF;
    SREG |= 0x80;
    
    while (1)
    {
        if(TIM3_OVF_INT_BIT == 1)
        {   
            TIM3_OVF_INT_BIT = 0;
            led = led << 1;
            led = led | 0x01;
            if(led == 0xFF) led = 0xFE;
            PORTC = led;    
        }

    }
}

interrupt [TIM3_OVF] void timer_overflow (void)
{
    TCNT3H = 0xFF; // start point 0xFFFF
    TCNT3L = 0xFF;
    SREG &= 0x7F;
    TIM3_OVF_INT_BIT = 1; 
    SREG |= 0x80;
}
