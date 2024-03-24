#include <mega128.h>

unsigned char blink1 = 0; 
unsigned char blink3 = 0;
unsigned int time = 0xB6C2; // 346786

void main(void)
{ 
	DDRC = 0xFF; // 포트 C 출력으로 설정
	//PORTC = led; // 포트 C에 초기값 출력
	
	TCCR1A = 0x00; // 모드 0(일반모드)
	TCCR1B = 0x04; // 프리스케일 = CK/256
	TCCR1C = 0x0;
	TCNT1L = time;	// C2 
	TCNT1H = time>>8; 	
	TIMSK = 0x04; // TOIE1 = 1, 오버플로우 인터럽트 인에이블

	TCCR3A = 0x00; // 모드 0(일반모드)
	TCCR3B = 0b00001101; // 프리스케일 = CK/1024 -101
	TCCR3C = 0x0;
	TCNT3L = 0;	 
	TCNT3H = 0; 
	OCR3AL = 0x1A;	// 46874 0xB71A 
	OCR3AH = 0xB7; 	
	ETIMSK = 0b00010000;
	
	SREG |= 0x80; // 전역 인터럽트 인에이블 비트 I set
	while(1); 
}

interrupt [TIM1_OVF] void timer_int1(void) 
{ 
	TCNT1L = 0xC2;	 
	TCNT1H = 0xB6;
	SREG &= 0x7F;	
	if(blink1==0) {
		PORTC = 0xA0; 
		blink1=1;
	}
	else {
		PORTC = 0x0A; 
		blink1=0;
	}		
		SREG |= 0x80;
}

interrupt [TIM3_COMPA] void timer_int3(void) 
{ 
	TCNT3L = 0;	 
	TCNT3H = 0;
	SREG &= 0x7F;	
	if(blink3==0) {
		PORTC = 0xF0; 
		blink3=1;
	}
	else {
		PORTC = 0x0F; 
		blink3=0;
    }        
        SREG |= 0x80;
}
