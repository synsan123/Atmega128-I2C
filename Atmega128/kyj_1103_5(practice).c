/*
 * kyj_1103_5(practice).c
 *
 * Created: 2023-11-06 오전 8:47:09
 * Author: user
 */  
 
#include <mega128a.h>

void main(void)
{
    unsigned char ago_sw1, now_sw1;
    unsigned char led = 0xFE;
    
    DDRC = 0xFF;
    DDRE = 0x00;
    PORTC = led;
    
    ago_sw1 = PINE & 0x10; 
    //PINE의 기본값은 1111 1111임 
    // 스위치를 누르지 않으면 1의 값을 기본으로 가지기 때문(스위치 OFF상태)
    //사실 그냥 0001 0000을 AND하면 PINE의 값은 4번자리 빼고는 다 0임.
    // 0x10 = 0b00010000이다. PINE4를 불러오겠다. 스위치1을 불러오겠다.
    // ago_sw1의 현재상태는 OFF 상태이다.
    
    while (1)
        {
         now_sw1 = PINE & 0x10; // 설명은 위와 동일하다.
         
         if(ago_sw1 != 0 && now_sw1 == 0) // ago_sw1가 1이고 now_sw1가 0일 때
                                        //달리 말하면 이전스위치가 OFF이고 현재스위치가 ON일 때
         {
          led = (led << 1) | 0x01;
           
          // led를 1111 1110에서 스위치를 누르면
          // 한 칸 이동 후 1과 OR를 하여 전에 ON을 한  LED를 OFF 시킨다.
          // 이 값은 밑의 조건문의 조건이 달성 될 때까지 스위치를 눌러 계속해서 반복된다.
         
          if(led == 0xFF) // 스위치를 누르던 중 LED가 전부 OFF 되었을 때 LED를 0xFE로 초기화 한다.
          {
            led = 0xFE;
          }
          
          PORTC = led; //초기화한 LED값을 PORTC에 대입한다.
         }
         
         ago_sw1 = now_sw1; 
         
         //스위치를 누를 때마다 ago_sw1에 now_sw1의 값을 저장시킨다.
         //예를 들어 위 과정에서 초기 상태는 둘 다 OFF였다. 그러나 스위치를 누름으로써 now_sw1는 ON(0)이 되고
         //ago_sw1은 OFF(1) 상태이다. 그리고 now_sw1의 ON값을 ago_sw1에 저장하여 둘 다 ON을 만들어준다.
         //now_sw1에서 손을 떼면 now_sw1은 OFF가 되고 ago_sw1은 ON이 된다.
         //정리하면 1. OFF OFF 2. OFF ON 3. ON ON 4. ON OFF 가 된다.
         //그리고 now_sw1의 OFF값이 ago_sw1에 저장되어 5. OFF OFF 가 된다.
         //그러면 1부터 4까지가 계속해서 무한 반복 된다.
                              
        }
}
