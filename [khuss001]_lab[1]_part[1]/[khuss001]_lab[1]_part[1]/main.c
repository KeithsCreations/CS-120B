/*
 * [khuss001]_lab[1]_part[1].c
 *
 * Created: 8/6/2018 3:41:19 PM
 * Author : Keith
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PINA = 0x00; // Configure port A's 8 pins as inputs
	DDRC = 0x00; PINC = 0x00;
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	// Initialize output on PORTB to 0x00
	while(1)
	{
		if (PINA == 0x01 && PINC == 0x00)
		{
			PORTB = 0x01;
		}
	}
}
