/*
 * kyj_1120_Project2(test).c
 *
 * Created: 2023-11-20 ���� 4:04:03
 * Author: user
 */

#include <mega128.h>
unsigned char led = 0xFE;
unsigned char INT4_BIT = 0;
int TIM2_BIT = 0;
void main(void)
{ 
DDRC = 0xFF; // ��Ʈ C ������� ����
PORTC = led; // ��Ʈ C�� �ʱⰪ ��
EIMSK = 0x10;
EICRB = 0x03;
SREG = 0x80; // ���� ���ͷ�Ʈ �ο��̺� ��Ʈ I set
while(1)
{
    if(INT4_BIT == 1)
    {
        INT4_BIT = 0;
        TIMSK = 0x40; // OCIE0 = 1(��� �� ���ͷ�Ʈ �ο��̺�)
        TCCR2 = 0x05; // WGM01=1, CTC ���, 1024����
        TCNT2 = 0x00; // Ÿ�̸�/ī����0 �������� �ʱⰪ
    }
    if(TIM2_BIT >= 100)
    {
        TIM2_BIT = 0;
        led = led << 1; // 1��Ʈ ����Ʈ
        led = led | 0x01; // ������ ��Ʈ set
        if(led == 0xFF) led = 0xFE; // ��� off�̸� �ʱⰪ �缳��
        PORTC = led; // ��Ʈ ���
    }   
    
}
}
// Ÿ�̸�/ī����0 ��º�(TCNT0 = OCRO �϶�) ���ͷ�Ʈ ���� ��ƾ
// ���ͷ�Ʈ �߻� �ֱ� 1/16us * 1024���� * (1 + 155) = 9.98ms
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
