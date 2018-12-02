/*
 * khuss001_lab9_part3.c
 *
 * Created: 8/28/2018 12:46:38 AM
 * Author : Keith
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned short max = 9;
unsigned short min = 0;
unsigned char count = 0;
unsigned short hold = 0;
unsigned char button_1 = 0x00; //variable for PORTA
unsigned char button_2 = 0x00; //variable for PORTA

enum UP_states { START1, SM1_RESET, SM1_ON, SM1_INCREASE_SLOW, SM1_INCREASE_FAST } up_state;
enum DOWN_states { START2, SM2_RESET, SM2_ON, SM2_DECREASE_SLOW, SM2_DECREASE_FAST } down_state;
enum Combine_states {START3, COMBINE} c_state;

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer
//should clear to 0.
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s
	// AVR output compare register OCR1A.
	OCR1A = 125; // Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt
	//Initialize avr counter
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}
void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}
void TimerISR() {
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void Tick1(){
button_1 = ~PINC & 0x01;
button_2 = ~PINC & 0x02;

	switch(up_state){ // Transitions
		case START1:
		up_state = SM1_RESET;
		break;
		case SM1_RESET:
		if (!button_1 && !button_2) {
			up_state = SM1_ON;
		}
		else if (button_1 && button_2) {
			up_state = SM1_RESET;
			hold = 0;
			count = 0;
		}
		break;
		case SM1_ON:
		if (!button_1 && !button_2) {
			up_state = SM1_ON;
		}
		else if (button_1 && button_2) {
			up_state = SM1_RESET;
			hold = 0;
			count = 0;
		}
		else if (((button_1 && !button_2) && count < max)) {
			up_state = SM1_INCREASE_SLOW;
		}
		break;
		case SM1_INCREASE_SLOW:
		if (!button_1 && !button_2) {
			up_state = SM1_ON;
			if (count < 9){ count++;}
			hold = 0;
		}
		else if (button_1 && !button_2) {
			up_state = SM1_INCREASE_SLOW;
			hold++;
			if (hold % 5 == 0){
				if (count < 9){ 
					count++;
				}
			}
			if (hold == 15){
				up_state = SM1_INCREASE_FAST;
				hold = 0;
			}
		}
		else if (button_1 && button_2) {
			up_state = SM1_RESET;
			hold = 0;
			count = 0;
		}
		break;
		
		case SM1_INCREASE_FAST:
		if (!button_1 && !button_2) {
			up_state = SM1_ON;
		}
		else if (button_1 && !button_2){
			up_state = SM1_INCREASE_FAST;
			hold++;
			if (hold % 2 == 0){
				if (count < 9){
					count++;
				}
			}
		}
		else if (button_1 && button_2) {
			up_state = SM1_RESET;
		}
		break;
		default:
		up_state = SM1_RESET;
		hold = 0;
		count = 0;
	}//Transitions

	switch(up_state){ // state actions
		case START1:
		break;
		case SM1_RESET:
		PORTD = count;
		break;
		case SM1_ON:
		PORTD = count;
		break;
		case SM1_INCREASE_SLOW:
		break;
		case SM1_INCREASE_FAST:
		break;
		default: // ADD default behaviour below
		break;
	}// State actions
}
void Tick2(){
	button_1 = ~PINC & 0x01;
	button_2 = ~PINC & 0x02;

	switch(down_state){ // Transitions
		case START2:
		down_state = SM2_RESET;
		break;
		case SM2_RESET:
		if (!button_1 && !button_2) {
			down_state = SM2_ON;
		}
		else if (button_1 && button_2) {
			down_state = SM2_RESET;
		}
		break;
		case SM1_ON:
		if (!button_1 && !button_2) {
			down_state = SM2_ON;
		}
		else if (button_1 && button_2) {
			down_state = SM2_RESET;
			hold = 0;
			count = 0;
		}
		else if (!button_1 && button_2) {
			down_state = SM2_DECREASE_SLOW;
		}
		break;
		case SM2_DECREASE_SLOW:
		if (!button_1 && !button_2) {
			down_state = SM2_ON;
			if (count > 0){ count--;}
			hold = 0;
		}
		else if (!button_1 && button_2) {
			down_state = SM2_DECREASE_SLOW;
			hold++;
			if (hold % 5 == 0){
				if (count > 0){
					count--;
				}
			}
			if (hold == 15){
				down_state = SM2_DECREASE_FAST;
				hold = 0;
			}
		}
		else if (button_1 && button_2) {
			down_state = SM2_RESET;
			hold = 0;
			count = 0;
		}
		break;
		
		case SM2_DECREASE_FAST:
		if (!button_1 && !button_2) {
			down_state = SM2_ON;
		}
		else if (!button_1 && button_2){
			down_state = SM2_DECREASE_FAST;
			hold++;
			if (hold % 2 == 0){
				if (count > 0){
					count--;
				}
			}
		}
		else if (button_1 && button_2) {
			down_state = SM2_RESET;
			hold = 0;
			count = 0;
		}
		break;
		default:
		down_state = SM2_RESET;
		hold = 0;
		count = 0;
	}//Transitions

	switch(up_state){ // state actions
		case START2:
		break;
		case SM2_RESET:
		break;
		case SM2_ON:
		break;
		case SM2_DECREASE_SLOW:
		break;
		case SM2_DECREASE_FAST:
		break;
		default: // ADD default behaviour below
		break;
	}// State actions
}
void Tick3(){
	switch(c_state){
		case START3:
		c_state = COMBINE;
		break;
		
		case COMBINE:
		PORTB = count;
		break;
	}
}

void main(){
	DDRB = 0xFF; PORTB = 0x00; // Initialize outputs
	DDRC = 0x00; PORTC = 0xFF;
	TimerSet(200);
	TimerOn();
	up_state = START1; // Indicates initial call
	down_state = START2;
	c_state = START3;

	while(1) {
				Tick1();
				Tick2();
				Tick3();
				while (!TimerFlag){}
				TimerFlag = 0;	
	}
}
