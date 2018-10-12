/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/13/2018 14:1:25 PST
*/

#include "rims.h"

unsigned char score;
unsigned char i;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_standby, SM1_GAME_PLAY, SM1_GAME_OVER } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_standby;
         break;
         case SM1_standby: 
         if (A0) {
            SM1_State = SM1_GAME_PLAY;
            i = 0;
            score = 0;
         }
         else if (!A0) {
            SM1_State = SM1_standby;
         }
         break;
      case SM1_GAME_PLAY: 
         if (i = 0) {
            SM1_State = SM1_GAME_OVER;
            i = 10;
         }
         else if (i <= 60) {
            SM1_State = SM1_GAME_PLAY;
            if (A1) score ++;
            else if (A2) score += 2;
            else if (A3) score += 3;
            else if (A4) score += 4;
            else if (A5) score += 5;
            else if (A6) score += 10;
            B = score;
         }
         break;
      case SM1_GAME_OVER: 
         if (i = 0) {
            SM1_State = SM1_standby;
         }
         else if (i > 0) {
            SM1_State = SM1_GAME_OVER;
            if (i % 2 == 0) B = score;
            else B = 0;
         }
         break;
      default:
         SM1_State = SM1_standby;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_standby:
         break;
      case SM1_GAME_PLAY:
         i++;
         break;
      case SM1_GAME_OVER:
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