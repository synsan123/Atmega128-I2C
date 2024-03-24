/*
 * kyj_1103_4(Another_switch_Example).c
 *
 * Created: 2023-11-03 오후 3:53:18
 * Author: user
 */

#include <mega128a.h>

void main(void)
{
    unsigned char key;
    
    DDRC = 0xFF; // 포트 C 출력 설정
    DDRE = 0x00; // 포트 E 입력 설정
    PORTC = 0xFF; // LED 모두 OFF   
    
    while(1)
    {
        key = PINE & 0xF0; // 포트 E 4.5.6.7 읽어오기.
        
        if(key == 0b11100000) PORTC = 0x00; // SW1이 눌리면 LED 모두 ON
        else if(key == 0b11010000) PORTC = 0xFF; // SW2가 눌리면 LED 모두 OFF
        else if(key == 0b10110000) PORTC = 0b01010101; // SW3이 눌리면 짝수번째 LED ON
        else if(key == 0b01110000) PORTC = 0b10101010; // SW4가 눌리면 홀수번째 LED ON
    }

}
