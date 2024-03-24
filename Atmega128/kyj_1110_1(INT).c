/*
 * kyj_1110_1(INT).c
 *
 * Created: 2023-11-13 ¿ÀÀü 11:33:51
 * Author: user
 */

#include <mega128a.h>

void main(void)
{
    DDRC = 0xFF; // PORTC Output
    
    PORTC = 0xFF; // LED Number 0 On
    
    EIMSK = 0xF0; // External Interrupt 4,5,6,7 Enable
    EICRB = 0xAF; // External Interrupt 4,6 falling Edge, 7,5 = Rising Edge
    SREG |= 0x80; // Global Interrupt Enable set
    
    while (1);
    
}

interrupt [EXT_INT4] void external_int4 (void)
{
    SREG &= 0x7F; // Global Interrupt Enable clear
    
    PORTC = 0xAA;
    
    SREG |= 0x80;
}

interrupt [EXT_INT5] void external_int5 (void)
{
    SREG &= 0x7F; // Global Interrupt Enable clear
    
    PORTC = 0x55;
    
    SREG |= 0x80;
}

interrupt [EXT_INT6] void external_int6 (void)
{
    SREG &= 0x7F; // Global Interrupt Enable clear
    
    PORTC = 0xF0;
    
    SREG |= 0x80;
}

interrupt [EXT_INT7] void external_int7 (void)
{
    SREG &= 0x7F; // Global Interrupt Enable clear
    
    PORTC = 0x0F;
    
    SREG |= 0x80;
}
