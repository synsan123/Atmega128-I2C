/*
 * kyj_1110_6(Segment).c
 *
 * Created: 2023-11-13 ���� 8:46:45
 * Author: user
 */

#include <mega128.h>
#include <delay.h>

const unsigned char seg_pat[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void main(void) 
{ 
    unsigned char old_key, new_key, i = 0;
    
    DDRE = 0x00; // ��Ʈ E ���� 4��Ʈ �Է� ���� 
    DDRB = 0xF0; // ��Ʈ B ���� 4��Ʈ ��� ����
    DDRD = 0xF0; // ��Ʈ D ���� 4��Ʈ ��� ����
    DDRG = 0x0F; // ��Ʈ G ���� 4��Ʈ ��� ���� 
                       
    PORTG = 0x0F; // �� ���� 7-Segment DIG4 ON(PG3=1)
    PORTB = 0x00; // E, F, G, DP off (�ʱⰪ)
    PORTD = 0x00; // A, B, C, D off (�ʱⰪ)
    
    old_key = PINE & 0x10;
    
        
    while(1)
    { 
        new_key = PINE & 0x10;
    
        if((old_key == 0x10) && (new_key == 0x00))
        {   
            PORTD = ((seg_pat[i] & 0x0F) << 4) | (PORTD & 0x0F); // seg_pat ���� 4bit A, B, C, D
            PORTB = (seg_pat[i] & 0x70 ) | (PORTB & 0x0F); // seg_pat ���� 3bit E, F, G
            i = (i + 1) % 0x10;
        }
        old_key = new_key;
        
    }
}