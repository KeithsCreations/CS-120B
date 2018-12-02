/*
 * khuss001_lab3_part5.c
 *
 * Created: 8/15/2018 1:32:50 PM
 * Author : Keith
 */ 

#include <avr/io.h>

enum SM1_States { START, SM1_LOCKED, SM1_PERHAPS_1, SM1_PERHAPS_2, SM1_PERHAPS_3, SM1_PERHAPS_4, SM1_UNLOCKED, UNPERHAPS, UNPERHAPS_2 } SM1_State;

unsigned char code[4] = {0x04, 0x01, 0x02, 0x01};
	unsigned short i = 0;
	unsigned short j = 0;
TickFct_State_machine_1() {
	switch(SM1_State) { // Transitions
		case START:
		SM1_State = SM1_LOCKED;
		break;
		case SM1_LOCKED:
		if ((PINA == 0x00)
		) {
			SM1_State = SM1_PERHAPS_1;
			i = 0;
			j = 0;
		}
		break;
		case SM1_PERHAPS_1: //MAIN LOOP
		if (PINA == code[i] && ((PINA & 0x80) == 0x00) && i < 4)
		 {
			SM1_State = SM1_PERHAPS_2;
		}
		else if (i == 4)
		 {
			 if (j == 1){
				 SM1_State = SM1_LOCKED;
			 }
			 else{
			SM1_State = SM1_UNLOCKED;}
		}
		else if (PINA & 0x80) {
			SM1_State = SM1_LOCKED;
		}
		else if ((PINA != code[i] && PINA != 0x00))
		{
			if (j == 1){
				SM1_State = SM1_UNLOCKED;
			}
			else{
			SM1_State = SM1_LOCKED;}
		}
		break;
		case SM1_PERHAPS_2: 
		if (PINA == 0x00)
		 {
			SM1_State = SM1_PERHAPS_1;
			i++;
		}
		else if (PINA < 5) {
			break;
		}
		else if (PINA & 0x80 == 0x80) {
			SM1_State = SM1_LOCKED;		
			}
		break;
		case SM1_PERHAPS_3:
		if (PINA == 0x00 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = SM1_UNLOCKED;
						
		}
		else if (PINA == 0x02 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = SM1_PERHAPS_3;
		}
		break;
		case SM1_PERHAPS_4:
		if (PINA == 0x00 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = SM1_UNLOCKED;			
		}
		else if (PINA == 0x04
		) {
			SM1_State = SM1_PERHAPS_4;
		}
		break;
		case SM1_UNLOCKED:
		i = 0;
		j = 1;
		SM1_State = SM1_PERHAPS_1;
		break;
		case UNPERHAPS: //ignore
		if (PINA == 0x00 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = UNPERHAPS_2;
			
		}
		else if (PINA == 0x04 && ((PINA & 0x80) == 0x00)
		) {
			SM1_State = UNPERHAPS; 
		}
		break;
		case UNPERHAPS_2: //            ignore
		if ((PINA == 0x01 || PINA == 0x80)
		) {
			SM1_State = SM1_UNLOCKED;
		}
		else if (PINA == 0x02
		) {
			SM1_State = SM1_LOCKED;
			PORTB = 0x00;
		}
		else if (PINA == 0x04){
			SM1_State = SM1_PERHAPS_4;
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
		case SM1_PERHAPS_3:
					PORTC = 0x03;
		break;
		case SM1_PERHAPS_4:
					PORTC = 0x07;
		break;
		case SM1_UNLOCKED:
					PORTC = 0x04;
					PORTB = 0x01;
		break;
		case UNPERHAPS:
					PORTC = 0x05;
		break;
		case UNPERHAPS_2:
					PORTC = 0x06;
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
