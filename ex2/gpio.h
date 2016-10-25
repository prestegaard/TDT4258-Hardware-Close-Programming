#ifndef GPIO_H
#define GPIO_H

#define button1 0xFE
#define button2 0xFD
#define button3 0xFB
#define button4 0xF7
#define button5 0xEF
#define button6 0xDF
#define button7 0xBF
#define button8 0x7F


void setupGPIO();
uint8_t button_status();
void leds_output(uint32_t leds);
#endif				/* GPIO_H */
