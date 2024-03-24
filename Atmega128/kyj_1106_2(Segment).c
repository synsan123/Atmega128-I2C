/*
 * kyj_1106_2(Segment).c
 *
 * Created: 2023-11-06 ¿ÀÀü 9:25:16
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

void main(void)
{
    char segment_on, i;
    
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    PORTB = 0xF0;
    PORTD = 0xF0;
    
    while (1)
        {   segment_on = 0x08;
            for(i = 0; i < 4; i++)
            {
            PORTG = (segment_on & 0x0F);
            delay_ms(500);
            segment_on = segment_on >> 1;
            }
        }
}
