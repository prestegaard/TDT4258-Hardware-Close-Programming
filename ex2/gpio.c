#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
	
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0x0700;	/* turn on LEDs D4-D8 (LEDs are active low) */
	*GPIO_PC_MODEL = 0x33333333;	/* set pin 0-7 as input */
	*GPIO_PC_DOUT = 0xff;	/* enable pullup for inputs */

}
uint8_t button_status(){
	return (*GPIO_PC_DIN & 0xFF);
}
void leds_output(uint32_t leds){
	*GPIO_PA_DOUT=leds;
}	