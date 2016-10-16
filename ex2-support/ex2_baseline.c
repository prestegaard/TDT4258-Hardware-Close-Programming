#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

#include "dac.h"
#include "gpio.h"
#include "timer.h"
#include "melodies.h"
#include "sound.h"

volatile uint8_t mode = 0;

//default volume is 1, differs from 0-15 (0.1 stepsize)
volatile double volume = 1;

void setupNVIC();

int main(void)
{
	/* Call the peripheral setup functions */

	setupGPIO();
	setupDAC();
	setupTimer(1000);

	/* Enable timer interrupt handling */
	setupNVIC();
	
	/* Start up melody */
	play_melody(C_chord);

	uint8_t buttons_old = 0;
	while (1) {
		uint8_t buttons = button_status();
		leds_output((buttons << 8));

		if (buttons == buttons_old) {
			//nothing
		} else {
			if (buttons == button1) { //Play C chord
				play_melody(C_chord);
			} 
			else if (buttons == button2) { //Turn volume up
				if (volume <= 15) {
					volume = volume + 0.1;
				}
				play_melody(beep);
			} 
			else if (buttons == button3) { //Play Lisa gikk til skolen
				play_melody(lisa_gikk_til_skolen);
			} 
			else if (buttons == button4) { //Turn volume down
				if (volume >= 0) {
					volume = volume - 0.1;
				}
				play_melody(beep);

			} 
			else if (buttons == button5) { //Play Star Wars main theme
				play_melody(star_wars_theme);

			} 
			else if (buttons == button6) { //Sweep up
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
		            buttons = button_status();
		         }
		         disableDAC();
		         stopTimer0();

			} 
			else if (buttons == button7) { //Crunsh sound
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
			else if (buttons == button8) { //Sweep down
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
				   buttons = button_status();
				}
				disableDAC();
				stopTimer0();
			}
		}
		buttons_old = buttons;
	}
	return 0;
}

void setupNVIC()
{

	*TIMER0_IEN = 1;
	*ISER0 |= ISER0_02;
	*ISER0 |= 0x802;

}
