/*
 * kyj_1113_4(INT).c
 *
 * Created: 2023-11-13 오후 2:03:01
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

typedef unsigned char U8;

const U8 seg_pat[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f};

U8 N1 = 0, N10 = 0, N100 = 0, N1000 = 0; // 0 : 1자리, 1 : 10자리 , 2 : 100자리 , 3 : 1000자리
U8 pos = 0;

U8 External_INT4_BIT = 0;
U8 External_INT5_BIT = 0;

void Seg4_out(void);

void main(void)
{
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    EIMSK = 0x30;
    EICRB = 0x0A;
    SREG |= 0x80;
    
    while(1)
    {   
        Seg4_out();
        if(External_INT4_BIT == 1)
        {   
            External_INT4_BIT = 0;
            if(pos == 0) N1 = (N1 + 1) % 10;   
            else if(pos == 1) N10 = (N10 + 1) % 10;
            else if(pos == 2)  N100 = (N100 + 1) % 10; 
            else N1000 = (N1000 + 1) % 10;
        }
        if(External_INT5_BIT == 1)
        {   
            External_INT5_BIT = 0;
            pos = (pos + 1) % 4;  
        }
    }

}

void Seg4_out(void)
{
        PORTG = 0x08;
        PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_pat[N1] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    
        PORTG = 0x04;
        PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_pat[N10] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    
        PORTG = 0x02;
        PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_pat[N100] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
    
        PORTG = 0x01;
        PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (seg_pat[N1000] & 0x70) | (PORTD & 0x0F);
        delay_ms(10);
}

interrupt [EXT_INT4] void external_int4(void)
{
    SREG &= 0x7F;
    External_INT4_BIT = 1;    
    SREG |= 0x80;
}

interrupt [EXT_INT5] void external_int5(void)
{
    SREG &= 0x7F;
    External_INT5_BIT = 1;    
    SREG |= 0x80;
} 