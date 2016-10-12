#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	/*
	   TODO enable and set up the Digital-Analog Converter

	   1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
	   2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL
	   3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL
	      and DAC0_CH1CTRL
	   4. Write a continuous stream of samples to the DAC data registers,
	   	  DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
	 */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;	/* enable DAC clock */
	*DAC0_CTRL = 0x50010; /* Set prescalar, CHECK OUT WHICH PRESCALER THIS IS */
	*DAC0_CH0CTRL = 1; /* enable left  audio channel */
	*DAC0_CH1CTRL = 1; /* enable right audio channel */

}

void resetDAC()
{
	*CMU_HFPERCLKEN0 &= ~(1 << 17);	/* enable DAC clock */
	*DAC0_CTRL = 0x00; /* Set prescalar, CHECK OUT WHICH PRESCALER THIS IS */
	*DAC0_CH0CTRL = 0; /* disable left  audio channel */
	*DAC0_CH1CTRL = 0; /* disable right audio channel */

}
