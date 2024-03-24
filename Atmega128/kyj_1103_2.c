/*
 * kyj_1103_2.c
 *
 * Created: 2023-11-03 오후 2:17:27
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

void main(void)
{
   
    
    DDRC = 0xFF; // 포트C 출력 설정
    PORTC = 0x55;     //0101 0101 => 0101 0100으로 만들어라.
    while(1)
    { 
       //PORTC &= 0xFE;// 0101 0101 & 1111 1110 = 0101 0100 
       //AND는 계속 0번 led가 켜져있음.
       PORTC ^= 0x01; // 0101 0101 ^ 0000 0001 = 0101 0100 됨
       //XOR은 0번 LED가 계속 점멸됨. 0101 0101 => 0101 0100 => 0101 0101... 무한 반복 함.
       delay_ms(500); 
    }
}
