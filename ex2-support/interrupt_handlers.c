#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void button_change(){

	*GPIO_PA_DOUT=(*GPIO_PC_DIN << 8);
}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */


	 /*
	 *GPIO_EXTIPSELL = 0x22222222;
	 *GPIO_EXTIFALL = 0xFF;
	 *GPIO_EXTIRISE = 0xFF;
	 */
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = *GPIO_IF;
	/* TODO handle button pressed event, remember to clear pending interrupt */
	button_change();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = *GPIO_IF;
	/* TODO handle button pressed event, remember to clear pending interrupt */
	button_change();
}
