/*
 * kyj_1117_3(UART).c
 *
 * Created: 2023-11-17 ���� 2:06:17
 * Author: user
 */

/* ��ǻ���� Ű���忡�� �Է��� ���� ATmega128�� �����Ѵ�.
   ATmega128�� ������ �Է� ���� �Ǵ��Ͽ� �빮���̸� �ҹ���, �ҹ����̸� �빮�ڷ� ��ȯ�Ͽ�
   ��ǻ�ͷ� �����Ѵ�. */
#/*include <mega128a.h>

unsigned char ch;
unsigned char USART_BIT = 0;

void main(void)
{
    UCSR0A = 0x00;
    UCSR0B = 0x98; // ���� ���ͷ�Ʈenable ,����, �۽�(����)
    UCSR0C = 0x06;
    UBRR0H = 0x00;
    UBRR0L = 8;
    
    SREG |= 0x80;
    
    while (1)
    {
        while((UCSR0A & 0x80) == 0x00); // ATmega128 ���� ���ۿ� ���ο� ������ ���� ���� ������ ���� �ݺ�. �����Ͱ� ������ 1.
        
        ch = UDR0;
        
        if((ch >= 'a') && (ch <= 'z'))
        {
            ch = ch - 'a' + 'A';
        }
        else if((ch >= 'A') && (ch <= 'Z'))
        {
            ch = ch - 'A' + 'a';
        }
        
        while((UCSR0A & 0x20) == 0x00); // �۽�(����)������ �����Ͱ� ���� ������ ���� �ݺ�. �����Ͱ� ������ 1.
        UDR0 = ch;   
                

    }
}*/

#include <mega128a.h>

unsigned char ch;

void main(void)
{
    UCSR0A = 0x00;
    UCSR0B = 0x98; // ���� ���ͷ�Ʈenable ,����, �۽�(����)
    UCSR0C = 0x06;
    UBRR0H = 0x00;
    UBRR0L = 8;
    
    SREG |= 0x80;
    
    while (1)
    {

    }
}

interrupt [USART0_RXC] void usart0_rx (void)
{
    SREG &= 0x7F;
    
    ch = UDR0;
        
    if((ch >= 'a') && (ch <= 'z'))
    {
        ch = ch - 'a' + 'A';
    }
    else if((ch >= 'A') && (ch <= 'Z'))
    {
        ch = ch - 'A' + 'a';
    }
        
    while((UCSR0A & 0x20) == 0x00); // �۽�(����)������ �����Ͱ� ���� ������ ���� �ݺ�. �����Ͱ� ������ 1.
    UDR0 = ch;   

    SREG |= 0x80;
}
