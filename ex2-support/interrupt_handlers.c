#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "resource.h"



void button_change(){
	uint8_t buttons = (*GPIO_PC_DIN & 0xFF);
	*GPIO_PA_DOUT=(buttons << 8);

	if(buttons == 0xFE){
		//sw1
		*GPIO_PA_DOUT = 0x0700;	/* turn on LEDs D4-D8 (LEDs are active low) */
		startTimer();
		//*TIMER1_CMD =1;
		return;
	} else if (buttons == 0xFD){
		//sw2
		stopTimer();

	} else if (buttons == 0xFB){
		for(int i = 0; i < 10000; i++){
			*DAC0_CH1DATA = i;
			*DAC0_CH0DATA = i;
		}
		*DAC0_CH0DATA =0;
		*DAC0_CH1DATA = 0;

		//sw3

	}/* else if (buttons == 0xF7){
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
	wait_flag = 0;
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
