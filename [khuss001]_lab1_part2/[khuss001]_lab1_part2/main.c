/*
 * [khuss001]_lab1_part2.c
 *
 * Created: 8/6/2018 4:14:00 PM
 * Author : Keith
 */ 

#include <avr/io.h>
int main(void)
{
DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
// initialize to 0s
unsigned char i = 0x00;
unsigned char  cntavail = 0x04; // Temporary variable to hold the value of B
for (i= 0x00; i < 0x03; i++)
{
// if PA0 is 1, set PB1PB0=01, else =10
if (PINA & (0x01 << i)) { // True if PA0 is 1
cntavail = cntavail - 1; // Sets tmpB to bbbbbb01
// (clear rightmost 2 bits, then set to 01)
}
}
cntavail = PORTC;
return 0;
}
