/*
 * kyj_1110_2(Segment).c
 *
 * Created: 2023-11-10 오후 2:47:23
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>
#define N 16
void Segment_two(int);

unsigned char Segment[N] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void main(void)
{
    int number = 0;

    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    PORTD = 0x00;
    PORTB = 0x00;
    
    while (1)
    {
        Segment_two(number);
        number++;
        if(number > 0xFFFF) number = 0;    
    }
}

void Segment_two(int number)
{
    int i, N4, N3,N2, N1, buffer;
    
    N4 = number / 0x1000;
    buffer = number % 0x1000;
    
    N3 = buffer / 0x100;
    buffer = buffer % 0x100;
    
    N2 = buffer / 0x10; // 0 ~ FF 까지 나타낼 수 있도록 0x10으로 나눔. 0x10이 16이니까
    N1 = buffer % 0x10;
    
    for(i = 0; i < 5; i++) // 두 수가 한 번에 출력이 되지 않기 때문에 반복을 통해 두 수를 보이게 하는 눈속임
    {   
        PORTG = 0x01;
        PORTD = ((Segment[N4] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N4] & 0x70) | (PORTB & 0x0F);
        delay_ms(2);
        
        PORTG = 0x02;
        PORTD = ((Segment[N3] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N3] & 0x70) | (PORTB & 0x0F);
        delay_ms(2);
        
        PORTG = 0x04;
        PORTD = ((Segment[N2] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N2] & 0x70) | (PORTB & 0x0F);
        delay_ms(2);
        
        PORTG = 0x08;
        PORTD = ((Segment[N1] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N1] & 0x70) | (PORTB & 0x0F);
        delay_ms(2);    
    }

}
