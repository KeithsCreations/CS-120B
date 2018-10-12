/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/27/2018 17:15:12 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_s1, SM1_s2, SM1_s3 } SM1_State;

TickFct_ThreeLEDS() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_s1;
         break;
      case SM1_s1:
         if (1) {
            SM1_State = SM1_s2;
         }
         break;
      case SM1_s2:
         if (1) {
            SM1_State = SM1_s3;
         }
         break;
      case SM1_s3:
         if (1) {
            SM1_State = SM1_s1;
         }
         break;
      default:
         SM1_State = SM1_s1;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_s1:
         B5 = 1;
         B6 = 0;
         B7 = 0;
         break;
      case SM1_s2:
         B5 = 0;
         B6 = 1;
         B7 = 0;
         break;
      case SM1_s3:
         B5 = 0;
         B6 = 0;
         B7 = 1;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_OFF, SM2_ON } SM2_State;

TickFct_Blink() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_OFF;
         break;
      case SM2_OFF:
         if (1) {
            SM2_State = SM2_ON;
         }
         break;
      case SM2_ON:
         if (1) {
            SM2_State = SM2_OFF;
         }
         break;
      default:
         SM2_State = SM2_OFF;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_OFF:
         B0 = 0;
         break;
      case SM2_ON:
         B0 = 1;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(1000);
   TimerOn();
   SM1_State = -1;
   SM2_State = -1;
   while(1) {
      TickFct_ThreeLEDS();
      TickFct_Blink();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}