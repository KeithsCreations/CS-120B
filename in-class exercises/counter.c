/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/14/2018 14:55:35 PST
*/

#include "rims.h"

unsigned short max = 9;
unsigned short min = 0;
unsigned short count = 0;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_RESET, SM1_ON, SM1_DECREASE, SM1_INCREASE } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_RESET;
         break;
         case SM1_RESET: 
         if (!(A0A1)) {
            SM1_State = SM1_ON;
         }
         else if (A0A1) {
            SM1_State = SM1_RESET;
         }
         break;
      case SM1_ON: 
         if (!(A0 && A1)) {
            SM1_State = SM1_ON;
         }
         else if ((A0 && A1)) {
            SM1_State = SM1_RESET;
         }
         else if (((A0 && !A1) && count < max)) {
            SM1_State = SM1_INCREASE;
         }
         else if (((!A0 && A1) && count > min)) {
            SM1_State = SM1_DECREASE;
         }
         break;
      case SM1_DECREASE: 
         if (!(A0 && A1)) {
            SM1_State = SM1_ON;
            count--;
         }
         else if ((!A0 && A1)) {
            SM1_State = SM1_DECREASE;
         }
         else if (1) {
            SM1_State = SM1_RESET;
         }
         break;
      case SM1_INCREASE: 
         if (!(A0 && A1)) {
            SM1_State = SM1_ON;
            count++;
         }
         else if ((A0 && !A1)) {
            SM1_State = SM1_INCREASE;
         }
         else if ((A0 && A1)) {
            SM1_State = SM1_RESET;
         }
         break;
      default:
         SM1_State = SM1_RESET;
   } // Transitions

   switch(SM1_State) { // State actions
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
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 500;
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = ; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
   } // while (1)
} // Main