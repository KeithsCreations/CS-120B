/*
 * khuss001_project_pong.c
 *
 * Created: 8/30/2018 2:22:41 AM
 * Author : Keith
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "io.h"

int rand();
unsigned short x_coord[]  = {13, 3, 4, 10, 6, 11, 15, 16};
unsigned short y_coord[] = {9, 14, 8, 12, 1, 7, 2, 5};
unsigned char ball_xd[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char ball_yd[] = {~0x01, ~0x02, ~0x04, ~0x08, ~0x10, ~0x20, ~0x40, ~0x80};

void display(unsigned char row, unsigned char col) {
	unsigned char bin_display[16];
	//row = ~row;
	for (int i = 0; i < 8; i++) {
		if((row >> i) & 0x01) {
			bin_display[x_coord[i]-1] = 0x01;
			} else {
			bin_display[x_coord[i]-1] = 0x00;
		}
	}
	for(int i = 0; i < 8; i++) {
		if((col >> i) & 0x01) {
				bin_display[y_coord[i]-1] = 0x01;
			} else {
				bin_display[y_coord[i]-1] = 0x00;
		}
	}

	for(int i = 16; i > 0; i--) {
		PORTB = bin_display[i-1];
		PORTB = bin_display[i-1] | 0x04;
	}
	PORTB = 0x02;
}
void render_p1(unsigned short index_location1){
	if (index_location1 == 0){
		display(0xE0, ball_yd[0]);
	}
	else if (index_location1 == 1){
		display(0x70, ball_yd[0]);
	}
	else if (index_location1 == 2){
		display(0x38, ball_yd[0]);
	}
	else if (index_location1 == 3){
		display(0x1C, ball_yd[0]);
	}
	else if (index_location1 == 4){
		display(0x0E, ball_yd[0]);
	}
	else if (index_location1 == 5){
		display(0x07, ball_yd[0]);
	}
}
void render_p2(unsigned short index_location2){
	if (index_location2 == 0){
		display(0xE0, ball_yd[7]);
	}
	else if (index_location2 == 1){
		display(0x70, ball_yd[7]);
	}
	else if (index_location2 == 2){
		display(0x38, ball_yd[7]);
	}
	else if (index_location2 == 3){
		display(0x1C, ball_yd[7]);
	}
	else if (index_location2 == 4){
		display(0x0E, ball_yd[7]);
	}
	else if (index_location2 == 5){
		display(0x07, ball_yd[7]);
	}
}

void set_1(){
	
	PORTD = 0x00; //16 (C8)
	PORTD = 0x04;
	
	PORTD = 0x00; //15 (C7)
	PORTD = 0x04;
	
	PORTD = 0x01; //14 (R2)
	PORTD = 0x05;
	
	PORTD = 0x00; //13 (C1)
	PORTD = 0x04;
	
	PORTD = 0x01; //12 (R4)
	PORTD = 0x05;
	
	PORTD = 0x00; //11 (C6)
	PORTD = 0x04;
	
	PORTD = 0x00; //10 (C4)
	PORTD = 0x04;
	
	PORTD = 0x01; //9 (R1)
	PORTD = 0x05;
	
	//PORTD = 0x02; //output	
}
void set_2(){
	
	PORTD = 0x01; //8 (R3)
	PORTD = 0x05;
		
	PORTD = 0x00; //7 (R6)
	PORTD = 0x04;
		
	PORTD = 0x00; //6 (C5)
	PORTD = 0x04;
		
	PORTD = 0x01; //5 (R8)
	PORTD = 0x05;
		
	PORTD = 0x01; //4 (C3)
	PORTD = 0x05;
		
	PORTD = 0x00; //3 (C2)
	PORTD = 0x04;
		
	PORTD = 0x01; //2 (R7)
	PORTD = 0x05;
		
	PORTD = 0x01; //1 (R5)
	PORTD = 0x05;
		
	PORTD = 0x02;
}

unsigned char flag = 0x00;
unsigned char flag_2 = 0x00;
unsigned char ball = 0x00;
unsigned char p1_inc = 0x00;
unsigned char p2_inc = 0x00;
unsigned short hold_ball = 0;
unsigned short bot_ball = 0;
unsigned char p1_b1 = 0x00;
unsigned char p1_b2 = 0x00;
unsigned char p2_b1 = 0x00;
unsigned char p2_b2 = 0x00;
unsigned char score_p1 = 0;
unsigned char score_p2 = 0;
unsigned char ball_x_index = 0;
unsigned char ball_y_index = 0;
unsigned short p1_index = 0;
unsigned short p2_index = 0;
unsigned int random_temp = 0;
unsigned char win = 0;

enum BALL {STARTx, WAITx, WAITx_release, inc_y, dec_y, 
										 inc_x_inc_y, inc_x_dec_y, 
										 dec_x_inc_y, dec_x_dec_y,
										 DONE} b_state;
enum PLAYER_1 {START1, WAIT1, up_p1, down_p1, p1_b_hold} p1_state; 
enum BOT {STARTbot, WAITbot, PLAYbot} bot_state;
enum PLAYER_2 {START2, HOLD2, WAIT2, up_p2, down_p2, p2_b_hold} p2_state;
enum OUTPUT {STARTo, output} o_state;
	
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
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void X_update(){
p1_b1 = ~PINA & 0x01;
p1_b2 = ~PINA & 0x02;

	switch(b_state){
		case STARTx:
		b_state =  WAITx;
		break;
		
		case WAITx:
		if (p1_b1){
			flag = 0x01;
			b_state = WAITx_release;
		}
		else if (p1_b2){
			flag = 0x02;
			b_state = WAITx_release;
		}
		break;
		
		case WAITx_release:
		if (!p1_b1 && !p1_b2){
			if (flag == 0x01){
				flag_2 = 0x01;
			}
			else {
				flag_2 = 0x02;
			}
			b_state = dec_y;
			LCD_ClearScreen();
			LCD_DisplayString(1, "P1: 0   P2: 0");
			ball_x_index = 4;
			ball_y_index = 4;
	//		display(ball_xd[ball_x_index], ball_yd[ball_y_index]);
		}
		break;
		
		case dec_y: // -y
		if ((score_p1 == 5) || (score_p2 == 5)){
			if (score_p1 == 5){
				win = 0x01;
				b_state = DONE;
			}
			else{
				win = 0x02;
				b_state = DONE;
			}
		}
		
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 1)){
			b_state = inc_y;
			ball_y_index++;
		}
		else if (ball_y_index == 1 && (ball_x_index == p1_index)){
			b_state = dec_x_inc_y;
			ball_y_index++;
			ball_x_index--;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 2)){
			b_state = inc_x_inc_y;
			ball_x_index++;
			ball_y_index++;
		}
		else if (ball_y_index == 0){
			if (rand() % 2 == 1){
				b_state = inc_y;
			}
			ball_x_index = 4;
			ball_y_index = 4;
			score_p2++;
			LCD_Cursor(13);
			LCD_WriteData(score_p2 + 0x30);
		}
		else{
			ball_y_index--;
		}
		
		hold_ball = 0;
		break;
		
		case inc_y: // +y
		if ((score_p1 == 5) || (score_p2 == 5)){
			if (score_p1 == 5){
				win = 0x01;
				b_state = DONE;
			}
			else{
				win = 0x02;
				b_state = DONE;
			}
		}
		if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 1)){
			b_state = dec_y;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index)){
			b_state = dec_x_dec_y;
			ball_x_index--;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 2)){
			b_state = inc_x_dec_y;
			ball_x_index++;
			ball_y_index--;
		}
		else if (ball_y_index == 7){
			if (rand() % 2 == 1){
				b_state = inc_y;
			}
			else {
				b_state = dec_y;
			}
			ball_x_index = 4;
			ball_y_index = 4;
			score_p1++;
			LCD_Cursor(5);
			LCD_WriteData(score_p1 + 0x30);
		}
		else{
			ball_y_index++;
		}
	//	display(ball_xd[ball_x_index], ball_yd[ball_y_index]);
		hold_ball = 0;
		break;
		
		
		case dec_x_inc_y: // -x, +y
		if (ball_x_index == 0){
			if ((ball_y_index == 6) && p2_index <= 1){
				b_state = inc_x_dec_y;
				ball_x_index++;
				ball_y_index--;
			}
			else if (ball_y_index == 7){
				if (rand() % 2 == 1){
					b_state = inc_y;
				}
				else{
					b_state = dec_y;
				}
				ball_y_index = 4;
				ball_x_index = 4;
				score_p1++;
			LCD_Cursor(5);
			LCD_WriteData(score_p1 + 0x30);
				break;
			}
			else{
				b_state = inc_x_inc_y;
				ball_x_index++;
				ball_y_index++;
			}
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 1)){
			b_state = dec_x_dec_y;
			ball_x_index--;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index)){
			b_state = dec_x_dec_y;
			ball_x_index--;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 2)){
			b_state = dec_x_dec_y;
			ball_x_index--;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 3)){
			b_state = inc_x_dec_y;
			ball_x_index++;
			ball_y_index--;
		}
		else if (ball_y_index == 7){
			if (rand() % 2 == 1){
				b_state = inc_y;
			}
			else {
				b_state = dec_y;
			}
			ball_x_index = 4;
			ball_y_index = 4;
			score_p1++;
			LCD_Cursor(5);
			LCD_WriteData(score_p1 + 0x30);
		}
		else {
			ball_x_index--;
			ball_y_index++;
		}
	//	display(ball_xd[ball_x_index], ball_yd[ball_y_index]);
		hold_ball = 0;
		break;
		
		
		case dec_x_dec_y: // -x, -y
		if (ball_x_index == 0){
			if ((ball_y_index == 1) && p1_index <= 1){
				b_state = inc_x_inc_y;
				ball_x_index++;
				ball_y_index++;
			}
			else if (ball_y_index == 0){
				if (rand() % 2 == 1){
					b_state = inc_y;
				}
				else{
					b_state = dec_y;
				}
				ball_y_index = 4;
				ball_x_index = 4;
				score_p2++;
			LCD_Cursor(13);
			LCD_WriteData(score_p2 + 0x30);
				break;
			}
			else{
				b_state = inc_x_dec_y;
				ball_x_index++;
				ball_y_index--;
			}
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 1)){
			b_state = dec_x_inc_y;
			ball_x_index--;
			ball_y_index++;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index)){
			b_state = dec_x_inc_y;
			ball_x_index--;
			ball_y_index++;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 2)){
			b_state = dec_x_inc_y;
			ball_x_index--;
			ball_y_index++;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 3)){
			b_state = inc_x_inc_y;
			ball_x_index++;
			ball_y_index++;
		}
		else if (ball_y_index == 0){
			if (rand() % 2 == 1){
				b_state = inc_y;
			}
			else {
				b_state = dec_y;
			}
			ball_x_index = 4;
			ball_y_index = 4;
			score_p2++;
			LCD_Cursor(13);
			LCD_WriteData(score_p2 + 0x30);
		}
		else {
			ball_x_index--;
			ball_y_index--;
		}
	//	display(ball_xd[ball_x_index], ball_yd[ball_y_index]);
		hold_ball = 0;
		break;
		
		case inc_x_inc_y: // +x, +y
		if (ball_x_index == 7){
			if ((ball_y_index == 6) && p2_index >= 4){
				b_state = dec_x_dec_y;
				ball_x_index--;
				ball_y_index--;
			}
			else if (ball_y_index == 7){
				if (rand() % 2 == 1){
					b_state = inc_y;
				}
				else{
					b_state = dec_y;
				}
				ball_y_index = 4;
				ball_x_index = 4;
				score_p1++;
			LCD_Cursor(5);
			LCD_WriteData(score_p1 + 0x30);
				break;
			}
			else{
				b_state = dec_x_inc_y;
				ball_x_index--;
				ball_y_index++;
			}
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 1)){
			b_state = inc_x_dec_y;
			ball_x_index++;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index)){
			b_state = inc_x_dec_y;
			ball_x_index++;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index - 1)){
			b_state = dec_x_dec_y;
			ball_x_index--;
			ball_y_index--;
		}
		else if ((ball_y_index == 6 ) && (ball_x_index == p2_index + 2)){
			b_state = inc_x_dec_y;
			ball_x_index++;
			ball_y_index--;
		}
		else if (ball_y_index == 7){
			if (rand() % 2 == 1){
				b_state = inc_y;
			}
			else {
				b_state = dec_y;
			}
			ball_x_index = 4;
			ball_y_index = 4;
			score_p1++;
			LCD_Cursor(5);
			LCD_WriteData(score_p1 + 0x30);
		}
		else {
			ball_x_index++;
			ball_y_index++;
		}
	//	display(ball_xd[ball_x_index], ball_yd[ball_y_index]);
		hold_ball = 0;
		break;
		
		case inc_x_dec_y: // +x, -y
		if (ball_x_index == 7){
			if ((ball_y_index == 1) && p1_index >= 4){
				b_state = dec_x_inc_y;
				ball_x_index--;
				ball_y_index++;
			}
			else if (ball_y_index == 0){
				if (rand() % 2 == 1){
					b_state = inc_y;
				}
				else{
					b_state = dec_y;
				}
				ball_y_index = 4;
				ball_x_index = 4;
				score_p2++;
			LCD_Cursor(13);
			LCD_WriteData(score_p2 + 0x30);
				break;
			}
			else{
				b_state = dec_x_dec_y;
				ball_x_index--;
				ball_y_index--;
			}
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 1)){
			b_state = inc_x_inc_y;
			ball_x_index++;
			ball_y_index++;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index)){
			b_state = inc_x_inc_y;
			ball_x_index++;
			ball_y_index++;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index - 1)){
			b_state = dec_x_inc_y;
			ball_x_index--;
			ball_y_index++;
		}
		else if ((ball_y_index == 1 ) && (ball_x_index == p1_index + 2)){
			b_state = inc_x_inc_y;
			ball_x_index++;
			ball_y_index++;
		}
		else if (ball_y_index == 0){
			if (rand() % 2 == 1){
				b_state = inc_y;
			}
			else {
				b_state = dec_y;
			}
			ball_x_index = 4;
			ball_y_index = 4;
			score_p2++;
			LCD_Cursor(13);
			LCD_WriteData(score_p2 + 0x30);
		}
		else {
			ball_x_index++;
			ball_y_index--;
		}
		hold_ball = 0;
		break;
		
		case DONE:
		if (win == 0x01){
			LCD_ClearScreen();
			LCD_DisplayString(1, "Player 1 wins!");
		}
		else {
			LCD_ClearScreen();
			LCD_DisplayString(1, "Player 2 wins!");
		}
		break;
}
	}
void Paddle_One(){
p1_b1 = ~PINA & 0x01;
p1_b2 = ~PINA & 0x02;

	switch(p1_state){
		case START1:
		p1_state = WAIT1;
		p1_index = 3;
		break;
		
		case WAIT1:
		if ((p1_b1 && !p1_b2) && (p1_index < 5)){
		//	p1_inc++;
			p1_index++;
			p1_state = p1_b_hold;
		}
		else if (!p1_b1 && p1_b2 && (p1_index > 0)){
			p1_index--;
			p1_state = p1_b_hold;
		}
	//	render_p1(p1_index);
		break;
		
		case up_p1:
		break;
		
		case down_p1:
		break;
		
		case p1_b_hold:
		if (!p1_b1 && !p1_b2){
			p1_state = WAIT1;
		}
	//	render_p1(p1_index);
		break;
	}
}
void Paddle_Bot(){
	switch(bot_state){
		case STARTbot:
		bot_state = WAITbot;
		break;
		
		case WAITbot:
		if (flag_2 == 1){
			bot_state = PLAYbot;
		}
		break;
		
		case PLAYbot:
		random_temp = rand();
		if(ball_x_index <= 2){
			if (((random_temp % 8) < 6) && (p2_index > 0)){
				p2_index--;
			}
			else if (((random_temp % 8) >= 6) && (p2_index < 5)){
				p2_index++;
			}
		}
// 		else if (ball_x_index == 3 || ball_x_index == 4){
// 			if ((random_temp % 3 == 0) && (p2_index > 0)){
// 				p2_index--;
// 			}
// 			else if ((random_temp % 3 == 2) && (p2_index < 5)){
// 				p2_index++;
// 			}
// 		}
		else if(ball_x_index >= 5){
			if (((random_temp % 8) >= 6) && (p2_index > 0)){
				p2_index--;
			}
			else if (((random_temp % 8) < 6) && (p2_index < 5)){
				p2_index++;
			}
		}
		break;
	}
}
void Paddle_Two(){
	p2_b1 = ~PINA & 0x04;
	p2_b2 = ~PINA & 0x08;

	switch(p2_state){
		case START2:
		p2_state = HOLD2;
		p2_index = 3;
		break;
		
		case HOLD2:
		if (flag_2 == 2){
			p2_state = WAIT2;
		}
		break;
		
		case WAIT2:
		if ((p2_b1 && !p2_b2) && (p2_index < 5)){
			//	p1_inc++;
			p2_index++;
			p2_state = p2_b_hold;
		}
		else if (!p2_b1 && p2_b2 && (p2_index > 0)){
			p2_index--;
			p2_state = p2_b_hold;
		}
	//	render_p2(p2_index);
		break;
		
		case up_p2:
		break;
		
		case down_p2:
		break;
		
		case p2_b_hold:
		if (!p2_b1 && !p2_b2){
			p2_state = WAIT2;
		}
	//	render_p2(p2_index);
		break;
	}
}
void show_all(){
	switch(o_state){
		case STARTo:
		o_state = output;
		break;
		
		case output:
		for (int q = 0; q < 50; q++){
		render_p2(p2_index);
		render_p1(p1_index);
		}
		display(ball_xd[ball_x_index], ball_yd[ball_y_index]);
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	hold_ball = 360;
	TimerSet(20);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	LCD_DisplayString(1, "AI Game: Buttn 12P Game: Buttn 2");
    
	//set_1();
	//set_2();
//	display(0x1C, ball_yd[7]);
    /* Replace with your application code */
    while (1) 
    {
 		if (hold_ball >= 360){
 			X_update();
 			hold_ball = 0;
		}
		if (bot_ball >= 280){
			Paddle_Bot();
			bot_ball = 0;
		}
		Paddle_One();
		Paddle_Two();
		show_all();
 		while (!TimerFlag);
 		TimerFlag = 0;
 		hold_ball += 20;
		bot_ball += 20;
}
}
