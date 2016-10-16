#ifndef MELODIES_H
#define MELODIES_H
#include "resource.h"

#include "freqs.h"
uint16_t C_chord[9][2] = {
  {8,  60},
  {C4, 2},
  {C4, 4},
  {G4, 4},
  {C5, 4},
  {C4, 8},
  {E4, 8},
  {G4, 8},
  {C5, 8}
};




uint16_t lisa_gikk_til_skolen[20][2] = {
	{19, 0},
	{C3, 400},
	{D3, 400},
	{E3, 400},
	{F3, 400},
	{G3, 350},
	{0,  500},
	{G3, 400},
	{A3, 150},
	{0, 500},
	{A3, 150},
	{0, 500},
	{A3, 150},
	{0, 500},
	{A3, 150},
	{0, 50},
	{G3, 150},
	{0, 50},
	{G3, 150},
	{0, 50}
};
//first element is used for setup, length, possible tempo
//melody[0][0] = [number of tones][tempo?]

#endif /* MELODIES_H */