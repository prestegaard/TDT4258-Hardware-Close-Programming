#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

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
	*TIMER1_CNT = 0;
	*TIMER1_CMD = 2;
}
void shutdownTimer1(){
	*CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
}
/* function to setup the timer */
void setupTimer(uint16_t period)
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
	//*TIMER1_CTRL |= (1 << 25);
	//*TIMER1_CTRL |= (1 << 27);
	*TIMER1_CTRL = TIMER1_PRESCALE;
	//*TIMER1_TOP = period;	
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER0;
	*TIMER0_TOP = period;
}
	/*
	   TODO enable and set up the timer

	   1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
	   2. Write the period to register TIMER1_TOP
	   3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
	   4. Start the timer by writing 1 to TIMER1_CMD
	*/
