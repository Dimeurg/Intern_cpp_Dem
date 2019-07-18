#include <mega8.h>
#include <delay.h>      

void main(void){   
     
DDRB = 0b00001110;
PORTB = 0x00;

DDRC = 0b00001110;
PORTC = 0b00000001;

DDRD = 0b00001110;
PORTD = 0x00;

while (1){
               if(PINC.0 == 0){
                PORTC.1 = 0; PORTC.2 = 1; PORTC.3 = 0;        
                PORTD.1 = 0; PORTD.2 = 0; PORTD.3 = 0;
                PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 0;
                delay_ms(500);
                         
                PORTC.1 = 1; PORTC.2 = 1; PORTC.3 = 1;        
                PORTD.1 = 0; PORTD.2 = 1; PORTD.3 = 0;
                PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 0;
                delay_ms(500);  
                
                PORTC.1 = 0; PORTC.2 = 0; PORTC.3 = 0;        
                PORTD.1 = 1; PORTD.2 = 1; PORTD.3 = 1;
                PORTB.1 = 0; PORTB.2 = 1; PORTB.3 = 0;
                delay_ms(500);        
                
                PORTC.1 = 0; PORTC.2 = 0; PORTC.3 = 0;        
                PORTD.1 = 0; PORTD.2 = 0; PORTD.3 = 0;
                PORTB.1 = 1; PORTB.2 = 1; PORTB.3 = 1;
                delay_ms(500);             
                
                PORTC.1 = 0; PORTC.2 = 0; PORTC.3 = 0;        
                PORTD.1 = 0; PORTD.2 = 0; PORTD.3 = 0;
                PORTB.1 = 0; PORTB.2 = 0; PORTB.3 = 0;
                delay_ms(500);
                }
               
    }
}
