#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "efm32gg.h"

#define PI  3.14159265



int16_t sine_generator(uint16_t frequency, uint16_t phase_offset, uint16_t number_of_samples){
  double temp_sin = sin( 2 * PI * phase_offset / number_of_samples);
  //temp_sin=temp_sin + 1;
  return temp_sin*4095;
}

void sine_set_frequency(uint32_t freq){
	uint32_t TimerTopValue;
	/* Get peripheral clock frequency */
	uint32_t hfperFreq = 14000000;

	/* Calculate new timer top value */
	TimerTopValue = (hfperFreq >> 6) / freq;

	if (TimerTopValue > 0xffff)
	{
		TimerTopValue = 0xffff;
	}

	*TIMER0_TOP = TimerTopValue;
}
