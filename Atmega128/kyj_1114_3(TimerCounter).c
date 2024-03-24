/*
 * kyj_1114_3(TimerCounter).c
 *
 * Created: 2023-11-14 오전 11:47:09
 * Author: user
 */

/*#include <mega128a.h>

unsigned char Shine = 0;
unsigned char Bright = 0;
unsigned char INT_BIT4 = 0;
unsigned char INT_BIT5 = 0;

void main(void)
{
    DDRC = 0xFF; // 포트 C 출력으로 설정
    PORTC = 0xFF; // 포트 C에 초기값 출력
    
    TIMSK = 0x42; // timer0 compare, timer2 overflow interrupt
	
	TCCR0 = 0x0F; // WGM01=1, CTC 모드, 1024분주
	OCR0 = 255; 
	TCNT0 = 0x0; // 타이머/카운터0 레지스터 초기값
	
	TCCR2 = 0x05; // overflow mode, 1024분주
	TCNT2 = 0x06; 
	
	SREG |= 0x80; // 전역 인터럽트 인에이블 비트 I set
    
    while (1)
    {   
        if(INT_BIT4 == 1) // Overflow
        {   
            INT_BIT4 = 0;
            if(Shine == 0)
            {
                PORTC.0 = 0;
                Shine = 1;
            }
            else
            {
                PORTC.0 = 1;
                Shine = 0;
            }   
        }       
        
        if(INT_BIT5 == 1) // Compare match // LED 1개 키고 끼고 1개 끼고 크고 
        {
            INT_BIT5 = 0;
            if(Bright == 0)
            {
                PORTC.7 = 0;
                Bright = 1;
            }
            else
            {
                PORTC.7 = 1;
                Bright = 0;
            }
        }               
    }
}

interrupt [TIM0_OVF] void timer_int0 (void)
{
    TCNT0 = 0x00; // initial value 재설정, 0이면 생략가능
    SREG &= 0x7F;
    INT_BIT4 = 1;
    SREG |= 0x80;
}

interrupt [TIM2_COMP] void timer_comp0 (void)
{
    TCNT2 = 0x06; // initial value 재설정, 0이면 생략가능
    SREG &= 0x7F;
    INT_BIT5 = 1;
    SREG |= 0x80;
}
*/


#include <mega128.h>
unsigned char BLINK0=0;
unsigned char BLINK2=0;

unsigned char TIMER2_OVF_INT_BIT =0;
unsigned char TIMER0_COMPARE_INT_BIT =0;


void main(void)
{ 
	DDRC = 0xFF; // 포트 C 출력으로 설정
	PORTC = 0xFF; // 포트 C에 초기값 출력
	
	TIMSK = 0x42; // timer0 compare, timer2 overflow interrupt
	
	TCCR0 = 0x0F; // WGM01=1, CTC 모드, 1024분주
	OCR0 = 255; 
	TCNT0 = 0x0; // 타이머/카운터0 레지스터 초기값
	
	TCCR2 = 0x05; // overflow mode, 1024분주
	TCNT2 = 6; 
	
	SREG |= 0x80; // 전역 인터럽트 인에이블 비트 I set
	
	while(1){
			if(TIMER0_COMPARE_INT_BIT==1){
				TIMER0_COMPARE_INT_BIT=0;
				if(BLINK0==0) {
					PORTC.0 = 0; // 포트 출력
					BLINK0=1;
				}
				else {
					PORTC.0 = 1; // 포트 출력
					BLINK0=0;
				}				
			}
			if(TIMER2_OVF_INT_BIT==1){
				TIMER2_OVF_INT_BIT=0;
				if(BLINK2==0) {
					PORTC.7 = 0; // 포트 출력
					BLINK2=1;
				}
				else {
					PORTC.7 = 1; // 포트 출력
					BLINK2=0;
				}				
			}		
		}	
}

interrupt [TIM0_COMP] void timer_comp0(void) 
{ 
	TCNT0 = 0x0;
	SREG &= 0x7F; // All Interrupt disable
	TIMER0_COMPARE_INT_BIT=1;
	SREG |= 0x80; // All Interrupt enable

}
interrupt [TIM2_OVF] void timer_ovf2(void) 
{ 
	TCNT2 = 6;
	SREG &= 0x7F; // All Interrupt disable
	TIMER2_OVF_INT_BIT=1;
	SREG |= 0x80; // All Interrupt enable
}
