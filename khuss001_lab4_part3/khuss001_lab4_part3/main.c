/*
 * khuss001_lab4_part3.c
 *
 * Created: 8/20/2018 3:34:08 PM
 * Author : Keith
 */ 

#include <avr/io.h>

enum States {START, RELEASE, PRESS} State;
	
unsigned char seq[4] = {0x00, 0x15, 0x2A, 0x3F};
unsigned short i = 0;
unsigned char button = 0x00;

Tick(){
	button = ~PINC & 0x01;
	switch(State){ // Transitions
		case START:
		State = RELEASE;
		break;
		case RELEASE:
		if (button){
			State = PRESS;
		}
		break;
		case PRESS:
		if (!button){
			State = RELEASE;
			if (i < 3){
				i++;
			}
			else{
				i = 0;
			}
		}
		break;
	}//Transitions

	switch(State){ // state actions
		case RELEASE:
	PORTD = seq[i];
	break;
	}// State actions	
}

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0x00; PORTC = 0xFF;
	State = START;
    while (1) 
    {

		Tick();
    }
}
