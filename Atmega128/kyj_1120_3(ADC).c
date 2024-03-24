/*
 * kyj_1120_3(ADC).c
 * LM35DZ 온도 센서.
 * Created: 2023-11-20 오전 11:18:30
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

typedef unsigned char u_char;

const u_char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
int TIM0_INT_BIT = 0;
int ad_val = 0;

void AD_disp(int); // A/D 변환값 표시

void main(void)
{ 
    DDRB = 0xF0; // 포트 B 상위 4비트 출력 설정
    DDRD = 0xF0; // 포트 D 상위 4비트 출력 설정
    DDRG = 0x0F; // 포트 G 하위 4비트 출력 설정
    
    TIMSK = 0x01; // timer0 overflow
    TCCR0 = 0x05; // 128 divider
    TCNT0 = 0x83; // 131 , 1sec
    
    ADMUX = 0x07; // ADC7 단극성 입력 선택
    ADCSRA = 0x87; // ADEN=1, ADSC=1, ADFR=1, 16MHz, 128분주 -> 125kHz
    
    SREG |= 0x80;
    
    while(1)
    {   
        AD_disp(ad_val); // A/D값 표시
        
        if(TIM0_INT_BIT >= 1000)
        {
            TIM0_INT_BIT = 0;
            ADCSRA = 0xC7;
            
            while((ADCSRA & 0x10) == 0);
        
            ad_val = (int)ADCL + ((int)ADCH << 8); // A/D값 읽기
        
        }  
                 
    }
} 


void AD_disp(int val) 
{ 
    float fval;
    int ival, buf; 
    u_char N100, N10, N1;

    fval = (float)val * 5.0 / 1023.0; // 전압값으로 변환
    ival = (int)(fval * 1000.0 + 0.5); // 반올림 후 정수화

    N100 = ival / 100; // 정수부 추출
    buf = ival % 100;
     
    N10 = buf / 10; // 소수 첫째 자리 추출
    N1 = buf % 10; // 소수 둘째 자리 추출

    PORTG = 0b00001000; // PG3=1, 소수 둘째 자리
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);
    delay_ms(10);
    
    PORTG = 0b00000100; // PG2=1, 소수 첫째 자리
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F);
    PORTB = PORTB | 0x80; // DP on(소수점) 
    delay_ms(10); 
    
    PORTG = 0b00000010; // PG1=1, 정수부
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F);  
    delay_ms(10);
}

interrupt [TIM0_OVF] void timer_ovf (void)
{
    TCNT0 = 0x83;
    SREG |= 0x80;
    TIM0_INT_BIT++;
    SREG &= 0x7F;
}