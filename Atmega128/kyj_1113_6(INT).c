/*
 * kyj_1113_4(INT).c
 *
 * Created: 2023-11-13 ���� 2:03:01
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

typedef unsigned char U8;

const U8 seg_pat[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f};

U8 N1 = 0, N10 = 0, N100 = 0, N1000 = 0; // 0 : 1�ڸ�, 1 : 10�ڸ� , 2 : 100�ڸ� , 3 : 1000�ڸ�
U8 pos = 0;
U8 hour = 12, min = 0, sec = 0;

U8 External_INT4_BIT = 0;
U8 External_INT5_BIT = 0;

void Time_out(void);

void main(void)
{
    U8 i;
    
    DDRB |= 0xF0;
    DDRD |= 0xF0;
    DDRG |= 0x0F;
    
    EIMSK = 0x30;
    EICRB = 0x0A;
    SREG |= 0x80;
    
    while(1)
    {        
        for(i = 0; i < 40; i++)
        {
            Time_out();
        }// end of for
        
        sec += 1;
        
        if(sec == 60)
        {
            sec = 0;
            min += 1;
            
            if(min == 60)
            {
                min = 0;
                hour = (hour + 1) % 24;
            }// end of min
            
        }// end of sec
        
        if(External_INT4_BIT == 1)
        {
            External_INT4_BIT = 0;
            
            if(pos == 0) N1 = (N1 + 1) % 10; // ���� �� 1���� ǥ�� +1 ��
            else if(pos == 1) N10 = (N10 + 1) % 6; // ���� �� 10���� ǥ�� +1 �� (60��)
            else if(pos == 2) 
            { // �ð� 1���� +1
                if(N1000 == 2) N100 = (N100 + 1) % 4; // 24�ð��� ���ڸ� 2�̹Ƿ� 20,21,22,23,24
                else N100 = (N100 + 1) % 10;
            }// end of pos == 2
            else 
            {   // pos=3 (�� �� ������)
                if(N100 < 4) N1000 = (N1000 + 1) % 3; // N100<4�� 04 (00,01,02,03), (10,11,12,13), 
                // else if(N1000 != 1) N1000 = (N1000 + 1) % 3; 
                else N1000 = (N1000 + 1) % 2;
            }// end of else
            
            hour = N1000 * 10 + N100;
            min = N10 * 10 + N1;
        }    
        if(External_INT5_BIT == 1)   // �ڸ� �� ����
        {
            External_INT5_BIT = 0;
            pos = (pos + 1) % 4;
        }// end of INT5_BIT
        
        
    }// end of while    

}// end of main

void Time_out(void)
{
    N1 = min % 10; // ( 0 ~ 9�� )
    N10 = min / 10; // (0 ~ 50��, 10�� �ڸ�)
    N100 = hour % 10; // (0 ~ 9�ð�)
    N1000 = hour / 10; // (0 ~ 20�ð�, 10�� �ڸ�)
    
    PORTG = 0x08;
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70) | (PORTD & 0x0F);
    delay_ms(6);
    
    PORTG = 0x04;
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70) | (PORTD & 0x0F);
    delay_ms(6);
    
    PORTG = 0x02;
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70) | (PORTD & 0x0F);
    delay_ms(6);
    
    PORTG = 0x01;
    PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1000] & 0x70) | (PORTD & 0x0F);
    delay_ms(6);
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