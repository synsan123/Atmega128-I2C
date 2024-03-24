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
    
    for(i = 0; i < 49; i++) // 두 수가 한 번에 출력이 되지 않기 때문에 반복을 통해 두 수를 보이게 하는 눈속임
    {   
        PORTG = 0x04;
        PORTD = ((Segment[N2] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N2] & 0xF0) | (PORTB & 0x0F);
        delay_ms(10);
        
        PORTG = 0x08;
        PORTD = Segment[N1] << 4;
        PORTB = Segment[N1] & 0xF0;
        delay_ms(10);    
    }

}