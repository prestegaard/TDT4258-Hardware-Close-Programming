#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;	/* enable DAC clock */
	*DAC0_CTRL = 0x50010; /* Set prescalar, CHECK OUT WHICH PRESCALER THIS IS */
	*DAC0_CH0CTRL = 1; /* enable left  audio channel */
	*DAC0_CH1CTRL = 1; /* enable right audio channel */

}

void resetDAC()
{
	*CMU_HFPERCLKEN0 &= ~(1 << 17);	/* disable DAC clock */
	*DAC0_CTRL = 0x00; /* Set prescalar, CHECK OUT WHICH PRESCALER THIS IS */
	*DAC0_CH0CTRL = 0; /* disable left  audio channel */
	*DAC0_CH1CTRL = 0; /* disable right audio channel */

}
void enableDAC(){
	*DAC0_CH0CTRL = 1; /* enable left  audio channel */
	*DAC0_CH1CTRL = 1; /* enable right audio channel */
}
void disableDAC(){
	*DAC0_CH0CTRL = 0; /* disable left  audio channel */
	*DAC0_CH1CTRL = 0; /* disable right audio channel */
}

