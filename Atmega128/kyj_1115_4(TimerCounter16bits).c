/*
 * kyj_1115_4(TimerCounter16bits).c
 *
 * Created: 2023-11-15 오후 2:33:44
 * Author: user
 */

#include <mega128a.h>

unsigned int pwm = 0x0200; // 현재의 출력비교 레지스터 값 저장
unsigned char INT_BIT4 = 0;
unsigned char INT_BIT5 = 0;

void main(void)
{
    DDRB = 0xFF; //OC1C(PB7)핀 출력방향 설정
    DDRG = 0xFF; //7-segment ON/OFF
    PORTG = 0x0F; //7-segment all On
    
    EIMSK = 0x30; //INT4,5
    EICRB = 0x0A; //falling edge(4,5)
    
    TCCR1A = 0x0B; // phase correct PWM
    TCCR1B = 0x03; // 64 // 256 divider
    TCCR1C = 0x00;
    
    TCNT1 = 0x00; // timer1 register initial valid set
    
    OCR1CH = (pwm & 0xFF00) >> 8; // 0x02
    OCR1CL = pwm & 0x00FF; // 0x00
    
    SREG |= 0x80; // Global interrupt enable I set
    
    while (1)
    {
        if(INT_BIT4 == 1)
        {
            INT_BIT4 = 0;
            if(pwm < 0x03B0) pwm += 0x0040;  // 944보다 작으면 증가
            OCR1CH = (pwm & 0xFF00) >> 8; // 0x02
            OCR1CL = pwm & 0x00FF; // 0x00
        }
        if(INT_BIT5 == 1)
        {
            INT_BIT5 = 0;
            if(pwm > 0x0050) pwm -= 0x0040; // 80보다 크면 감소
            OCR1CH = (pwm & 0xFF00) >> 8; // 0x02 // 값 갱신
            OCR1CL = pwm & 0x00FF; // 0x00
        }

    }
}

interrupt [EXT_INT4] void external_int4 (void)
{
    TCNT1 = 0x00; // timer1 register initial valid set
    SREG &= 0x7F;
    INT_BIT4 = 1;
    SREG |= 0x80;
    
    /*if(pwm < 0x03B0) pwm += 0x0040;  // 944보다 작으면 증가
    OCR1CH = (pwm & 0xFF00) >> 8; // 0x02
    OCR1CL = pwm & 0x00FF; // 0x00*/
}

interrupt [EXT_INT5] void external_int5 (void)
{
    TCNT1 = 0x00; // timer1 register initial valid set
    SREG &= 0x7F;
    INT_BIT5 = 1;
    SREG |= 0x80;
    
    /*if(pwm > 0x0050) pwm -= 0x0040; // 80보다 크면 감소
    OCR1CH = (pwm & 0xFF00) >> 8; // 0x02
    OCR1CL = pwm & 0x00FF; // 0x00*/
}
