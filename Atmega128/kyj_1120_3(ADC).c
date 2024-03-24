/*
 * kyj_1120_3(ADC).c
 * LM35DZ �µ� ����.
 * Created: 2023-11-20 ���� 11:18:30
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

typedef unsigned char u_char;

const u_char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
int TIM0_INT_BIT = 0;
int ad_val = 0;

void AD_disp(int); // A/D ��ȯ�� ǥ��

void main(void)
{ 
    DDRB = 0xF0; // ��Ʈ B ���� 4��Ʈ ��� ����
    DDRD = 0xF0; // ��Ʈ D ���� 4��Ʈ ��� ����
    DDRG = 0x0F; // ��Ʈ G ���� 4��Ʈ ��� ����
    
    TIMSK = 0x01; // timer0 overflow
    TCCR0 = 0x05; // 128 divider
    TCNT0 = 0x83; // 131 , 1sec
    
    ADMUX = 0x07; // ADC7 �ܱؼ� �Է� ����
    ADCSRA = 0x87; // ADEN=1, ADSC=1, ADFR=1, 16MHz, 128���� -> 125kHz
    
    SREG |= 0x80;
    
    while(1)
    {   
        AD_disp(ad_val); // A/D�� ǥ��
        
        if(TIM0_INT_BIT >= 1000)
        {
            TIM0_INT_BIT = 0;
            ADCSRA = 0xC7;
            
            while((ADCSRA & 0x10) == 0);
        
            ad_val = (int)ADCL + ((int)ADCH << 8); // A/D�� �б�
        
        }  
                 
    }
} 


void AD_disp(int val) 
{ 
    float fval;
    int ival, buf; 
    u_char N100, N10, N1;

    fval = (float)val * 5.0 / 1023.0; // ���а����� ��ȯ
    ival = (int)(fval * 1000.0 + 0.5); // �ݿø� �� ����ȭ

    N100 = ival / 100; // ������ ����
    buf = ival % 100;
     
    N10 = buf / 10; // �Ҽ� ù° �ڸ� ����
    N1 = buf % 10; // �Ҽ� ��° �ڸ� ����

    PORTG = 0b00001000; // PG3=1, �Ҽ� ��° �ڸ�
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);
    delay_ms(10);
    
    PORTG = 0b00000100; // PG2=1, �Ҽ� ù° �ڸ�
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F);
    PORTB = PORTB | 0x80; // DP on(�Ҽ���) 
    delay_ms(10); 
    
    PORTG = 0b00000010; // PG1=1, ������
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