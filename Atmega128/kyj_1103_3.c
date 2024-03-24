/*
 * kyj_1103_3.c
 *
 * Created: 2023-11-03 ���� 3:06:30
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
    
    DDRC = 0xFF; //PORTC�� ������� ����. �޸� ���ϸ� PORTC�� �ҷ��´�.
    DDRE = 0x00; //PORTE�� �Է����� ����. �޸� ���ϸ� PINE�� �ҷ��´�.
    
    PORTC = 0xFF; //LED�� ��� OFF�Ͽ���.
     
    while (1)
        {           
            sw1 = PINE & 0b00010000; //PE��Ʈ �κ� ����.PORTE�� 4�� �Է��� 
            sw2 = PINE & 0b00100000;
            sw3 = PINE & 0b01000000;
            sw4 = PINE & 0b10000000;
            if(sw1 == 0) PORTC = 0x00; //�Է��� 1�̸� ����ġ OFF. �� LED OFF
            else if(sw2 == 0) PORTC = 0xFF; //�Է��� 0�̸� ����ġ ON. �� LED ON
            else if(sw3 == 0) PORTC = 0xAA;
            else if(sw4 == 0) PORTC = 0xAA;   
        }              
        
}
