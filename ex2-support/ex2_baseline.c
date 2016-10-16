#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

#include "dac.h"
#include "gpio.h"
#include "timer.h"
#include "melodies.h"
#include "sound.h"




volatile uint8_t mode=0;

//default volume is 1, differs from 0-15 (0.1 stepsize)
volatile double volume=1;

void setupNVIC();

int main(void)
{
   /* Call the peripheral setup functions */
   
   setupGPIO();
   setupDAC();
   setupTimer(1000);


	/* Enable timer interrupt handling */
	setupNVIC();
   play_melody(C_chord);

   uint8_t buttons_old=0;
   while (1) {
      uint8_t buttons = (*GPIO_PC_DIN & 0xFF);
      *GPIO_PA_DOUT=(buttons << 8);
      if(buttons==buttons_old){
         //nothing
      }
      else{
         if(buttons == 0xFE){ //sw1
           play_melody(C_chord);
         } 
         else if (buttons == 0xFD){ //sw2
            if(volume<=15){
               volume=volume+0.1;
            }
         } 
         else if (buttons == 0xFB){ //sw3
            play_melody(lisa_gikk_til_skolen);
         } 
         else if (buttons == 0xF7){ //sw4
            if(volume>=0){
               volume=volume-0.1;
            }

         } 
         else if (buttons == 0xEF){ //sw5
            play_melody(star_wars_theme);

         } 
         else if (buttons == 0xDF){ //sw6
            startTimer0();
            sine_set_frequency(A3);

         } 
         else if (buttons == 0xBF){ //sw7
            for(int j=0; j<20;j++){ 
               for(int i = 0; i < 5000; i++){
                  *DAC0_CH1DATA = i;
                  *DAC0_CH0DATA = i;
               }
               for(int i = 0; i < 5000; i++){
                  *DAC0_CH1DATA = 5000-i;
                  *DAC0_CH0DATA = 5000-i;
               }
            }
            *DAC0_CH0DATA =0;
            *DAC0_CH1DATA = 0;
         }
         else if (buttons == 0x7F){ //sw8
         }
      }
      buttons_old=buttons;
   }
	return 0;
}

void setupNVIC()
{

   *TIMER0_IEN = 1;	
   *ISER0 |= ISER0_02;
   *ISER0 |= 0x802;


}

