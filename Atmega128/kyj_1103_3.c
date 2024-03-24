/*
 * kyj_1103_3.c
 *
 * Created: 2023-11-03 오후 3:06:30
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

#define LED0 PORTC.0
#define LED1 PORTC.1
#define ON 0
#define OFF 1

void main(void)
{
    unsigned char sw1, sw2, sw3, sw4;
    
    DDRC = 0xFF; //PORTC를 출력으로 설정. 달리 말하면 PORTC를 불러온다.
    DDRE = 0x00; //PORTE를 입력으로 설정. 달리 말하면 PINE를 불러온다.
    
    PORTC = 0xFF; //LED를 모두 OFF하였다.
     
    while (1)
        {           
            sw1 = PINE & 0b00010000; //PE비트 부분 추출.PORTE의 4번 입력핀 
            sw2 = PINE & 0b00100000;
            sw3 = PINE & 0b01000000;
            sw4 = PINE & 0b10000000;
            if(sw1 == 0) PORTC = 0x00; //입력이 1이면 스위치 OFF. 즉 LED OFF
            else if(sw2 == 0) PORTC = 0xFF; //입력이 0이면 스위치 ON. 즉 LED ON
            else if(sw3 == 0) PORTC = 0xAA;
            else if(sw4 == 0) PORTC = 0xAA;   
        }              
        
}
