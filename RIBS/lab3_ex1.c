/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/11/2018 21:9:50 PST
*/

#include "rims.h"

unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_OFF_RELEASE, SM1_ON_PRESS, SM1_ON_RELEASE, SM1_OFF_PRESS } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         PB0 = 1;
         PB1 = 0;
         SM1_State = SM1_OFF_RELEASE;
         break;
         case SM1_OFF_RELEASE: 
         if (PA0) {
            SM1_State = SM1_ON_PRESS;
         }
         else if (!PA0) {
            SM1_State = SM1_OFF_RELEASE;
         }
         break;
      case SM1_ON_PRESS: 
         if (!PA0) {
            SM1_State = SM1_ON_RELEASE;
         }
         else if (PA0) {
            SM1_State = SM1_ON_PRESS;
         }
         break;
      case SM1_ON_RELEASE: 
         if (PA0) {
            SM1_State = SM1_OFF_PRESS;
         }
         else if (!PA0) {
            SM1_State = SM1_ON_RELEASE;
         }
         break;
      case SM1_OFF_PRESS: 
         if (!PA0) {
            SM1_State = SM1_OFF_RELEASE;
         }
         else if (PA0) {
            SM1_State = SM1_OFF_PRESS;
         }
         break;
      default:
         SM1_State = SM1_OFF_RELEASE;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_OFF_RELEASE:
         break;
      case SM1_ON_PRESS:
         PB0 = 0;
         PB1 = 1;
         break;
      case SM1_ON_RELEASE:
         break;
      case SM1_OFF_PRESS:
         PB0 = 1;
         PB1 = 0;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
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