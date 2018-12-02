/*
 * khuss001_lab3_part2.c
 *
 * Created: 8/13/2018 10:41:28 AM
 * Author : Keith
 */ 

#include <avr/io.h>

unsigned short max = 9;
unsigned short min = 0;
unsigned short count = 0;
enum States { START, SM1_RESET, SM1_ON, SM1_DECREASE, SM1_INCREASE } state;

void Tick(){

	switch(state){ // Transitions
      case START:
      state = SM1_RESET;
      break;
      case SM1_RESET:
      if (PINA != 0x03) {
	      state = SM1_ON;
      }
      else if (PINA = 0x03) {
	      state = SM1_RESET;
      }
      break;
      case SM1_ON:
      if (PINA == 0x00) {
	      state = SM1_ON;
      }
      else if (PINA == 0x03) {
	      state = SM1_RESET;
      }
      else if (((PINA == 0x01) && count < max)) {
	      state = SM1_INCREASE;
      }
      else if (((PINA == 0x02) && count > min)) {
	      state = SM1_DECREASE;
      }
      break;
      case SM1_DECREASE:
      if (PINA == 0x00) {
	      state = SM1_ON;
	      count--;
      }
      else if ((PINA == 0x02)) {
	      state = SM1_DECREASE;
      }
      else if (PINA == 0x03) {
	      state = SM1_RESET;
      }
      break;
      case SM1_INCREASE:
      if (PINA == 0x00) {
	      state = SM1_ON;
	      count++;
      }
      else if (PINA == 0x01) {
	      state = SM1_INCREASE;
      }
      else if (PINA == 0x03) {
	      state = SM1_RESET;
      }
      break;
      default:
      state = SM1_RESET;
	}//Transitions

	switch(state){ // state actions
      case SM1_RESET:
      count = 0;
      PORTC = count;
      break;
      case SM1_ON:
      PORTC = count;
      break;
      case SM1_DECREASE:
      break;
      case SM1_INCREASE:
      break;
      default: // ADD default behaviour below
      break;
	}// State actions
}

void main(){
	PORTC = 0x00; // Initialize outputs
	state = START; // Indicates initial call
	while(1) {Tick();}
}

