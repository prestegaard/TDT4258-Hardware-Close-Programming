#include <stdint.h>


int setupFB();
void drawRect(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height);
uint16_t mapRGB(uint8_t R, uint8_t G, uint8_t B);
void fillBackground(uint8_t R, uint8_t G, uint8_t B);