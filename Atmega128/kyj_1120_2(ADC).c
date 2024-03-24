/*
 * kyj_1120_2(ADC).c
 *
 * Created: 2023-11-20 오전 10:40:50
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

typedef unsigned char U8;

U8 seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

void AD_display(int);

void main(void)
{
    int AD_value;
    
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    ADMUX = 0x07; // ADC6 사용
    ADCSRA = 0xE7; // free running
    delay_ms(5);
    
    while (1)
    {
        AD_value = (int)ADCL + ((int)ADCH << 8);
        AD_display(AD_value);
        delay_ms(2);
    }
}
void AD_display(int val) 
{ 
    float fval;
    int ival, buf; 
    U8 N100, N10, N1;
    
    fval = (float)val * 5.0 / 1023.0; // 전압값으로 변환
    ival = (int)(fval * 1000.0 + 0.5); // 반올림 후 정수화
    
    N100 = ival / 100; // 정수부 추출
    buf = ival % 100;
     
    N10 = buf / 10; // 소수 첫째 자리 추출
    N1 = buf % 10; // 소수 둘째 자리 추출
    
    PORTG = 0b00001000; // PG3=1, 소수 둘째 자리
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);
    delay_ms(1);
    
    PORTG = 0b00000100; // PG2=1, 소수 첫째 자리
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F);
    PORTB = PORTB | 0x80; // DP on(소수점) 
    delay_ms(1);
     
    PORTG = 0b00000010; // PG1=1, 정수부
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F);  
    delay_ms(1);
}
