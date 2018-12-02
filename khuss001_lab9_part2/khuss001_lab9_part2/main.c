/*
 * khuss001_lab9_part2.c
 *
 * Created: 8/28/2018 12:35:19 AM
 * Author : Keith
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

enum BLINK_States {START1, ON, OFF} blink_state;
enum LED_States {START2, L1, L2, L3} LED_state;
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
// Bit-access function

unsigned char tmp1 = 0x00;
unsigned char tmp2 = 0x00;

void Tick_1(){

	switch(blink_state){ // Transitions
		case START1:
		blink_state = OFF;
		break;
		
		case OFF:
		blink_state = ON;
		break;

		case ON:
		blink_state = OFF;
		break;
	}//Transitions

	switch(blink_state){ // state actions
		case START1:
		break;
		
		case OFF:
		tmp1 = 0x00;
		//	PORTB = tmp1;
		break;
		
		case ON:
		tmp1 = 0x01;
		//	PORTB = tmp1;
		break;
		
	}// State actions
}

void Tick_2(){

	switch(LED_state){ // Transitions
		case START2:
		LED_state = L1;
		break;
		
		case L1:
		LED_state = L2;
		break;
		
		case L2:
		LED_state = L3;
		break;
		
		case L3:
		LED_state = L1;
		break;
	}//Transitions

	switch(LED_state){ // state actions
		case START2:
		break;
		
		case L1:
		tmp2 = 0x02;
		//PORTB = tmp2;
		break;
		
		case L2:
		tmp2 = 0x04;
		//PORTB = tmp2;
		break;
		
		case L3:
		tmp2 = 0x08;
		//PORTB = tmp2;
		break;
	}// State actions
}

void Tick_3(){
	switch(c_state){
		case START3:
		c_state = COMBINE;
		break;
		
		case COMBINE:
		PORTB = tmp1 | tmp2;
		break;
	}
}

int main(void)
{
    unsigned long BL_elapsedTime = 1000;
    unsigned long TL_elapsedTime = 300;
    const unsigned long timerPeriod = 100;
	
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(timerPeriod);
	TimerOn();
	blink_state = START1;
	LED_state = START2;
	c_state = COMBINE;
	/* Replace with your application code */
	while (1)
	{
		if (BL_elapsedTime >= 1000){
			Tick_1();
			BL_elapsedTime = 0;
		}
		if (TL_elapsedTime >= 300){
			Tick_2();
			TL_elapsedTime = 0;
		}
		Tick_3();
		while (!TimerFlag){}
		TimerFlag = 0;
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
	}
}
