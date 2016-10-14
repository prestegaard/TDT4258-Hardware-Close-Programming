#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "efm32gg.h"

#define PI  3.14159265



uint16_t sine_generator(uint16_t frequency, uint16_t phase_offset, uint16_t number_of_samples){
  double temp_sin = sin( 2 * PI * phase_offset / number_of_samples);
  if(temp_sin >= 0){
    return temp_sin*4095 + 4095;
  }
  else{
    return 4095 - temp_sin*4095;
  }
}
