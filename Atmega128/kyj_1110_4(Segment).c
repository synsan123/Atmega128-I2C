#include <mega128.h>                              //라이브러리 헤더 파일 포함
#include <delay.h>                             

typedef unsigned char U8;                         //typedef 전처리문을 사용  
                                                 //변수의 다양한 사용을 위해 main문밖 Global variable로 선언
typedef unsigned int U32;
                           
const U8 Segment[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};


void My_Birthday(U32 number);
void main(void)
{
    DDRB = 0xF0;
    DDRD = 0xF0;
    DDRG = 0x0F;
    
    PORTB = 0x00;
    PORTD = 0x00;
    
    while(1)
    {
        My_Birthday(1996);
        delay_ms(500);
        My_Birthday(316);
        delay_ms(500);
    }

}

void My_Birthday(U32 number)
{
    int i, N4, N3, N2, N1, buffer;
    
    N4 = number / 1000;
    buffer = number % 1000;
    
    N3 = buffer / 100;
    buffer = buffer % 100;
    
    N2 = buffer / 10;
    N1 = buffer % 10;
    
   
        PORTG = 0x01;
        PORTD = ((Segment[N4] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N4] & 0x70) | (PORTB & 0x0F);
        delay_ms(1000);
        
        PORTG = 0x02;
        PORTD = ((Segment[N3] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N3] & 0x70) | (PORTB & 0x0F);
        delay_ms(1000);
        
        PORTG = 0x04;
        PORTD = ((Segment[N2] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N2] & 0x70) | (PORTB & 0x0F);
        delay_ms(1000);
        
        PORTG = 0x08;
        PORTD = ((Segment[N1] & 0x0F) << 4) | (PORTD & 0x0F);
        PORTB = (Segment[N1] & 0x70) | (PORTB & 0x0F);
        delay_ms(1000);        
    
    
}