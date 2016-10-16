#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "resource.h"
#include "melodies.h"
#include "freqs.h"

void startTimer0();
void stopTimer0();
void shutdownTimer0();
void play_tone(uint16_t tone, uint16_t duration);
void play_melody();
void sine_set_frequency(uint32_t frequency);
void play_notes(uint16_t note0, uint16_t note1, uint16_t note2);


volatile uint8_t toggle_flag=0;
volatile uint16_t sample_number=0;

void button_change(){
	uint8_t buttons = (*GPIO_PC_DIN & 0xFF);
	*GPIO_PA_DOUT=(buttons << 8);

	if(buttons == 0xFE){
		//sw1
		*GPIO_PA_DOUT = 0x0700;	/* turn on LEDs D4-D8 (LEDs are active low) */
		startTimer0();
		//*TIMER1_CMD =1;
		return;
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
		play_notes(C4, E4, G4);
		//play_melody(C_chord);
		*DAC0_CH0DATA =0;
		*DAC0_CH1DATA = 0;

	} else if (buttons == 0xEF){
		//sw5
		sine_set_frequency(C5);

	} else if (buttons == 0xDF){
		//sw6
		sine_set_frequency(G5);

	} else if (buttons == 0xBF){
		//sw7
		play_tone(A3, 1000);
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

	*DAC0_CH0DATA=sinewave[sample_number];
  *DAC0_CH1DATA=sinewave[sample_number];
  sample_number+=4;
  if(sample_number>256){
  	sample_number=0;
  }
	
	*TIMER0_IFC = 1; //clear interrupt


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
