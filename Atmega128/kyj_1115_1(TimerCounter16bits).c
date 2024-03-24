/*
 * kyj_1115_1(TimerCounter16bits).c
 *
 * Created: 2023-11-15 ¿ÀÀü 10:44:45
 * Author: user
 */

#include <mega128a.h>

typedef unsigned char U8;

/*U8 EINT_BIT4 = 0;
U8 EINT_BIT5 = 0;
U8 EINT_BIT6 = 0;
U8 EINT_BIT7 = 0;  */
U8 TIM1_OVF_INT_BIT =0;
/*U8 TIM1_COMP_INT_BIT =0;
U8 TIM3_OVF_INT_BIT =0;
U8 TIM3_COMP_INT_BIT =0;*/
U8 led = 0xFE;

void main(void)
{
    DDRC = 0xFF;
    PORTC = led;
    
    TIMSK = 0x04; // overflow, timer 1
    
    TCCR1A = 0x00; // overflow mode
    TCCR1B = 0x04; // overflow mode, clock/256 divider
    TCCR1C = 0x00;
    
    TCNT1H = 0x80;
    TCNT1L = 0x00;
    
    SREG |= 0x80;
    
    while (1)
    {   
        if(TIM1_OVF_INT_BIT == 1)
        {
            TIM1_OVF_INT_BIT = 0;
            led <<= 1;
            led |= 0x01;
            if(led == 0xFF) led = 0xFE;
            PORTC = led;
        }    
    }
}

interrupt [TIM1_OVF] void timer1_overflow (void)
{
    TCNT1H = 0x80;
    TCNT1L = 0x00;
    SREG &= 0x7F;
    TIM1_OVF_INT_BIT = 1;
    SREG |= 0x80;    
}
