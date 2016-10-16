#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

void stopTimer0();
void stopTimer1();
void startTimer0();
void startTimer1();
void enableDAC();
void disableDAC();
void sine_set_frequency(uint32_t freq);

void wait_for_timer(uint16_t delay_ms){
   *TIMER1_TOP=14*delay_ms;
   *TIMER1_CNT = 0;
   *TIMER1_CMD = 1;
   uint8_t next_tone=0;
   while(!next_tone){
      __asm("WFI");
      if(*TIMER1_IF &= (1<<0) ){
         next_tone=1;
         *TIMER1_IFC = 1; //clear interrupt   
      }
   }
   *TIMER1_CMD = 2;
}

void play_tone(uint16_t tone, uint16_t duration){
  
  sine_set_frequency(tone);

  //timer_wait_ms(duration);
  //delay_ms(duration);
  //stopTimer0();
}



//melody[0][0] = [number of tones][tempo/length]
void play_melody(uint16_t melody[][2]){
  startTimer0();
  double tempo=0;
  if(melody[0][1]!=0){ //tempo mode
    tempo=melody[0][1];
    tempo = tempo*1000;
    tempo = tempo/60;
  }
  for(uint8_t tone=1; tone<=melody[0][0]; tone++){
    if(melody[tone][0]==0){
      disableDAC();
    }
    else{
      enableDAC();
      sine_set_frequency(melody[tone][0]);
      
    }
    if(tempo){
      uint16_t duration = (uint16_t)tempo/melody[tone][1];
      wait_for_timer(duration);
    }
    else{
      wait_for_timer(melody[tone][1]);
    }
  }
  stopTimer0();
}

/*
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
}*/
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
