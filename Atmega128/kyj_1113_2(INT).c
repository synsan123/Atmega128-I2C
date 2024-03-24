/*
 * kyj_1113_2(INT).c
 *
 * Created: 2023-11-13 오후 12:08:53
 * Author: user
 */
 
#include <mega128.h> 
#include <delay.h>
 
unsigned char led = 0xFE;
unsigned char EINT4_FLAG_BIT = 0; // 주석을 만들어 놓은 문장을 고치기 위해 필요한 전역변수
unsigned char EINT5_FLAG_BIT = 0;
unsigned char EINT6_FLAG_BIT = 0;
unsigned char EINT7_FLAG_BIT = 0;

void main(void)
{ 

    DDRC = 0xFF; // 포트 C 출력 설정
    DDRE = 0b00000010; // 포트 E 입력(PE1 출력)
    
    PORTC = led; // 포트 C에 초기값 출력
    
    EIMSK = 0b11110000; // 외부 인터럽트4 enable
    EICRB = 0b10101010; // 외부 인터럽트4 level trigger
    SREG |= 0x80; // 전역 인터럽트 enable-bit set
    
    while(1)
    {   
        if(EINT4_FLAG_BIT == 1)
        {   
            EINT4_FLAG_BIT == 0;
            led = led << 1; // 1비트 쉬프트
            led = led | 0b00000001; // LSB bit set
    
            if(led == 0xFF) led = 0xFE;
            PORTC = led; // 포트 출력
    
            delay_ms(500);
        }
        
        if(EINT5_FLAG_BIT == 1)
        {   
            EINT5_FLAG_BIT == 0;
            PORTC = 0xAA;
        }
        
        if(EINT6_FLAG_BIT == 1)
        {                      
            EINT6_FLAG_BIT == 0;
            PORTC = 0x55;
        }
        
        if(EINT7_FLAG_BIT == 1)
        {
            EINT7_FLAG_BIT == 0;
            PORTC = 0xA5;
        }       
    
    }
    
}

/*interrupt [EXT_INT4] void external_int4(void) // 인터럽트가 이렇게 긴건 좋은게 아님 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    led = led << 1; // 1비트 쉬프트
    led = led | 0b00000001; // LSB bit set
    
    if(led == 0xFF) led = 0xFE;
    PORTC = led; // 포트 출력
    
    delay_ms(500);
    SREG |= 0x80; // All Interrupt enable
}*/

interrupt [EXT_INT4] void external_int4(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT4_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [EXT_INT5] void external_int5(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT5_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [EXT_INT6] void external_int6(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT6_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}

interrupt [EXT_INT7] void external_int7(void) 
{ 
    SREG &= 0x7F; // All Interrupt disable
    
    EINT7_FLAG_BIT = 1;
    
    SREG |= 0x80; // All Interrupt enable
}