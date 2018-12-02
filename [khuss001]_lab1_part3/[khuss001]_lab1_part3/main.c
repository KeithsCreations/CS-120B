/*
 * [khuss001]_lab1_part3.c
 *
 * Created: 8/9/2018 9:40:41 PM
 * Author : Keith
 */ 

#include <avr/io.h>
int main(void)
{
 	DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char i = 0x00;
	unsigned char  cntavail = 0x04; // Temporary variable to hold the value of B
	for (i= 0; i < 4; i++)
	{
		// if PA0 is 1, set PB1PB0=01, else =10
		if (PINA & (0x01 << i)) { // True if PA0 is 1
			cntavail = cntavail - 1; // Sets tmpB to bbbbbb01
			
			// (clear rightmost 2 bits, then set to 01)
		}
	}
	PORTC = cntavail;
	if (cntavail == 0){
		PORTC = PORTC | 0x80;
	}
	return 0;
}
