/*
 * kyj_1117_1(UART).c
 *
 * Created: 2023-11-17 오전 10:58:34
 * Author: user
 */

#include <mega128.h>
#include <delay.h>

void Putch(char); // 송신
void main(void)
{ 
    char string[] = "No I can't forget this evening,\r";
    char string2[] = "Or your face as you were leaving.\r";
    char string3[] = "But I guess that's just the way The story goes.\r";
    char string4[] = "You always smile but in your eyes.\r";
    char string5[] = "Your sorrow shows Yes it shows.\r";
    char string6[] = "When I think of all my sorrow\r";
    char string7[] = "When I had you there\r";
    char string8[] = "But then I let you go And now it's only fair.\r";
    char string9[] = "That I should let you know What you should know\r";
    //char string[] = "No I can't forget this evening\r, Or your face as you were leaving.\r But I guess that's just the way The story goes.\r You always smile but in your eyes.\r Your sorrow shows Yes it shows.\r No I can't forget tomorrow I think of all my sorrow When I had you there But then I let you go And now it's only fair. That I should let you kow What you should know";
    //띄우고자 하는 곳에 \r를 삽입하여 해도 됨. 그러면 string 변수 한 번만 사용해도 가능 
    char *pStr; // 문자열 포인터
    
    delay_ms(7000); // PC에서 메시지 확인 위해
    UCSR0A = 0x0; // USART 초기화 (251p)
    UCSR0B = 0b00001000; // 송신 인에이블 TXEN = 1 (253p)
    UCSR0C = 0b00000110; // 비동기[7], 데이터 8비트 모드 (253,4p)
    UBRR0H = 0; // X-TAL = 16MHz 일때, BAUD = 9600
    UBRR0L = 103; // (257p)
     
    pStr = string; 
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송   
    //Putch(0x0D); // \r를 삽입함. 문장마다 쓰기 귀찮으면 이거 넣으면 됨. 대신 아래와 같이 string변수와 pStr변수 그리고 Putch, while문이 많이 필요. 그 때 마다 Putch(0x0D)삽입해야함.
    pStr = string2;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string3;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string4;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string5;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string6;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string7;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string8;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
    pStr = string9;
    while(*pStr != 0) Putch(*pStr++); // 문자열 전송
           
    while(1); 
}
void Putch(char data) // 한 바이트 송신
{
    while((UCSR0A & 0x20) == 0x0); //UDRE0[5] = 1 송신준비완료 될 때까지 대기 
    UDR0 = data; // 데이터 전송  
}





























