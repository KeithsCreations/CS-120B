/*
 * khuss001_lab4_example.c
 *
 * Created: 8/16/2018 3:13:27 PM
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
	DDRD = 0xFF; PORTD = 0x00; // Initialize outputs
	DDRC = 0x00; PORTC = 0xFF;
	unsigned char tmpA = 0x00;
	unsigned char button_1 = 0x00; //variable for PORTA
	unsigned char button_2 = 0x00;
	unsigned char button_3 = 0x00;
	unsigned char button_4 = 0x00;
	while (1)
	{
		button_1 = ~PINC & 0x01;
		button_2 = ~PINC & 0x02;
		button_3 = ~PINC & 0x04;
		button_4 = ~PINC & 0x08;
		//tmpC = ((PINA & 0x0F));
		
		/*Setting the bits*/
		//PC0
		if ((button_1 | button_2 | button_3 | button_4) >= 0x0D){
			tmpA = SetBit(tmpA, 0, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) < 0x0D){
			tmpA = SetBit(tmpA, 0, 0);
		}
		//PC1
		if ((button_1 | button_2 | button_3 | button_4) >= 0x0A){
			tmpA = SetBit(tmpA, 1, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) < 0x0A){
			tmpA = SetBit(tmpA, 1, 0);
		}
		//PC2
		if ((button_1 | button_2 | button_3 | button_4) >= 0x07){
			tmpA = SetBit(tmpA, 2, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) < 0x07){
			tmpA = SetBit(tmpA, 2, 0);
		}
		//PC4
		if ((button_1 | button_2 | button_3 | button_4) >= 0x04){
			tmpA = SetBit(tmpA, 4, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) < 0x04){
			tmpA = SetBit(tmpA, 4, 0);
		}
		//PC3
		if ((button_1 | button_2 | button_3 | button_4) >= 0x05){
			tmpA = SetBit(tmpA, 3, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) < 0x05){
			tmpA = SetBit(tmpA, 3, 0);
		}
		//PC5
		if ((button_1 | button_2 | button_3 | button_4) >= 0x01){
			tmpA = SetBit(tmpA, 5, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) < 0x01){
			tmpA = SetBit(tmpA, 5, 0);
		}
		//PC6
		if ((button_1 | button_2 | button_3 | button_4) <= 0x04){
			tmpA = SetBit(tmpA, 6, 1);
		}
		if ((button_1 | button_2 | button_3 | button_4) > 0x04){
			tmpA = SetBit(tmpA, 6, 0);
		}
		//PC7
		if (GetBit(PINC, 4) && GetBit(PINC, 5) && ~(GetBit(PINC, 6)))
		{
			tmpA = SetBit(tmpA, 7, 1);
		}
		if ((GetBit(PINC, 4) && GetBit(PINC, 5) && ~(GetBit(PINC, 6))) == 0)
		{
			tmpA = SetBit(tmpA, 7, 0);
		}
		PORTD = tmpA;
	}
}


