#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "timer.h"

/* function to setup the timer */
void setupTimer(uint16_t period)
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
	*TIMER1_CTRL = TIMER1_PRESCALE;
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER0;
	*TIMER0_TOP = period;
	*TIMER1_TOP = period;
}


void startTimer0(){
	*TIMER0_CMD = 1;
}

void stopTimer0(){
	*TIMER0_CMD = 2;
}
void shutdownTimer0(){
	*CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER0;
}
void startTimer1(){
	*TIMER1_CMD = 1;
}

void stopTimer1(){
	*TIMER1_CMD = 2;
}
void shutdownTimer1(){
	*CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
}

void wait(uint16_t delay_ms){
   *TIMER1_TOP=14*delay_ms;
   *TIMER1_CNT = 0;
   startTimer1();
   uint8_t next_tone=0;
   while(!next_tone){
      __asm("WFI");
      if(*TIMER1_IF &= (1<<0) ){
         next_tone=1;
         *TIMER1_IFC = 1; //clear interrupt   
      }
   }
   stopTimer1();
}