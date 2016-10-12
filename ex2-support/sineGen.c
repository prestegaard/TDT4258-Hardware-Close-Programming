#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <efm32gg>

int _frequency = 0;
int _sample_rate = 0;
double PI = 3.14159265;


void set_freq_sr(frequency, sample_rate){
  _frequency = frequency;
  _sample_rate = sample_rate;
}
void sineBuffer(_frequency, _sample_rate){
  for(int i=0, i<_sample_rate, i++){
    buffer[i] = sin(_frequency * (2 * PI) * i / 44100);
  }
}
