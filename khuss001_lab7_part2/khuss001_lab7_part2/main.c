/*
 * khuss001_lab7_part2.c
 *
 * Created: 8/24/2018 6:00:42 PM
 * Author : Keith
 */ 

#include <avr/io.h>

enum States { START, OFF, ON, PRESS_0, PRESS_1, PRESS_2 } state;
unsigned char button_0 = 0x00;
unsigned char button_1 = 0x00;
unsigned char button_2 = 0x00;
unsigned short powtog = 0;
double codec = 261.63;
double codec_hold = 0;

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void Tick(){
button_0 = ~PINC & 0x01;
button_1 = ~PINC & 0x02;
button_2 = ~PINC & 0x04;

	switch(state){ // Transitions
		case START:
		state = OFF;
		break;
		
		case OFF:
		if (button_0){
			state = PRESS_0;
		}
		break;
		
		case ON:
		if (button_0){
			state = PRESS_0;
		}
		else if (button_1){
			state = PRESS_1;
		}
		else if (button_2){
			state = PRESS_2;
		}
		break;
		
		case PRESS_0:
		if (!button_0){
			if (!powtog){
				powtog = 1;
				state = ON;
				set_PWM(codec);
			}
			else{
				powtog = 0;
				state = OFF;
				set_PWM(0);
			}
		}
		break;
					
		case PRESS_1: // Decrease frequency
		if (!button_1){
			if (codec == 293.66){
				codec = 261.63;
			}
			else if (codec == 329.63){
				codec = 293.66;
			}
			else if (codec == 349.23){
				codec = 329.63;
			}
			else if (codec == 392.00){
				codec = 349.23;
			}
			else if (codec == 440.00){
				codec = 392.00;
			}
			else if (codec == 493.88){
				codec = 440.00;
			}
			else if (codec == 523.25){
				codec = 493.88;
			}
			state = ON;
		}
		break;
		
		case PRESS_2: // Increase frequency
		if (!button_2){
			if (codec == 261.63){
				codec = 293.66;
			}
			else if (codec == 293.66){
				codec = 329.63;
			}
			else if (codec == 329.63){
				codec = 349.23;
			}
			else if (codec == 349.23){
				codec = 392.00;
			}
			else if (codec == 392.00){
				codec = 440.00;
			}
			else if (codec == 440.00){
				codec = 493.88;
			}
			else if (codec == 493.88){
				codec = 523.25;
			}
			state = ON;
		}
		break;
	}//Transitions

	switch(state){ // state actions
		case START:
		break;
		
		case OFF:
		break;
		
		case ON:
		set_PWM(codec);
		break;
		
		case PRESS_0:
		break;
		
		case PRESS_1:
		break;
		
		case PRESS_2:
		break;
	}// State actions
}

int main(void)
{
	DDRC = 0x00; PORTC = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
//	set_PWM(codec);
	state = START;
    /* Replace with your application code */
    while (1) 
    {
		Tick();
    }
}

