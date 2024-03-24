/*
 * kyj_1117_4(UART).c
 *
 * Created: 2023-11-17 ���� 2:54:31
 * Author: user
 */

#include <mega128.h> 
#include <delay.h> 

unsigned char ch;
unsigned char EINT6_FLAG_BIT=0;  
unsigned char BLINK2=0;
unsigned char BLINK0=0;
unsigned char string1[] = "Menu Display\r"; // ���� ���ڿ�
unsigned char string2[] = "1 : LED Blink\r"; // ���� ���ڿ�
unsigned char string3[] = "2 : WRITE without you\r"; // ���� ���ڿ�
unsigned char string4[] = "Press the number\r"; // ���� ���ڿ�
unsigned char string5[] = "From Monday to Friday\r"; // ���� ���ڿ�
unsigned char *pStr; // ���ڿ� ������

void Putch(char);
void Str_func(void);

void main(void)
{ 
    UCSR0A = 0x00;
    UCSR0B = 0x98;
    UCSR0C = 0x06;
    UBRR0H = 0;
    UBRR0L = 103;
    
    DDRC = 0xFF; // ��Ʈ C ��� ����
    PORTC = 0xFF; // ��Ʈ C�� �ʱⰪ ���
    
    EIMSK = 0b01000000; // �ܺ� ���ͷ�Ʈ4 enable
    EICRB = 0b00100000; // �ܺ� ���ͷ�Ʈ4 level trigger
    
    SREG |= 0x80; // ���� ���ͷ�Ʈ enable-bit set
    
    while(1)
    {
        Str_func();
        if(ch == '1')
        {
            PORTC = 0x55;
            delay_ms(10);
            PORTC = 0xFF;
            delay_ms(10);
            pStr = string4;
            while(*pStr != 0) Putch(*pStr++);
        }
        else if(ch == '2')
        {
            pStr = string5;
            while(*pStr != 0) Putch(*pStr++);    
            pStr = string4;
            while(*pStr != 0) Putch(*pStr++);
        }     
        if(EINT6_FLAG_BIT == 1) 
        {
            EINT6_FLAG_BIT= 0;
            
           
        }
                 
    }
}


interrupt [EXT_INT6] void external_int6(void)  // UART �ۼ��� ���� �����ϴµ� ���ù� ���ͷ�Ʈ�� setting, Menu Display(�۽�) CPU -> PC�� �۽�, 1: LED Blink 2: Write without you ���� Press the number 1�� ������(���ŵ� ��) LED �����Ÿ��� �ϱ�. �׸��� �Ȱ��� ���� ������ �ϱ�. 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT6_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [USART0_RXC] void usart0_rx(void) // ���� �Ϸ� ���ͷ�Ʈ
{
    ch = UDR0; // ����
    
    while((UCSR0A & 0x20) == 0x0); // UERE0=1 �� ������ ���
    UDR0 = ch; // �۽�
}

void Putch(char data) // �� ����Ʈ �۽�
{
    while((UCSR0A & 0x20) == 0x0); //UDRE0[5] = 1 �۽��غ�Ϸ� �� ������ ���
    UDR0 = data; // ������ ����
}

void Str_func(void)
{
    pStr = string1;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string2;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string3;
    while(*pStr != 0) Putch(*pStr++);
    pStr = string4;
    while(*pStr != 0) Putch(*pStr++);
}


    


