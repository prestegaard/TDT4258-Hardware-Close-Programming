#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "resource.h"
#include "melodies.h"
//#include "freqs.h"
/*
  TODO calculate the appropriate sample period for the sound wave(s)
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   317	//	44100/14MHz = 317

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void play_melody(uint16_t melody[][2]);
void sine_set_frequency(uint32_t freq);
void startTimer0();
void stopTimer0();
void wait_for_timer(uint16_t delay_ms);
/* Your code will start executing here */
volatile uint8_t mode=0;



int main(void)
{
   /* Call the peripheral setup functions */
   
   setupGPIO();
   setupDAC();
   setupTimer(SAMPLE_PERIOD);

	//startMelody();
	/* Enable interrupt handling */
	setupNVIC();

	/* TODO for higher energy efficiency, sleep while waiting for interrupts
	   instead of infinite loop for busy-waiting
	 */

	 //*SCR |= 6; //energy mode
   while (1) {
      __asm("WFI");
      if(mode==1){
         play_melody(C_chord);
        /*
         startTimer0();
         sine_set_frequency(A3);
         wait_for_timer(150);
         sine_set_frequency(C4);
         wait_for_timer(150);
         sine_set_frequency(E4);
         wait_for_timer(300);
         sine_set_frequency(A4);
         wait_for_timer(300);
         stopTimer0();
         */
         mode=0;
      }
      if(mode==4){
         play_melody(lisa_gikk_til_skolen);
         mode=0;
      }
      else if(mode ==0){
         *GPIO_PA_DOUT = 0x0700; 
      }
      /*
      
      */
   }
   

	return 0;
}

void setupNVIC()
{

	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xFF;
	*GPIO_EXTIRISE = 0xFF;
	*GPIO_IEN =0xFF;

   *TIMER0_IEN = 1;	
  // *TIMER1_IEN = 1;

   *ISER0 |= ISER0_02;
	//*ISER0 |= ISER0_12;
   *ISER0 |= 0x802;

	/* TODO use the NVIC ISERx registers to enable handling of interrupt(s)
	   remember two things are necessary for interrupt handling:
	   - the peripheral must generate an interrupt signal
	   - the NVIC must be configured to make the CPU handle the signal
	   You will need TIMER1, GPIO odd and GPIO even interrupt handling for this
	   assignment.
	 */
}

/* if other interrupt handlers are needed, use the following names:
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
