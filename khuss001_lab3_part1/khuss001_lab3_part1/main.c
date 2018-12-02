/*
 * khuss001_lab3_part1.c
 *
 * Created: 8/11/2018 8:11:37 PM
 * Author : Keith
 */ 

#include <avr/io.h>

//DDRA = OxOO; PORTA = OxFF;
//DDRB = OxFF; PORTB = Ox00;
unsigned char SM1_Clk;
unsigned char tmpB0 = 0x00;
//unsigned char tmpB1 = 0x00;
//unsigned char PA0 = 0x00;
void TimerISR() {
	SM1_Clk = 1;
}

enum SM1_States { START, SM1_OFF_RELEASE, SM1_ON_PRESS, SM1_ON_RELEASE, SM1_OFF_PRESS } SM1_State;

void TickFct_State_machine_1() {
	switch(SM1_State) { // Transitions
		SM1_State = SM1_OFF_RELEASE;
		break;
		case SM1_OFF_RELEASE:
		if (PINA == 0x01) {
			SM1_State = SM1_ON_PRESS;
		}
		//else if (PINA == 0x00) {
		//	SM1_State = SM1_OFF_RELEASE;
		//}
		break;
		case SM1_ON_PRESS:
		if (PINA == 0x00) {
			SM1_State = SM1_ON_RELEASE;
		}
		//else if (PINA == 0x01) {
		//	SM1_State = SM1_ON_PRESS;
		//}
		break;
		case SM1_ON_RELEASE:
		if (PINA == 0x01) {
			SM1_State = SM1_OFF_PRESS;
		}
		//else if (PINA == 0x00) {
		//	SM1_State = SM1_ON_RELEASE;
		//}
		break;
		case SM1_OFF_PRESS:
		if (PINA == 0x00) {
			SM1_State = SM1_OFF_RELEASE;
		}
		//else if (PINA == 0x01) {
		//	SM1_State = SM1_OFF_PRESS;
		//}
		break;
		default:
		SM1_State = SM1_OFF_RELEASE;
		break;
		
	} // Transitions

	switch(SM1_State) { // State actions
		case SM1_OFF_RELEASE:
		tmpB0 = 0x01;
		PORTB = tmpB0;
		break;
		case SM1_ON_PRESS:
		tmpB0 = 0x02;
		//tmpB1 = 0x02;
		PORTB = tmpB0;
		break;
		case SM1_ON_RELEASE:
		break;
		case SM1_OFF_PRESS:
		tmpB0 = 0x01;
		//tmpB1 = 0x00;
		PORTB = tmpB0;
		break;
		default: // ADD default behaviour below
		break;
	} // State actions

}

int main() {

	//const unsigned int periodState_machine_1 = 1000; // 1000 ms default
	//TimerSet(periodState_machine_1);
	//TimerOn();
	
	SM1_State = START; // Initial state
//DDRA = 0x00; PORTA = 0xFF;
//DDRB = 0xFF; PORTB = 0x00;
//unsigned char SM1_Clk;

	while(1) {
		TickFct_State_machine_1();
	} // while (1)
} // Main

