#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

#include "dac.h"
#include "gpio.h"
#include "timer.h"
#include "global_var.h"
#include "melodies.h"
#include "sound.h"

void setupNVIC();
/* Your code will start executing here */
volatile uint8_t mode = 0;
//default volume is 1, differs from 0-2 (0.1 stepsize)
volatile double volume = 1;
int main(void)
{
	/* Call the peripheral setup functions */

	setupGPIO();
	setupDAC();
	setupTimer(1000);

	/* Enable interrupt handling */
	setupNVIC();

   /* Start up melody */
	play_melody(C_chord);

   // Energy mode EM2
	*SCR = 6;		

	// Turn of RAM Block 1-3
	*EMU_MEMCTRL = 7;

	while (1) {
		mode = 0;
		*SCR |= 6;
		__asm("WFI");

		if (mode == 1) { //Play C chord
			play_melody(C_chord);
		} 
      else if (mode == 2) { //Turn volume up
			if (volume <= 15) {
				volume = volume + 0.1;
			}
         play_melody(beep);
		} 
      else if (mode == 3) { //Play Lisa gikk til skolen
			play_melody(lisa_gikk_til_skolen);
		}
      else if (mode == 4) { //Turn volume down
			if (volume >= 0) {
				volume = volume - 0.1;
			}
         play_melody(beep);
		} 
      else if (mode == 5) { //Play Star Wars main theme
			play_melody(star_wars_theme);

		} 
      else if (mode == 6) { //Sweep up
         enableDAC();
			startTimer0();
         uint8_t buttons = button_status();
         uint16_t freq=C3;
         while(buttons == button6){
            sine_set_frequency(freq);
            freq++;
            if(freq==C6)
               freq=C3;
            wait(5);
            buttons =button_status();
         }
         disableDAC();
         stopTimer0();
		} 
      else if (mode == 7) { //Crunsh sound
         enableDAC();
			for (int j = 0; j < 20; j++) {
				for (int i = 0; i < 5000; i++) {
					*DAC0_CH1DATA = i;
					*DAC0_CH0DATA = i;
				}
				for (int i = 0; i < 5000; i++) {
					*DAC0_CH1DATA = 5000 - i;
					*DAC0_CH0DATA = 5000 - i;
				}
			}
         disableDAC();
		} 
      else if (mode == 8) { //Sweep down
         enableDAC();
         startTimer0();
         uint8_t buttons = button_status();
         uint16_t freq=C5;
         while(buttons == button8){
            sine_set_frequency(freq);
            freq--;
            if(freq==0)
               freq=C5;
            wait(5);
            buttons =button_status();
         }
         disableDAC();
         stopTimer0();
		}

	}

	return 0;
}

void setupNVIC()
{

	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xFF;
	*GPIO_EXTIRISE = 0xFF;
	*GPIO_IEN = 0xFF;

	*TIMER0_IEN = 1;
	// *TIMER1_IEN = 1;

	*ISER0 |= ISER0_02;
	//*ISER0 |= ISER0_12;
	*ISER0 |= 0x802;

}
