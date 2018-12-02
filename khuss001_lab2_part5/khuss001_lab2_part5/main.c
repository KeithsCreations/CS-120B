/*
 * khuss001_lab2_part5.c
 *
 * Created: 8/10/2018 9:20:56 AM
 * Author : Keith
 */ 

#include <avr/io.h>
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRD = 0x00; PORTD = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFE; PORTB = 0x00; // Configure port B's 8 pins as outputs
	unsigned char tmpB = 0x00; //variable for PORTA
	unsigned int tmpD = 0x00;
	while (1)
	{
		tmpD = PIND;
		tmpD = ((tmpD << 1) + (PINB & 0x01));
		
		if (tmpD >= 0x46) {
			tmpB = SetBit(tmpB, 1, 1);
			tmpB = SetBit(tmpB, 2, 0);
		}
		else if (tmpD < 0x46 && tmpD > 0x05){
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 1);
		}
		else{
				tmpB = SetBit(tmpB, 1, 0);								
				tmpB = SetBit(tmpB, 2, 0);
		}
		PORTB = tmpB;
	}
}