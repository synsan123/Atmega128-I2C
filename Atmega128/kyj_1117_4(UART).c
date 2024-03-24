/*
 * kyj_1117_4(UART).c
 *
 * Created: 2023-11-17 오후 2:54:31
 * Author: user
 */

#include <mega128.h> 
#include <delay.h> 

unsigned char ch;
unsigned char EINT6_FLAG_BIT=0;  
unsigned char BLINK2=0;
unsigned char BLINK0=0;
unsigned char string1[] = "Menu Display\r"; // 전송 문자열
unsigned char string2[] = "1 : LED Blink\r"; // 전송 문자열
unsigned char string3[] = "2 : WRITE without you\r"; // 전송 문자열
unsigned char string4[] = "Press the number\r"; // 전송 문자열
unsigned char string5[] = "From Monday to Friday\r"; // 전송 문자열
unsigned char *pStr; // 문자열 포인터

void Putch(char);
void Str_func(void);

void main(void)
{ 
    UCSR0A = 0x00;
    UCSR0B = 0x98;
    UCSR0C = 0x06;
    UBRR0H = 0;
    UBRR0L = 103;
    
    DDRC = 0xFF; // 포트 C 출력 설정
    PORTC = 0xFF; // 포트 C에 초기값 출력
    
    EIMSK = 0b01000000; // 외부 인터럽트4 enable
    EICRB = 0b00100000; // 외부 인터럽트4 level trigger
    
    SREG |= 0x80; // 전역 인터럽트 enable-bit set
    
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


interrupt [EXT_INT6] void external_int6(void)  // UART 송수신 전부 세팅하는데 리시버 인터럽트만 setting, Menu Display(송신) CPU -> PC로 송신, 1: LED Blink 2: Write without you 띄우고 Press the number 1을 누르면(수신된 것) LED 깜빡거리게 하기. 그리고 똑같은 내용 나오게 하기. 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT6_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [USART0_RXC] void usart0_rx(void) // 수신 완료 인터럽트
{
    ch = UDR0; // 수신
    
    while((UCSR0A & 0x20) == 0x0); // UERE0=1 될 때까지 대기
    UDR0 = ch; // 송신
}

void Putch(char data) // 한 바이트 송신
{
    while((UCSR0A & 0x20) == 0x0); //UDRE0[5] = 1 송신준비완료 될 때까지 대기
    UDR0 = data; // 데이터 전송
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


    


