/*
 * khuss001_lab2_part2.c
 *
 * Created: 9/12/2018 11:56:59 PM
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
   DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
   unsigned char tmpC = 0x00; //variable for PORTA
   unsigned char tmpA = 0x00;
    while (1) 
    {
	tmpC = ((PINA & 0x0F));
		/*Setting the bits*/
		//PC0
		if ((PINA & 0x0F) >= 0x0D){
			tmpA = SetBit(tmpA, 0, 1);
		}
		if ((PINA & 0x0F) < 0x0D){
			tmpA = SetBit(tmpA, 0, 0);
		}
		//PC1
		if ((PINA & 0x0F) >= 0x0A){
			tmpA = SetBit(tmpA, 1, 1);
		}
		if ((PINA & 0x0F) < 0x0A){
			tmpA = SetBit(tmpA, 1, 0);
		}
		//PC2
		if ((PINA & 0x0F) >= 0x07){
			tmpA = SetBit(tmpA, 2, 1);
		}
		if ((PINA & 0x0F) < 0x07){
			tmpA = SetBit(tmpA, 2, 0);
		}
		//PC4
		if ((PINA & 0x0F) >= 0x04){
			tmpA = SetBit(tmpA, 4, 1);
		}
		if ((PINA & 0x0F) < 0x04){
			tmpA = SetBit(tmpA, 4, 0);
		}
		//PC3 
		if ((PINA & 0x0F) >= 0x05){
			tmpA = SetBit(tmpA, 3, 1);
		}
		if ((PINA & 0x0F) < 0x05){
			tmpA = SetBit(tmpA, 3, 0);
		}
		//PC5
		if ((PINA & 0x0F) >= 0x01){
			tmpA = SetBit(tmpA, 5, 1);
		}
		if ((PINA & 0x0F) < 0x01){
			tmpA = SetBit(tmpA, 5, 0);
		}
		//PC6
		if ((PINA & 0x0F) <= 0x04){
			tmpA = SetBit(tmpA, 6, 1);
		}
		if ((PINA & 0x0F) > 0x04){
			tmpA = SetBit(tmpA, 6, 0);
		}
		//PC7
		if (GetBit(PINA, 4) && GetBit(PINA, 5) && ~(GetBit(PINA, 6)))
		{
			tmpA = SetBit(tmpA, 7, 1);
		}
		if ((GetBit(PINA, 4) && GetBit(PINA, 5) && ~(GetBit(PINA, 6))) == 0)
		{
			tmpA = SetBit(tmpA, 7, 0);
		}
		PORTC = tmpA;
		}
    }


