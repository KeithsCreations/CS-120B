/*
 * khuss001_lab2_part4.c
 *
 * Created: 8/10/2018 7:58:46 AM
 * Author : Keith
 */ 

#include <avr/io.h>
// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
	unsigned char tmpC = 0x00; //variable for PORTA
	unsigned char tmpB = 0x00;
	unsigned int i = 0;
    while (1) 
    {
		for (i = 4; i < 8; i++){
			if (GetBit(PINA, i)){
				tmpB = SetBit(tmpB, i-4, 1);
			}
			else{
				tmpB = SetBit(tmpB, i-4, 0);
			}
		}
		for (i = 0; i < 4; i++){
			if (GetBit(PINA, i)){
				tmpC = SetBit(tmpC, i+4, 1);
			}
			else{
				tmpC = SetBit(tmpC, i+4, 0);
			}
		}
		PORTB = tmpB;
		PORTC = tmpC;
    }
}

