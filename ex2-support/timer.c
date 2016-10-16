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

void startTimer0()
{
	*TIMER0_CMD = 1;
}

void stopTimer0()
{
	*TIMER0_CMD = 2;
}


void startTimer1()
{
	*TIMER1_CMD = 1;
}

void stopTimer1()
{
	*TIMER1_CMD = 2;
}

void wait(uint16_t delay_ms)
{
	*TIMER1_TOP = 14 * delay_ms; //Timer1 is prescaled down 1024, must therefore multiply by 14 to get number of counts for ms
	*TIMER1_CNT = 0;
	startTimer1();
	uint8_t count_done = 0;
	while (!count_done) { //wait until Timer1 counts above top level (overflow)
		__asm("WFI");
		if (*TIMER1_IF &= (1 << 0)) { //Check that timer1 triggered the interrupt
			count_done = 1;
			*TIMER1_IFC = 1;	//clear interrupt   
		}
	}
	stopTimer1();
}
