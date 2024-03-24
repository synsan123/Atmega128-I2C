/*
 * kyj_1114_2(TimerCounter).c
 *
 * Created: 2023-11-14 ���� 11:47:09
 * Author: user
 */

#include <mega128a.h>

unsigned char led = 0xFE;
unsigned char INT_BIT = 0;

void main(void)
{
    DDRC = 0xFF;
    PORTC = led;
    
    TIMSK = 0x02; // (1bit) OCIE0 = 1 (overflow interrupt enable)
    TCCR0 = 0x0F; // 1024 divide, CTC ��
    OCR0 = 0x9B;  // ��� �� �������Ͱ� (9.98ms ����� ���� ��), 155
    TCNT0 = 0x00; // Timer/Counter0 Register initial value, main���� �� �� ����, Interrupt���� �ٽ� �� �� ����.
    SREG |= 0x80;
    
    while (1)
    {   
        if(INT_BIT == 1)
        {
            INT_BIT = 0;
            led = led << 1;
            led = led | 0x01;
            if(led == 0xFF) led = 0xFE;
            PORTC = led;
        }             
    }
}

interrupt [TIM0_COMP] void timer_comp0 (void)
{
    TCNT0 = 0x00; // initial value �缳��, 0�̸� ��������
    SREG &= 0x7F;
    INT_BIT = 1;
    SREG |= 0x80;
}
