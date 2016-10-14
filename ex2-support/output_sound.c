#include <stdint.h>
#include <stdbool.h>
#include "resource.h"

#include "efm32gg.h"


uint16_t sine_generator(uint16_t frequency, uint16_t phase_offset, uint16_t number_of_samples);
void startTimer();
volatile uint8_t wait_flag=0;
void wait_for_timer(){
    wait_flag=1;
    *GPIO_PA_DOUT = 0x0700;
    for(int i=0;i<317;i++){
      *GPIO_PA_DOUT = 0xFF;
    }

}

void play_sound( uint16_t frequency, uint16_t length){
  uint16_t number_of_samples=44100/frequency;
  uint16_t number_of_iterations=length*frequency/1000; //Length in milisecond
  for(uint16_t i=0; i< number_of_iterations;i++){
    for(uint16_t phase_offset=0; phase_offset<number_of_samples;phase_offset++){
      uint16_t output=sine_generator(frequency, phase_offset, number_of_samples);
      wait_for_timer();
      *DAC0_CH0DATA=output;
      *DAC0_CH1DATA=output;
    }
  }
}
//melody[0][0] = [number of tones][tempo?]
void play_melody(uint16_t melody[][2]){
  for(uint8_t tone=1; tone<melody[0][0]; tone++){
    play_sound(melody[tone][0],melody[tone][1]);
  }
}




/*
void playsound(char* waveform, uint16_t frequency){
  samplethis
}

struct Start(Tone, length){
  C,1000
  D,1000
  F,1000
}

void play(start){
  playsound(sineBuffer, start.Tone)
}
*/
