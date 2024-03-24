/*
 * kyj_1114_3(TimerCounter).c
 *
 * Created: 2023-11-14 ���� 11:47:09
 * Author: user
 */

/*#include <mega128a.h>

unsigned char Shine = 0;
unsigned char Bright = 0;
unsigned char INT_BIT4 = 0;
unsigned char INT_BIT5 = 0;

void main(void)
{
    DDRC = 0xFF; // ��Ʈ C ������� ����
    PORTC = 0xFF; // ��Ʈ C�� �ʱⰪ ���
    
    TIMSK = 0x42; // timer0 compare, timer2 overflow interrupt
	
	TCCR0 = 0x0F; // WGM01=1, CTC ���, 1024����
	OCR0 = 255; 
	TCNT0 = 0x0; // Ÿ�̸�/ī����0 �������� �ʱⰪ
	
	TCCR2 = 0x05; // overflow mode, 1024����
	TCNT2 = 0x06; 
	
	SREG |= 0x80; // ���� ���ͷ�Ʈ �ο��̺� ��Ʈ I set
    
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
        
        if(INT_BIT5 == 1) // Compare match // LED 1�� Ű�� ���� 1�� ���� ũ�� 
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
    TCNT0 = 0x00; // initial value �缳��, 0�̸� ��������
    SREG &= 0x7F;
    INT_BIT4 = 1;
    SREG |= 0x80;
}

interrupt [TIM2_COMP] void timer_comp0 (void)
{
    TCNT2 = 0x06; // initial value �缳��, 0�̸� ��������
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
	DDRC = 0xFF; // ��Ʈ C ������� ����
	PORTC = 0xFF; // ��Ʈ C�� �ʱⰪ ���
	
	TIMSK = 0x42; // timer0 compare, timer2 overflow interrupt
	
	TCCR0 = 0x0F; // WGM01=1, CTC ���, 1024����
	OCR0 = 255; 
	TCNT0 = 0x0; // Ÿ�̸�/ī����0 �������� �ʱⰪ
	
	TCCR2 = 0x05; // overflow mode, 1024����
	TCNT2 = 6; 
	
	SREG |= 0x80; // ���� ���ͷ�Ʈ �ο��̺� ��Ʈ I set
	
	while(1){
			if(TIMER0_COMPARE_INT_BIT==1){
				TIMER0_COMPARE_INT_BIT=0;
				if(BLINK0==0) {
					PORTC.0 = 0; // ��Ʈ ���
					BLINK0=1;
				}
				else {
					PORTC.0 = 1; // ��Ʈ ���
					BLINK0=0;
				}				
			}
			if(TIMER2_OVF_INT_BIT==1){
				TIMER2_OVF_INT_BIT=0;
				if(BLINK2==0) {
					PORTC.7 = 0; // ��Ʈ ���
					BLINK2=1;
				}
				else {
					PORTC.7 = 1; // ��Ʈ ���
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
