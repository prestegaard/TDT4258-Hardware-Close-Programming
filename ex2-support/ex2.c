#include <stdint.h>
#include <stdbool.h>


#include "efm32gg.h"

/*
  TODO calculate the appropriate sample period for the sound wave(s)
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   20	//	44100/14MHz = 317

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
uint16_t sine_generator(uint16_t frequency, uint16_t phase_offset, uint16_t number_of_samples);
/* Your code will start executing here */
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

	/* *SCR=6; //energy mode
	 __asm("WFI");
   */
  
   uint16_t length_ms=1000;
	while (1) {
  /*
      uint16_t frequency=4000;
      uint16_t number_of_samples=44100/frequency;
      uint16_t number_of_iterations=length_ms*frequency/1000;
      for(uint16_t phase_offset=0; phase_offset<number_of_samples;phase_offset++){
         uint16_t output=sine_generator(frequency, phase_offset, number_of_samples);
         *DAC0_CH0DATA=output;
         *DAC0_CH1DATA=output;
         //for(int i=0;i<1;i++){}
      }
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
	
   //*TIMER1_IEN = 1;
	//*ISER0 |= ISER0_12;

	*ISER0 |= ISER0_02;
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
