#include <stdbool.h>
#include <stdint.h>
#include "freqs.h"

//first element is used for setup, length, possible tempo
//melody[0][0] = [number of tones][tempo?]
uint16_t C_chord[5][2] = {
  {4,0},
  {C4, 1000},
  {E4, 1000},
  {G4, 500},
  {C5, 2000}
};
