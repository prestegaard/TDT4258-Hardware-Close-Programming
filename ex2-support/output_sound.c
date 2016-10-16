#include <stdint.h>
#include <stdbool.h>


#include "efm32gg.h"

void stopTimer0();
void stopTimer1();
void startTimer1();
void sine_set_frequency(uint32_t freq);
void delay_ms(uint32_t delaytime_ms){
  uint32_t number_of_ticks=delaytime_ms*1400;
  while(--number_of_ticks){
    // __asm("NOP");
  }
}

void timer_wait_ms(uint16_t msDelay) {
  /* adjustment factor for 14MHz oscillator, based on the timing of this whole function with speed optimization on, could probably be done in a prettier way. */
  uint16_t cycle_delay = msDelay * 14 - 28;
   /* Start TIMER0 */
  startTimer1();
  /* Wait until counter value is over top */
  while(*TIMER1_CNT < cycle_delay){
  /* Do nothing, just wait */
  }
  stopTimer1();
  *TIMER1_CNT=0;

}


void play_tone(uint16_t tone, uint16_t duration){
  
  sine_set_frequency(tone);

  //timer_wait_ms(duration);
  //delay_ms(duration);
  //stopTimer0();
}



//melody[0][0] = [number of tones][tempo/length]
void play_melody(uint16_t melody[][2]){
  for(uint8_t tone=1; tone<melody[0][0];){
    play_tone(melody[tone][0],melody[tone][1]);
    startTimer1();
    if(*TIMER1_CNT>=melody[tone][1]){
      tone++;
      *TIMER1_CNT=0;
    }
  }
  stopTimer0();
}
void play_notes(uint16_t note0, uint16_t note1, uint16_t note2){
  play_tone(note1, 0);
  startTimer1();
  while(*TIMER1_CNT<14000){
    *TIMER1_CNT=0;
    break;
  }
  play_tone(note1, 0);
  startTimer1();
  while(*TIMER1_CNT<14000){
    *TIMER1_CNT=0;
    break;
  }
  play_tone(note2, 0);
  startTimer1();
  while(*TIMER1_CNT<14000){
    *TIMER1_CNT=0;
    break;
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
