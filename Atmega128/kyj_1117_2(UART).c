/*
 * kyj_1117_2(UART).c
 *
 * Created: 2023-11-17 오후 12:25:29
 * Author: user
 */

#include <mega128a.h>

unsigned char seg_pat[16]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void main(void)
{
    unsigned char my_data, seg_data = 0;
    
    DDRB = 0xF0; //Segment Led
    DDRD = 0xF0; //Segment Led
    DDRG = 0x0F; //Segment power on
    
    PORTG = 0x0F; // 맨 끝 오른쪽
    
    UCSR0A = 0x00; // USART 초기화
    UCSR0B = 0x10; // 수신 enable RXEN0 = 1
    UCSR0C = 0x06; // 비동기, No Parity, stop, 8bit
    UBRR0H = 0x00;
    UBRR0L = 103; // Baud rate 9600
        
    while (1)
    {
        while((UCSR0A & 0x80) == 0x00) // 수신 버퍼에 새로운 데이터 수신 될 까지 대기. 1이 될 때 까지 대기 
        {
            PORTD = ((seg_pat[seg_data] & 0x0F) << 4) | (PORTD & 0x0F);
            PORTB = (seg_pat[seg_data] & 0x70) | (PORTB & 0x0F);
        }
        my_data = UDR0; // keyboard input value
        
        if(my_data >= '0' && my_data <= '9') seg_data = my_data - '0';
        else if(my_data >= 'a' && my_data <= 'f') seg_data = my_data - 'a' + 10;
        else if(my_data >= 'A' && my_data <= 'F') seg_data = my_data - 'A' + 10; 

    } 

}












