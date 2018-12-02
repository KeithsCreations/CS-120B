/*
 * khuss001_lab2_part1.c
 *
 * Created: 8/9/2018 8:00:50 PM
 * Author : Keith
 */ 

#include <avr/io.h>

unsigned char getBit(unsigned char x, unsigned char k){
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
   DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
   DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as outputs
   DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
   unsigned int tmpC = 0x00; // used for updating C
	unsigned int i = 0x00;
    while (1) 
    {
		tmpC = 0x00;
		for (i = 0; i < 0x08; ++i)
		{
		//if there is a bit that is == 1, add 1 to PINC
		if (getBit(PINA, i)){
			tmpC += 1;
			}
		if (getBit(PINB, i)) {
			tmpC += 1;
		}
		PORTC = tmpC;
	}
}
}