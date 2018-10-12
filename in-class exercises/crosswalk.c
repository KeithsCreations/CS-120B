/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/13/2018 14:28:46 PST
*/

#include "rims.h"

unsigned int i;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_OFF_RELEASE, SM1_ON_PRESS, SM1_ON_RELEASE, SM1_LIGHT, SM1_FLASHING } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_OFF_RELEASE;
         break;
         case SM1_OFF_RELEASE: 
         if (A0) {
            SM1_State = SM1_ON_PRESS;
            i = 0;
            
         }
         break;
      case SM1_ON_PRESS: 
         if (!A0) {
            SM1_State = SM1_ON_RELEASE;
            B1 = 1;
         }
         break;
      case SM1_ON_RELEASE: 
         if (i == 20) {
            SM1_State = SM1_LIGHT;
            B1 = 0;
            B0 = 1;
            i = 0;
         }
         else if (i <= 20) {
            SM1_State = SM1_ON_RELEASE;
         }
         break;
      case SM1_LIGHT: 
         if (i == 20) {
            SM1_State = SM1_FLASHING;
            i = 0;
         }
         else if (i <= 20) {
            SM1_State = SM1_LIGHT;
         }
         break;
      case SM1_FLASHING: 
         if (i == 12) {
            SM1_State = SM1_OFF_RELEASE;
         }
         else if (i <= 12) {
            SM1_State = SM1_FLASHING;
         }
         break;
      default:
         SM1_State = SM1_OFF_RELEASE;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_OFF_RELEASE:
         break;
      case SM1_ON_PRESS:
         break;
      case SM1_ON_RELEASE:
         i++;
         break;
      case SM1_LIGHT:
         i++;
         break;
      case SM1_FLASHING:
         if (i % 2 == 0){
         B0 = 1;}
         else {
         B0 = 0; }
         i++;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 500;
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