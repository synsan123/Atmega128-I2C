/*
 * kyj_1106_1(Segment).c
 *
 * Created: 2023-11-06 ¿ÀÀü 9:01:02
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

void main(void)
{
    char i, segment;
    
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    PORTB = 0x00;
    PORTD = 0x00;
    PORTG = 0x08;
    
    while (1)
        {   
            segment = 0x01;
            
            for(i = 0; i < 8; i++)
            {
                PORTD = (segment & 0x0F) << 4;
                PORTB = (segment & 0xF0);
                delay_ms(500);
                segment = segment << 1;
            }
        }
}
