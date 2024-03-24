/*
 * kyj_1113_3(INT).c
 *
 * Created: 2023-11-13 ¿ÀÈÄ 12:30:20
 * Author: user
 */

#include <mega128a.h>

const char seg_pat[10] = {0x3F, 0x06, 0x5b, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

unsigned char N1 = 0;
unsigned char EINT4_FLAG_BIT = 0;
unsigned char EINT5_FLAG_BIT = 0;

void main(void)
{
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    EIMSK = 0x30;
    EICRB = 0x0A;
    SREG |= 0x80;
    
    PORTG = 0x08;    

    while (1)
        {
           
        PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = ((seg_pat[N1] & 0x70) | (PORTB & 0x0F)); 
            
            if(EINT4_FLAG_BIT == 1)
            {  
                EINT4_FLAG_BIT = 0;
        
                N1 = (N1 + 1) % 10;
            }
            if(EINT5_FLAG_BIT == 1)
            {                   
                EINT5_FLAG_BIT = 0;
                
                if(N1 == 0) N1 = 9;
                else N1--;0   
            }
        }
}

interrupt [EXT_INT4] void external_int4(void)
{
    SREG &= 0x7F;
    
    EINT4_FLAG_BIT = 1;
    
    SREG |= 0x80;    
}

interrupt [EXT_INT5] void external_int5(void)
{
    SREG &= 0x7F;       
    
    EINT5_FLAG_BIT = 1;
    
    SREG |= 0x80;    
}