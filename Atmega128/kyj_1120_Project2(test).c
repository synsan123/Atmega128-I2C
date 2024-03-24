/*
 * kyj_1120_Project2(test).c
 *
 * Created: 2023-11-20 오후 4:04:03
 * Author: user
 */

#include <mega128.h>
unsigned char led = 0xFE;
unsigned char INT4_BIT = 0;
int TIM2_BIT = 0;
void main(void)
{ 
DDRC = 0xFF; // 포트 C 출력으로 설정
PORTC = led; // 포트 C에 초기값 출
EIMSK = 0x10;
EICRB = 0x03;
SREG = 0x80; // 전역 인터럽트 인에이블 비트 I set
while(1)
{
    if(INT4_BIT == 1)
    {
        INT4_BIT = 0;
        TIMSK = 0x40; // OCIE0 = 1(출력 비교 인터럽트 인에이블)
        TCCR2 = 0x05; // WGM01=1, CTC 모드, 1024분주
        TCNT2 = 0x00; // 타이머/카운터0 레지스터 초기값
    }
    if(TIM2_BIT >= 100)
    {
        TIM2_BIT = 0;
        led = led << 1; // 1비트 쉬프트
        led = led | 0x01; // 최하위 비트 set
        if(led == 0xFF) led = 0xFE; // 모두 off이면 초기값 재설정
        PORTC = led; // 포트 출력
    }   
    
}
}
// 타이머/카운터0 출력비교(TCNT0 = OCRO 일때) 인터럽트 서비스 루틴
// 인터럽트 발생 주기 1/16us * 1024분주 * (1 + 155) = 9.98ms
interrupt [TIM2_OVF] void timer_ovf(void) 
{ 
 TCNT2 = 0x00;
 SREG &= 0x7F;
 TIM2_BIT++;
 SREG |= 0x80;
}
interrupt [EXT_INT4] void external_int4(void)
{
 SREG &= 0x7F;
 INT4_BIT = 1;
 SREG |= 0x80;
}
