/*
 * kyj_1103_4(Another_switch_Example).c
 *
 * Created: 2023-11-03 ���� 3:53:18
 * Author: user
 */

#include <mega128a.h>

void main(void)
{
    unsigned char key;
    
    DDRC = 0xFF; // ��Ʈ C ��� ����
    DDRE = 0x00; // ��Ʈ E �Է� ����
    PORTC = 0xFF; // LED ��� OFF   
    
    while(1)
    {
        key = PINE & 0xF0; // ��Ʈ E 4.5.6.7 �о����.
        
        if(key == 0b11100000) PORTC = 0x00; // SW1�� ������ LED ��� ON
        else if(key == 0b11010000) PORTC = 0xFF; // SW2�� ������ LED ��� OFF
        else if(key == 0b10110000) PORTC = 0b01010101; // SW3�� ������ ¦����° LED ON
        else if(key == 0b01110000) PORTC = 0b10101010; // SW4�� ������ Ȧ����° LED ON
    }

}
