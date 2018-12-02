/*
 * khuss001_lab4_part2.c
 *
 * Created: 8/20/2018 11:13:16 AM
 * Author : Keith
 */ 

#include <avr/io.h>

unsigned short max = 9;
unsigned short min = 0;
unsigned char count = 0;
unsigned char button_1 = 0x00; //variable for PORTA
unsigned char button_2 = 0x00; //variable for PORTA

enum UP_states { START1, SM1_RESET, SM1_ON, SM1_INCREASE_SLOW, SM1_INCREASE_FAST } up_state;
enum DOWN_states { START2, SM2_RESET, SM2_ON, SM2_DECREASE_SLOW, SM2_DECREASE_FAST } down_state;

void Tick(){
button_1 = ~PINC & 0x01;
button_2 = ~PINC & 0x02;

	switch(up_state){ // Transitions
		case START:
		up_state = SM1_RESET;
		break;
		case SM1_RESET:
		if (!button_1 && !button_2) {
			up_state = SM1_ON;
		}
		else if (button_1 && button_2) {
			up_state = SM1_RESET;
		}
		break;
		case SM1_ON:
		if (!button_1 && !button_2) {
			up_state = SM1_ON;
		}
		else if (button_1 && button_2) {
			up_state = SM1_RESET;
	}
	else if (((button_1 && !button_2) && count < max)) {
			up_state = SM1_INCREASE_SLOW;
		}
// 		else if (((!button_1 && button_2) && count > min)) {
// 			state = SM1_DECREASE;
// 		}
		break;
// 		case SM1_DECREASE:
// 		if (!button_1 && !button_2) {
// 			state = SM1_ON;
// 			count--;
// 		}
// 		else if ((!button_1 && button_2)) {
// 			state = SM1_DECREASE;
// 		}
// 		else if (button_1 && button_2) {
// 			state = SM1_RESET;
// 		}
// 		break;
		case SM1_INCREASE_SLOW:
		if (!button_1 && !button_2) {
			state = SM1_ON;
			count++;
		}
		else if (button_1 && !button_2) {
			state = SM1_INCREASE;
		}
		else if (button_1 && button_2) {
			state = SM1_RESET;
		}
		break;
		default:
		state = SM1_RESET;
	}//Transitions

	switch(state){ // state actions
		case SM1_RESET:
		count = 0;
		PORTD = count;
		break;
		case SM1_ON:
		PORTD = count;
		break;
		case SM1_INCREASE_SLOW:
		hold_c
		break;
		case SM1_INCREASE_FAST:
		
		break;
		default: // ADD default behaviour below
		break;
	}// State actions
}

void main(){
	DDRD = 0xFF; PORTD = 0x00; // Initialize outputs
	DDRC = 0x00; PORTC = 0xFF;
	state = START; // Indicates initial call
	while(1) {
				Tick();
	}
}