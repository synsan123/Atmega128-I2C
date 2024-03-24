/*
 * kyj_1113_2(INT).c
 *
 * Created: 2023-11-13 ���� 12:08:53
 * Author: user
 */
 
#include <mega128.h> 
#include <delay.h>
 
unsigned char led = 0xFE;
unsigned char EINT4_FLAG_BIT = 0; // �ּ��� ����� ���� ������ ��ġ�� ���� �ʿ��� ��������
unsigned char EINT5_FLAG_BIT = 0;
unsigned char EINT6_FLAG_BIT = 0;
unsigned char EINT7_FLAG_BIT = 0;

void main(void)
{ 

    DDRC = 0xFF; // ��Ʈ C ��� ����
    DDRE = 0b00000010; // ��Ʈ E �Է�(PE1 ���)
    
    PORTC = led; // ��Ʈ C�� �ʱⰪ ���
    
    EIMSK = 0b11110000; // �ܺ� ���ͷ�Ʈ4 enable
    EICRB = 0b10101010; // �ܺ� ���ͷ�Ʈ4 level trigger
    SREG |= 0x80; // ���� ���ͷ�Ʈ enable-bit set
    
    while(1)
    {   
        if(EINT4_FLAG_BIT == 1)
        {   
            EINT4_FLAG_BIT == 0;
            led = led << 1; // 1��Ʈ ����Ʈ
            led = led | 0b00000001; // LSB bit set
    
            if(led == 0xFF) led = 0xFE;
            PORTC = led; // ��Ʈ ���
    
            delay_ms(500);
        }
        
        if(EINT5_FLAG_BIT == 1)
        {   
            EINT5_FLAG_BIT == 0;
            PORTC = 0xAA;
        }
        
        if(EINT6_FLAG_BIT == 1)
        {                      
            EINT6_FLAG_BIT == 0;
            PORTC = 0x55;
        }
        
        if(EINT7_FLAG_BIT == 1)
        {
            EINT7_FLAG_BIT == 0;
            PORTC = 0xA5;
        }       
    
    }
    
}

/*interrupt [EXT_INT4] void external_int4(void) // ���ͷ�Ʈ�� �̷��� ��� ������ �ƴ� 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    led = led << 1; // 1��Ʈ ����Ʈ
    led = led | 0b00000001; // LSB bit set
    
    if(led == 0xFF) led = 0xFE;
    PORTC = led; // ��Ʈ ���
    
    delay_ms(500);
    SREG |= 0x80; // All Interrupt enable
}*/

interrupt [EXT_INT4] void external_int4(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT4_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [EXT_INT5] void external_int5(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT5_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [EXT_INT6] void external_int6(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT6_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [EXT_INT7] void external_int7(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT7_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}