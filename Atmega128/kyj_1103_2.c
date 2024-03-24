/*
 * kyj_1103_2.c
 *
 * Created: 2023-11-03 ���� 2:17:27
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

void main(void)
{
   
    
    DDRC = 0xFF; // ��ƮC ��� ����
    PORTC = 0x55;     //0101 0101 => 0101 0100���� ������.
    while(1)
    { 
       //PORTC &= 0xFE;// 0101 0101 & 1111 1110 = 0101 0100 
       //AND�� ��� 0�� led�� ��������.
       PORTC ^= 0x01; // 0101 0101 ^ 0000 0001 = 0101 0100 ��
       //XOR�� 0�� LED�� ��� �����. 0101 0101 => 0101 0100 => 0101 0101... ���� �ݺ� ��.
       delay_ms(500); 
    }
}