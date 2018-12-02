/*
 * khuss001_lab1_part4.c
 *
 * Created: 8/9/2018 5:12:31 PM
 * Author : Keith
 */ 

#include <avr/io.h>


int main(void)
{
	//inputs
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	//output
	DDRD = 0xFF; PORTD = 0x00;
	unsigned short all;
	//difference between a and c
	unsigned char difference;
    while (1) 
    {
  		all = PINA + PINB + PINC;
		difference = PINA - PINC;
		unsigned char sum = all;
		PORTD = (sum & 0xFC);
		//if the sum is more than 140 kg, set PD0 to 1
		if (all > 140)
		{
			PORTD = PORTD | 0x01;
		}
		
		
		//if A - C is > 80, then set PD1 to 1
		if (difference > 80){
			PORTD = PORTD | 0x02;
		}
	
		
		 /* Checking the remaining bits on PD3 - PD7 */
		
		//PD7

		
    }
}

