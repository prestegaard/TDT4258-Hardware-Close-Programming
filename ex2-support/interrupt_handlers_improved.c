#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"


#include "timer.h"
#include "global_var.h"




const uint8_t sinewave[] = /* 256 values */
{
  0x80, 0x83, 0x86, 0x89, 0x8c, 0x8f, 0x92, 0x95, 0x98, 0x9c, 0x9f, 0xa2, 0xa5, 0xa8, 0xab, 0xae,
  0xb0, 0xb3, 0xb6, 0xb9, 0xbc, 0xbf, 0xc1, 0xc4, 0xc7, 0xc9, 0xcc, 0xce, 0xd1, 0xd3, 0xd5, 0xd8,
  0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xed, 0xef, 0xf0, 0xf2, 0xf3, 0xf5,
  0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfc, 0xfd, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfc, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7,
  0xf6, 0xf5, 0xf3, 0xf2, 0xf0, 0xef, 0xed, 0xec, 0xea, 0xe8, 0xe6, 0xe4, 0xe2, 0xe0, 0xde, 0xdc,
  0xda, 0xd8, 0xd5, 0xd3, 0xd1, 0xce, 0xcc, 0xc9, 0xc7, 0xc4, 0xc1, 0xbf, 0xbc, 0xb9, 0xb6, 0xb3,
  0xb0, 0xae, 0xab, 0xa8, 0xa5, 0xa2, 0x9f, 0x9c, 0x98, 0x95, 0x92, 0x8f, 0x8c, 0x89, 0x86, 0x83,
  0x80, 0x7c, 0x79, 0x76, 0x73, 0x70, 0x6d, 0x6a, 0x67, 0x63, 0x60, 0x5d, 0x5a, 0x57, 0x54, 0x51,
  0x4f, 0x4c, 0x49, 0x46, 0x43, 0x40, 0x3e, 0x3b, 0x38, 0x36, 0x33, 0x31, 0x2e, 0x2c, 0x2a, 0x27,
  0x25, 0x23, 0x21, 0x1f, 0x1d, 0x1b, 0x19, 0x17, 0x15, 0x13, 0x12, 0x10, 0x0f, 0x0d, 0x0c, 0x0a,
  0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x09, 0x0a, 0x0c, 0x0d, 0x0f, 0x10, 0x12, 0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f, 0x21, 0x23,
  0x25, 0x27, 0x2a, 0x2c, 0x2e, 0x31, 0x33, 0x36, 0x38, 0x3b, 0x3e, 0x40, 0x43, 0x46, 0x49, 0x4c,
  0x4f, 0x51, 0x54, 0x57, 0x5a, 0x5d, 0x60, 0x63, 0x67, 0x6a, 0x6d, 0x70, 0x73, 0x76, 0x79, 0x7c
};

volatile uint8_t toggle_flag=0;
volatile uint16_t sample_number=0;

void button_change(){
	uint8_t buttons = (*GPIO_PC_DIN & 0xFF);
	*GPIO_PA_DOUT=(buttons << 8);

	if(buttons == 0xFE){
		//sw1
		mode=1;
		//*SCR &= ~(1<<1);
		//*SCR &= ~(1<<2);
		//*TIMER1_CMD =1;
	} else if (buttons == 0xFD){
		//sw2
		//shutdownTimer();
		stopTimer0();

	} else if (buttons == 0xFB){
		for(int j=0; j<10;j++){	
			for(int i = 0; i < 10000; i++){
				*DAC0_CH1DATA = i;
				*DAC0_CH0DATA = i;
			}
			for(int i = 0; i < 10000; i++){
				*DAC0_CH1DATA = 10000-i;
				*DAC0_CH0DATA = 10000-i;
			}
		}
		*DAC0_CH0DATA =0;
		*DAC0_CH1DATA = 0;

		//sw3

	} else if (buttons == 0xF7){
		//sw4
		mode=4;

	} else if (buttons == 0xEF){
		//sw5
		sine_set_frequency(C5);

	} else if (buttons == 0xDF){
		//sw6
		sine_set_frequency(G5);

	} else if (buttons == 0xBF){
		//sw7
		play_tone(A3, 1000);
		*SCR=6;
		//stopTimer();

	}/* else if (buttons == 0x7F){
		//sw8

	} */else{
		return;
	}
//startTimer();

}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER0_IRQHandler()
{
	*DAC0_CH0DATA= (uint16_t) sinewave[sample_number]*volume;
  *DAC0_CH1DATA= (uint16_t) sinewave[sample_number]*volume;
  sample_number+=4;
  if(sample_number>256){
  	sample_number=0;
  }
	
	*TIMER0_IFC = 1; //clear interrupt


}
void __attribute__ ((interrupt)) TIMER1_IRQHandler(){
	__asm("NOP");
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
