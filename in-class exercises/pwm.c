/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/14/2018 16:2:36 PST
*/

#include "rims.h"

unsigned short count = 0;

unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_ON, SM1_OFF } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_ON;
         break;
         case SM1_ON: 
         if (count == 6) {
            SM1_State = SM1_OFF;
            count = 0;
         }
         else if (count < 6) {
            SM1_State = SM1_ON;
            B0 = 1;
         }
         break;
      case SM1_OFF: 
         if (count == 34) {
            SM1_State = SM1_ON;
            count = 0;
         }
         else if (count < 34) {
            SM1_State = SM1_OFF;
            B0 = 0;
         }
         break;
      default:
         SM1_State = SM1_ON;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_ON:
         count++;
         break;
      case SM1_OFF:
         count++;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 25;
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main