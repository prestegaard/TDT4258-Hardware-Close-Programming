#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"




static unsigned int t=0;
static unsigned int freq[n] = {0};
static unsigned int length = 0;

void startMelody(){
	length = 441000/4;		//lag struct, og legg inn sanger osv?
	freq[0] = C6;					//sett på noen andre, funket ikke
	freq[1] = E6;
	freq[2] = G6;
	freq[3] = C7;
	//startTimer();
	*TIMER1_CMD = 1;
}

void button_change(){
	uint8_t buttons = (*GPIO_PC_DIN & 0xFF);
	*GPIO_PA_DOUT=(buttons << 8);

	if(buttons == 0xFE){
		//sw1
		return;
	}/* else if (buttons == 0xFD){
		//sw2

	} else if (buttons == 0xFB){
		//sw3

	} else if (buttons == 0xF7){
		//sw4

	} else if (buttons == 0xEF){
		//sw5

	} else if (buttons == 0xDF){
		//sw6

	} else if (buttons == 0xBF){
		//sw7

	} else if (buttons == 0x7F){
		//sw8

	} */else{
		return;
	}
//startTimer();

}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1; //clear interrupt
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
