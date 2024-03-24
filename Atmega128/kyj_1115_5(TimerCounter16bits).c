/*
 * kyj_1115_5(TimerCounter16bits).c
 *
 * Created: 2023-11-15 오후 4:14:22
 * Author: user
 */

#include <mega128a.h>
#include <delay.h>

typedef unsigned char U8;
typedef unsigned short U16;

U8 INT_BIT4 = 0;
U8 INT_BIT5 = 0;
U16 TIM0_OVF_INT_BIT = 0;
U8 N1000 = 0, N100 = 0, N10 = 0, N1 = 0;
U8 pos = 0; // 자릿 수 변경
U8 hour = 12, min = 0, sec = 0;
U8 seg_pat[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

//unsigned int count = 0; 

void Time_out(void);

void main(void)
{
    DDRC = 0xFF;
    PORTC = 0xFF;
    DDRB = 0xF0; //7segment 출력
    DDRD = 0xF0; //7segment 출력
    DDRG = 0x0F; //7segment 전원 출력
    
    EIMSK = 0x30;  // INT 4,5 enable
    EICRB = 0x0A; // Falling Edge
    
    TIMSK = 0x01; // TIMER0 Overflow
    TCCR0 = 0x05; // overflow, 128 divide , 1ms * 1000 = 1sec
    TCNT0 = 0x83; // decade = 131
    
    SREG |= 0x80; // Global INT I set
    
    while (1)
    {   
        Time_out();   
        if(TIM0_OVF_INT_BIT >= 1000)
        {
            TIM0_OVF_INT_BIT = 0;  
                sec++;       
                if(sec == 60){
                    sec = 0;  
                    PORTC = 0xFE; // 1min = 60sec 확인용
                    min++;           
                    if(min == 60){
                        min = 0;
                        hour = (hour + 1) % 24;
                    }// end of min
            
                }// end of sec+9
        }
        if(INT_BIT4 == 1)
        {   
            INT_BIT4 = 0;
            
            if(pos == 0) N1 = (N1 + 1) % 10; // 현재 분 1단위 표시 +1 값
            else if(pos == 1) N10 = (N10 + 1) % 6; // 현재 분 10단위 표시 +1 값 (60분)
            else if(pos == 2) 
            { // 시간 1단위 +1
                if(N1000 == 2) N100 = (N100 + 1) % 4; // 24시간의 앞자리 2이므로 20,21,22,23,24
                else N100 = (N100 + 1) % 10;
            }// end of pos == 2
            else 
            {   // pos=3 (세 번 눌러짐)
                if(N100 < 4) N1000 = (N1000 + 1) % 3; // N100<4은 04 (00,01,02,03), (10,11,12,13), 
                // else if(N1000 != 1) N1000 = (N1000 + 1) % 3; 
                else N1000 = (N1000 + 1) % 2;
            }// end of else
            
            hour = N1000 * 10 + N100;
            min = N10 * 10 + N1;    
        }
        if(INT_BIT5 == 1)
        {
            INT_BIT5 = 0;
            pos = (pos + 1) % 4;
        }

        
    }
    
}

void Time_out(void)
{
    N1 = min % 10; // ( 0 ~ 9분 )
    N10 = min / 10; // (0 ~ 50분, 10의 자리)
    N100 = hour % 10; // (0 ~ 9시간)
    N1000 = hour / 10; // (0 ~ 20시간, 10의 자리)

    PORTG = 0x08; // 7-Seg DIG4 ON(PG3=1), 분 1자리 표시
    PORTD = ((seg_pat[min % 10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[min % 10] & 0x70 ) | (PORTB & 0x0F);
    delay_us(300);

    PORTG = 0x04; // 7-Seg DIG3 ON(PG2=1), 분 10자리 표시
    PORTD = ((seg_pat[min / 10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[min / 10] & 0x70 ) | (PORTB & 0x0F); 
    delay_us(300); 

    PORTG = 0x02; // 7-Seg DIG2 ON(PG1=1), 시간 1자리 표시
    PORTD = ((seg_pat[hour % 10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[hour % 10] & 0x70 ) | (PORTB & 0x0F); 
    delay_us(300);

    PORTG = 0x01; // 7-Seg DIG1 ON(PG0=1), 시간 10자리 표시
    PORTD = ((seg_pat[hour / 10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[hour / 10] & 0x70 ) | (PORTB & 0x0F); 
    delay_us(300); 
}

interrupt [EXT_INT4] void INT5_int4 (void)
{
    SREG &= 0x7F;
    INT_BIT4 = 1;
    SREG |= 0x80; 
}
interrupt [EXT_INT5] void INT5_int5 (void)
{
    SREG &= 0x7F;
    INT_BIT5 = 1;
    SREG |= 0x80; 
}
interrupt [TIM0_OVF] void timer0_OVF (void)
{
    TCNT0 = 0x83;
    SREG &= 0x7F;
    TIM0_OVF_INT_BIT++;
    SREG |= 0x80;    
}
