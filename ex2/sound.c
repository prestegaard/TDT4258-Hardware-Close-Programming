#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

#include "timer.h"
#include "dac.h"

void sine_set_frequency(uint32_t freq)
{
	uint32_t TimerTopValue;
	/* Get peripheral clock frequency */
	uint32_t hfperFreq = 14000000;

	/* Calculate new timer top value */
	TimerTopValue = (hfperFreq >> 6) / freq;

	/* If freq is too low, force fixed maximum top value */
  if (TimerTopValue > 0xffff) {
		TimerTopValue = 0xffff;
	}

	*TIMER0_TOP = TimerTopValue;
}

//melody[0][0] = [number of tones][tempo/length]
void play_melody(uint16_t melody[][2])
{
	startTimer0();
	double tempo = 0;
	if (melody[0][1] != 0) {	//tempo mode
		tempo = melody[0][1];
		tempo = 60 / tempo; //get number of second per beat
    tempo = tempo * 4; //tempo is given with quarter note as referance
		tempo = tempo * 1000; //convert to ms from second
	}
	for (uint8_t tone = 1; tone <= melody[0][0]; tone++) {
		if (melody[tone][0] == 0) { //If frequency is set to zero disable DAC 
			disableDAC();
		} else {
			enableDAC();
			sine_set_frequency(melody[tone][0]);

		}
		if (tempo) {
			uint16_t duration = (uint16_t) tempo / melody[tone][1]; 
			wait(duration);
		} else {
			wait(melody[tone][1]);
		}
	}
	stopTimer0();
	disableDAC();
}
