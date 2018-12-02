/*
 * khuss001_lab7_part3.c
 *
 * Created: 8/25/2018 6:21:58 PM
 * Author : Keith
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char button = 0x00;
double notes[] = {392.00, 587.33, 466.16, 440.00, 392.00, 0, 466.16, 0, 440, 392, 
					0, 293.66, 0, 392.00, 587.33, 698.46, 659.25, 523.33, 0, 587.33,
					0, 349.23, 329.63, 261.63, 293.66};
double length[] = {888.88, 1200, 130, 85, 78, 15, 130, 115, 90, 1400, 
					15, 240, 15, 450, 2150, 100, 98, 92, 15, 700, 
					15, 95, 93, 87, 13, 700};
unsigned short i = 0;
unsigned short note_amount = 25;
unsigned short count = 0;

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
enum States { START, OFF, PLAY_1, PLAY_2, FINISH } state;
void Tick(){
button = ~PINC & 0x01;

	switch(state){ // Transitions
		case START:
		state = OFF;
		set_PWM(0);
		break;
		
		case OFF:
		if (button){
			state = PLAY_1;
		}
		break;
		
		case PLAY_1:
		if (count < length[i]){
			set_PWM(notes[i]);
		}
		else{
			if (i < note_amount){
				state = PLAY_2;
				count = 0;
				i++;
			}
			else {
				state = FINISH;
			}
		}
		break;
		
		case PLAY_2:
		if (count < length[i]){
			set_PWM(notes[i]);
		}
		else{
			if (i < note_amount){
				state = PLAY_1;
				count = 0;
				i++;
			}
			else {
				state = FINISH;
			}
		}
		break;
		
		case FINISH:
		if (!button){
			state = OFF;
			set_PWM(0);
			i = 0;
			count = 0;
		}
		break;
		
	}//Transitions

	switch(state){ // state actions
		case START:
		break;
		
		case OFF:
		break;
		
		case PLAY_1:
		count++;
		
		case PLAY_2:
		count++;
		
		case FINISH:
		break;
	}// State actions
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0x00; PORTC = 0xFF;
	PWM_on();
	TimerOn();
	TimerSet(2);
	state = START;
    while (1) 
    {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
}

