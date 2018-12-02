/*
 * khuss001_lab3_part3.c
 *
 * Created: 8/14/2018 9:17:07 PM
 * Author : Keith
 */ 

#include <avr/io.h>


enum SM1_States { START, SM1_LOCKED, SM1_PERHAPS_1, SM1_PERHAPS_2, SM1_UNLOCKED } SM1_State;

TickFct_State_machine_1() {
	switch(SM1_State) { // Transitions
		case START:
		SM1_State = SM1_LOCKED;
		break;
		case SM1_LOCKED:
		if ((PINA == 0x04) && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = SM1_PERHAPS_1;
		}
		break;
		case SM1_PERHAPS_1:
		if (PINA == 0x00 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = SM1_PERHAPS_2;
			
		}
		else if (PINA == 0x04 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = SM1_PERHAPS_1;
		}
		break;
		case SM1_PERHAPS_2:
		if ((PINA == 0x01 || PINA == 0x80)
		) {
			SM1_State = SM1_LOCKED;
		}
		else if (PINA == 0x02

		) {
			SM1_State = SM1_UNLOCKED;
			PORTB = 0x01;
		}
		break;
		case SM1_UNLOCKED:
		if ((PINA & 0x80) == 0x80) {
			SM1_State = SM1_LOCKED;
		}
		break;
		default:
		SM1_State = SM1_LOCKED;
	} // Transitions

	switch(SM1_State) { // State actions
		case SM1_LOCKED:
		PORTB = 0x00;
		PORTC = 0x00;
		break;
		case SM1_PERHAPS_1:
					PORTC = 0x01;
		break;
		case SM1_PERHAPS_2:
					PORTC = 0x02;
		break;
		case SM1_UNLOCKED:
					PORTC = 0x03;
		break;
		default: // ADD default behaviour below
		break;
	} // State actions

}

int main() {

	//const unsigned int periodState_machine_1 = 1000; // 1000 ms default
	//TimerSet(periodState_machine_1);
	// TimerOn();
	
	SM1_State = START; // Initial state
	PORTB = 0; // Init outputs

	while(1) {
		TickFct_State_machine_1();
	} // while (1)
} // Main

