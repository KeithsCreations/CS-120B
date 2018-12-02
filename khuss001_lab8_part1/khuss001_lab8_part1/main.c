/*
 * khuss001_lab8_part1.c
 *
 * Created: 8/27/2018 12:11:40 PM
 * Author : Keith
 */ 

#include <avr/io.h>

unsigned short my_short = 0x00;
unsigned char my_char;

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
		PORTB = my_char;
		my_char = (char)(my_short >> 8);
		PORTD = my_char;
		break;
		
	}//Transitions
}

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00;
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

