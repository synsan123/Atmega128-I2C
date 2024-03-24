/*
 * kyj_1117_3(UART).c
 *
 * Created: 2023-11-17 오후 2:06:17
 * Author: user
 */

/* 컴퓨터의 키보드에서 입력한 값을 ATmega128로 전송한다.
   ATmega128은 수신한 입력 값을 판단하여 대문자이면 소문자, 소문자이면 대문자로 변환하여
   컴퓨터로 전송한다. */
#/*include <mega128a.h>

unsigned char ch;
unsigned char USART_BIT = 0;

void main(void)
{
    UCSR0A = 0x00;
    UCSR0B = 0x98; // 수신 인터럽트enable ,수신, 송신(전송)
    UCSR0C = 0x06;
    UBRR0H = 0x00;
    UBRR0L = 8;
    
    SREG |= 0x80;
    
    while (1)
    {
        while((UCSR0A & 0x80) == 0x00); // ATmega128 수신 버퍼에 새로운 데이터 값이 들어올 때까지 무한 반복. 데이터가 들어오면 1.
        
        ch = UDR0;
        
        if((ch >= 'a') && (ch <= 'z'))
        {
            ch = ch - 'a' + 'A';
        }
        else if((ch >= 'A') && (ch <= 'Z'))
        {
            ch = ch - 'A' + 'a';
        }
        
        while((UCSR0A & 0x20) == 0x00); // 송신(전송)버퍼의 데이터가 빠질 때까지 무한 반복. 데이터가 빠지면 1.
        UDR0 = ch;   
                

    }
}*/

#include <mega128a.h>

unsigned char ch;

void main(void)
{
    UCSR0A = 0x00;
    UCSR0B = 0x98; // 수신 인터럽트enable ,수신, 송신(전송)
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
        
    while((UCSR0A & 0x20) == 0x00); // 송신(전송)버퍼의 데이터가 빠질 때까지 무한 반복. 데이터가 빠지면 1.
    UDR0 = ch;   

    SREG |= 0x80;
}
