/*
 * kyj_1110_1(Segment).c
 *
 * Created: 2023-11-10 오후 2:03:03
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>
#define N 16
unsigned char Segment[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void main(void)
{
    unsigned char i, value, k;
    
    DDRD = 0xF0; /* PORTD 4,5,6,7 */
    DDRB = 0xF0; /* PORTB 4,5,6,7 */
    DDRG = 0x0F; /* PORTG 0,1,2,3 */
    
    PORTG = 0x0F;
  

    while (1)
    {   
        for(k = 0; k < N; k++)
        {
            value = Segment[k];        
            PORTD = (value & 0x0F) << 4;
            PORTB = (value & 0xF0);
            //PORTD = Segment[i] << 4;   /* PORTD = (Segment[i] & 0x0F) << 4 교수님은 이거 사용. */
           // PORTB = Segment[i] & 0xF0; /* PORTB = (Segment[i] & 0x70) */ 
            delay_ms(50);
        }
          
    }
}

/*
 * kyj_1110_2(Segment).c
 *
 * Created: 2023-11-10 오후 2:47:23
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>
#define N 10
void Segment_two(int);

unsigned char Segment[N] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void main(void)
{
    unsigned char number = 0;

    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    PORTD = 0x00;
    PORTB = 0x00;
    
    while (1)
    {
        Segment_two(number);
        number++;
        if(number > 99) number = 0;    

    }
}

void Segment_two(int number)
{
    int i, N2, N1;
    
    N2 = number / 10;
    N1 = number % 10;
    
    for(i = 0; i < 49; i++)
    {   
        PORTG = 0x02;
        PORTD = ((Segment[N2] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N2] & 0xF0) | (PORTB & 0x0F);
        delay_ms(10);
        
        PORTG = 0x01;
        PORTD = Segment[N1] << 4;
        PORTB = Segment[N1] & 0xF0;
        delay_ms(10);    
    }


}

