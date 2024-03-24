
#include <mega128.h>                              //���̺귯�� ��� ���� ����
#include <delay.h>                             

typedef unsigned char U8;                         //typedef ��ó������ ���  
typedef unsigned short U16;                       //������ �پ��� ����� ���� main���� Global variable�� ����
typedef unsigned int U32;

const U8 seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};       //const�� �̿��� �迭 ���
                                                  
U8 EINT4_FLAG_BIT=0;                              //���� ����
U8 EINT5_FLAG_BIT=0;
U8 EINT6_FLAG_BIT=0;
U8 EINT7_FLAG_BIT=0;

U8 TIMER1_CTC_FLAG_BIT=0;
U8 TIMER2_OVF_FLAG_BIT=0;
U16 TIMER2_OVF_COUNT=0;

U8 ADC_RESULT_FLAG=0;
U32 TEMP_ADC_VALUE=0;

U8 NA100, NA10, NA1;
U8 NV100, NV10, NV1;
U32 ad_value=0;
                                                  //�Լ� ����
void Seg4_out(U8 N1, U8 N10, U8 N100, U8 N1000);  //Segment ���ڸ� ���� ��� �Լ� ����(�й� ���ڸ�)
void Seg4_out2(U8 N1, U8 N10, U8 N100, U8 N1000); //Segment ���ڸ� ���� ��� �Լ� ����(�й� ���ڸ�)
void EXIT6_PWM_M6(void);
void EXIT7_PWM_M2(void);
void EXIT4_TIMER2(void);
void EXIT5_TIMER1(void);

void UART_volt(void);
void AD_display(int value);
void SEG7_TEMP_DIS(int val);
             

void main(void){                             
    //DDRC = 0xFF;    PORTC = 0xFF;              //�۵� Ȯ�ο����� LED ��� ����
    DDRB = 0xF0;                                 //�й� ���÷��̸� ���� ��ƮB 4,5,6,7 ��� ����(LED)
    DDRD = 0xF0;                                 //�й� ���÷��̸� ���� ��ƮD 4,5,6,7 ��� ����(LED)
    DDRG = 0x0F;                                 //�й� ���÷��̸� ���� ��ƮG 0,1,2,3 ��� ����(COM)      
 
    Seg4_out(0,2,0,2);                           //�й� ���ڸ� ǥ��
    Seg4_out2(1,5,2,2);                          //�й� ���ڸ� ǥ��
     
    EIMSK = 0b11110000;                          //���ͷ�Ʈ ��� ���� ���� �������ͷ� INT4,5,6,7 enable
    EICRB = 0b10111011;                          //INT4,5,6,7�� B�� ���� Interrupt trigger ��� ����(7fa,6ri,5fa,4ri)              
    SREG |= 0x80;                                //status resister bit7(MSB)�� 1���� �����Ͽ� ��� ���ͷ�Ʈ�� ���(global interrupt enable),OR�� ����Ͽ� ��������Ʈ�� �״��
                                                     
        
    while(1){    
        if(ADC_RESULT_FLAG==0) Seg4_out2(1,5,2,2);     //�µ� ������� ���� ���� �й��� ��� ���
        else SEG7_TEMP_DIS(TEMP_ADC_VALUE);            //�µ� ���� ���ö��� �µ��� ���÷�����
           
        if(EINT4_FLAG_BIT==1) {                        //���ͷ�Ʈ�� �߻��ϸ� 
            EINT4_FLAG_BIT=0;                          //0���� �ٽ� �ٲ��ְ�
            EXIT4_TIMER2();                            //EXIT4_TIMER2�Լ��� �����Ѵ�.
        } //end of if
        if(EINT5_FLAG_BIT==1) { 
            EINT5_FLAG_BIT=0;
            EXIT5_TIMER1();
        } //end of if
        if(EINT6_FLAG_BIT==1) { 
            EINT6_FLAG_BIT=0;
            EXIT6_PWM_M6();
        } //end of if
        if(EINT7_FLAG_BIT==1) { 
            EINT7_FLAG_BIT=0;
            EXIT7_PWM_M2();
        } //end of if  
        if(TIMER2_OVF_FLAG_BIT==1){                     //�����÷ο� ��尡 �߻��ϸ�
            TIMER2_OVF_FLAG_BIT=0;                      //0���� �ٲ��ְ�
            TIMER2_OVF_COUNT++;                         //1�� ���ذ���  
            if(TIMER2_OVF_COUNT==868){                  //4.427sec(�ֱⰡ 5.1msec�̹Ƿ� 868��)   
                TIMER2_OVF_COUNT=0;        
                ADC_RESULT_FLAG++;                      
                ADMUX = 7;                              // A/D ��ȯ�� ��Ƽ�÷��� ���� �������� 
            	ADCSRA = 0xE2;                          // ADEN=1, ADSC=1, ADFR=1, 16MHz, 8����  
                delay_us(100);                          // ��ȯ�ð��� ����� �߻��ϴ� ADC������ ������ ���� �����̰� �ʿ��� 
                TEMP_ADC_VALUE = (int)ADCL + ((int)ADCH << 8);  //A/D ��ȯ�� �б�
                if(ADC_RESULT_FLAG==10){ 
                  ADC_RESULT_FLAG%=10;                  // 10ȸ���ȸ� �µ� ǥ��    
                 TIMSK &= 0b10111111;                   // TOIE2 = 0���� �����Ͽ� ���ͷ�Ʈ�� �Ȱɸ��� �ϴ� ���
                } //end of if  
            } //end of if
       } //end of if
       if(TIMER1_CTC_FLAG_BIT==1){
            TIMER1_CTC_FLAG_BIT=0;  
            ADMUX = 6;                                  //ADC6 �ܱؼ� �Է� ����
            ADCSRA = 0x82;                              
            ADCSRA = 0xC2;                              // ADEN=1, 8���� ��ȯ ����
          
            while((ADCSRA & 0x10) == 0);                // ADIF=1�� �� ������(Single)
            ad_value = (int)ADCL + ((int)ADCH << 8);    // A/D ��ȯ�� �б�
            AD_display(ad_value);                       // A/D ��ȯ�� ǥ��
            UART_volt();                                // ����Ʈ ���÷��̸� ���� �Լ�
       } //end of if
        
    } //end of while
} //end of main


    
void Seg4_out(U8 N1, U8 N10, U8 N100, U8 N1000)    //�й� ���ڸ� 2020 ���÷���
{
    U8 i;
    for(i = 0;i <40 ;i++){                                          //40���� �ɋ����� ���÷���
        PORTG = 0b00001000;                                         //�� ���� 7Segment DIG4(PG3=1)ON, 1�ڸ� ǥ��
        PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);       //[0]ǥ�ø� ���� ABCDǥ��, PORTD ���� 4bits ������� �ʰ� | 
        PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);             //[0]ǥ�ø� ���� EFGǥ��, PORTB ���� 4bits ������� �ʰ� | 
        delay_ms(2);                                                //������ ���� ������
        PORTG = 0b00000100;                                         //�������� �ι�° 7Segment DIG3(PG2=1)ON, 10�ڸ� ǥ��
        PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F); 
        PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F); 
        delay_ms(2); 
        PORTG = 0b00000010;                                         //�������� ����° 7Segment DIG2(PG1=1)ON, 100�ڸ� ǥ��
        PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);     
        PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F); 
        delay_ms(2);
        PORTG = 0b00000001;                                         //�������� �׹�° 7Segment DIG1(PG0=1)ON, 1000�ڸ� ǥ��
        PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);    
        PORTB = (seg_pat[N1000] & 0x70 ) | (PORTB & 0x0F); 
        delay_ms(2);    
    } //end of for    
}//end of Seg4_out

void Seg4_out2(U8 N1, U8 N10, U8 N100, U8 N1000)   //�й� ���ڸ� 2251 ���÷���
{ 
        PORTG = 0b00001000; 
        PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F); 
        PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);
        delay_ms(1);                                                //������ ���� �ּ����� �̼��� ������ 
        PORTG = 0b00000100; 
        PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F); 
        PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F); 
        delay_ms(1); 
        PORTG = 0b00000010; 
        PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F); 
        PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F); 
        delay_ms(1);
        PORTG = 0b00000001; 
        PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F); 
        PORTB = (seg_pat[N1000] & 0x70 ) | (PORTB & 0x0F); 
        delay_ms(1);    
} //end of Seg4_out2



interrupt [EXT_INT4] void external_int4(void){   //main CPU�� ���α׷��� �����ϰ� �ִٰ� interrupt�� �߻��ϸ� �� ���񽺷�ƾ�� ����,������Ͽ��� ���ǵ� ���ͷ�Ʈ �ҽ��� ���,�Լ����� ���Ƿ� ����
    SREG &= 0x7F;                                //Nesting ������ ���� status resister bit7�� 0���� �����Ͽ� ��� ���ͷ�Ʈ�� ����(global interrupt disable)
    EINT4_FLAG_BIT=1;                            //interrupt ª�� ���α׷����ϱ� ���� �̷��� ������. ���ͷ�Ʈ4�� ���� 1�̵Ǹ� while�� �ȿ� �����ص� ������ ����
    SREG |= 0x80;                                //status resister bit7�� 1���� �����Ͽ� ��� ���ͷ�Ʈ�� ���(global interrupt enable)
}                                                      
 
interrupt [EXT_INT5] void external_int5(void){ 
    SREG &= 0x7F;
    EINT5_FLAG_BIT=1;     
    SREG |= 0x80;  
} 
                                
interrupt [EXT_INT6] void external_int6(void){ 
    SREG &= 0x7F;  
    EINT6_FLAG_BIT=1;  
    SREG |= 0x80;   
}

interrupt [EXT_INT7] void external_int7(void){ 
    SREG &= 0x7F;  
    EINT7_FLAG_BIT=1;       
    SREG |= 0x80;       
}

interrupt [TIM2_OVF] void TIMER2_OVF_int(void){ 
    SREG &= 0x7F;  
    TIMER2_OVF_FLAG_BIT=1; 
    TCNT2 = 174;                                 //�ʱⰪ�� ���ͷ�Ʈ �ȿ��� �������
    SREG |= 0x80;         
}

interrupt [TIM1_COMPA] void TIMER1_CTC_int(void){ 
    SREG &= 0x7F;   
    TIMER1_CTC_FLAG_BIT=1;        
    SREG |= 0x80;         
}



void EXIT4_TIMER2(void){            
    TIMSK |= 0b01000000;                        // overflow interrupt enable
    TCCR2 = 0b00000101;                         // normal���, ����������= 1024����
    TCNT2 = 174;                                // 174=5.184ms (�й����ڸ� 51�̶� 8��Ʈ�� 51ms�Ұ�)
                                    
} //end of EXIT4_TIMER2              
  
void EXIT5_TIMER1(void){
    TCCR1A = 0b00000000;                        // mode4
    TCCR1B = 0b00001101;                        // mode4(CTC), ���������� = 1024����  
    TCCR1C = 0x00;
          
    TCNT1H = 0x0;                               //�������� �ʱ�ȭ
    TCNT1L = 0x0;                               //�������� �ʱ�ȭ   
    OCR1AH= 0x1F;                               //7968,hex1F20�� ���� ��.
    OCR1AL = 0x20;                              //510ms (5100ms 16��Ʈ�� �Ұ��Ͽ� 510ms�� ����)
    TIMSK |= 0x10;                              //compare match interrupt enable
                                   
} //end of EXIT5_TIMER1

void EXIT6_PWM_M6(void){           
    TCCR3A = 0b10000010;                        //OC3A(PE3)�� �������, mode6 
    TCCR3B = 0b00001100;                        //mode6(Fast PWM���),256���� - 122.07Hz ����
    TCCR3C = 0x00;              
      
    TCNT3H = 0x0;                               //�������� �ʱ�ȭ
    TCNT3L = 0x0;                               //�������� �ʱ�ȭ    
    OCR3AH= 0x01;                               //260,hex104�� ���� ��.
    OCR3AL = 0x04;                     
    
    DDRE |= 0x08;                               //TIMER3 PWM PIN PE3�� DDRE 3�� ��¼����ؾ� PWM ������ ��µ�
} //end of EXIT6_PWM_M6

void EXIT7_PWM_M2(void){
    TCCR3A = 0b10000010;                        //OC3A(PE3)�� �������, mode2 
    TCCR3B = 0b00000100;                        //mode2(PWM Phase correct ���),256���� - 61.035Hz
    TCCR3C = 0x00;              
      
    TCNT3H = 0x0;                               //�������� �ʱ�ȭ
    TCNT3L = 0x0;                               //�������� �ʱ�ȭ   
    OCR3AH= 0x01;                               //260,hex104�� ���� ��.
    OCR3AL = 0x04; 
    DDRE |= 0x08;                               //PE3 ��� 
} //end of EXIT7_PWM_M2




void SEG7_TEMP_DIS(int val){ 
    float fval;
    int ival, buf; 
         
    fval = (float)val * 5.0 / 1023.0;             // �µ������� ��ȯ
    ival = (int)(fval * 1000.0);                  // �ݿø� �� ����ȭ(�Ҽ� ù°�ڸ�����)    
    
    NA100 = ival / 100;                           // ������ 10�� �ڸ� ����
    buf = ival % 100; 
    NA10 = buf / 10;                              // ���ڸ��� ����
    NA1 = buf % 10;                               // �Ҽ� ù° �ڸ� ���� 
    
    PORTG = 0b00001000;                           // PG3=1, �Ҽ� ù° �ڸ�
    PORTD = ((seg_pat[NA1] & 0x0F) << 4) | (PORTD & 0x0F);  //PORTD ���� 4bits ������� �ʰ� |
    PORTB = (seg_pat[NA1] & 0x70 ) | (PORTB & 0x0F);        //PORTB ���� 4bits ������� �ʰ� |
    delay_ms(1);
    PORTG = 0b00000100;                           // PG2=1
    PORTD = 0x00;                                   
    PORTB = 0x80;                                 // DP on(�Ҽ���) 
    delay_ms(1);
    PORTG = 0b00000010;                           // PG1=1, ���� ���� �ڸ�      
    PORTD = ((seg_pat[NA10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[NA10] & 0x70 ) | (PORTB & 0x0F); 
    delay_ms(1);   
    PORTG = 0b00000001;                           // PG0=1,���� 10�� �ڸ�
    PORTD = ((seg_pat[NA100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[NA100] & 0x70 ) | (PORTB & 0x0F);                                                                                                                                           
    delay_ms(1); 
} //end of SEG7_TEMP_DIS


void AD_display(int value) { 
    float fvalue;
    int ivalue, buff; 

    fvalue = (float)value * 5.0 / 1024.0;        // ���� ������ ��ȯ
    ivalue = (int)(fvalue * 100.0 + 0.5);        // �ݿø� �� ����ȭ,(�Ҽ� ��°�ڸ�����) 
    
    NV100 = ivalue / 100;                        // ������ ����
    buff = ivalue % 100; 
    
    NV10 = buff / 10;                            // �Ҽ� ù° �ڸ� ����
    NV1 = buff % 10;                             // �Ҽ� ��° �ڸ� ����
} //end of AD_display


void UART_volt(){    
    UCSR0A = 0x0;               // USART �ʱ�ȭ 
    UCSR0B = 0b00001000;        // �۽� �ο��̺� TXEN = 1 
    UCSR0C = 0b00000110;        // �񵿱�[7], ������ 8��Ʈ ��� 
    UBRR0H = 0;                 // X-TAL = 16MHz �϶�, BAUD = 9600
    UBRR0L = 103;  
                                        
    while((UCSR0A & 0x20) == 0x0);     //�۽� ���۰� ������� UDRE0[5]= 1�� �� ������ ���       
     UDR0 =NV100+0x30;                 //�� ���� ���� �ƽ�Ű�ڵ�� ������ ����
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 ='.';                        //����'.'����
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =NV10+0x30;                  //�� ���� ���� �ƽ�Ű�ڵ�� ������ ����
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =NV1+0x30;                   //�� ���� ���� �ƽ�Ű�ڵ�� ������ ����
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 ='V';                        //����'V'����
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =' ';                        //�˾ƺ��� ���� ����
} //end of UART_volt





