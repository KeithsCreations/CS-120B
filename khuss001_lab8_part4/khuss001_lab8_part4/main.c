/*
 * khuss001_lab8_part4.c
 *
 * Created: 8/27/2018 3:52:33 PM
 * Author : Keith
 */ 

#include <avr/io.h>
//Bit Access Funcitons
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned short my_short = 0x00;
unsigned char my_char;
unsigned char tmpB = 0x00;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}

enum States { START, ON } state;

void Tick(){

	switch(state){ // Transitions
		case START:
		state = ON;
		break;
		
		case ON:
		my_short = ADC;
		my_char = (char)my_short;
		if (my_short > 255){
			my_char = 255;
		}
		if (my_char >= 227){
			tmpB = SetBit(tmpB, 0, 1);
		}
		if (my_char < 227){
			tmpB = SetBit(tmpB, 0, 0);
		}
		if (my_char >= 200){
			tmpB = SetBit(tmpB, 1, 1);
		}
		if (my_char < 200){
			tmpB = SetBit(tmpB, 1, 0);
		}
		if (my_char >= 172){
			tmpB = SetBit(tmpB, 2, 1);
		}
		if (my_char < 172){
			tmpB = SetBit(tmpB, 2, 0);
		}
		if (my_char >= 145){
			tmpB = SetBit(tmpB, 3, 1);
		}
		if (my_char < 145){
			tmpB = SetBit(tmpB, 3, 0);
		}
		if (my_char >= 117){
			tmpB = SetBit(tmpB, 4, 1);
		}
		if (my_char < 117){
			tmpB = SetBit(tmpB, 4, 0);
		}
		if (my_char >= 90){
			tmpB = SetBit(tmpB, 5, 1);
		}
		if (my_char < 90){
			tmpB = SetBit(tmpB, 5, 0);
		}
		if (my_char >= 62){
			tmpB = SetBit(tmpB, 6, 1);
		}
		if (my_char < 62){
			tmpB = SetBit(tmpB, 6, 0);
		}
		if (my_char > 35){
			tmpB = SetBit(tmpB, 7, 1);
		}
		if (my_char <= 35){
			tmpB = SetBit(tmpB, 7, 0);
		}
		PORTB = tmpB;
		break;
		
	}//Transitions
}

int main(void)
{
	//DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	
	ADC_init();
	
	my_short = ADC;
	
	state = START;
	
	/* Replace with your application code */
	while (1)
	{
		Tick();
	}
}

