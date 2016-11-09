#include <stdint.h>

#define DISPLAY_H 240
#define DISPLAY_W 320
#define DISPLAY_PIXELS 76800
#define DISPLAY_BYTES 153600

int setupFB();
void drawRect(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height);
uint16_t mapRGB(uint8_t R, uint8_t G, uint8_t B);
void fillBackground(uint8_t R, uint8_t G, uint8_t B);
void fill_rectangle(uint16_t x_pos_left, uint16_t x_pos_right, 
		uint16_t y_pos_top, uint16_t y_pos_bottom, uint8_t R, uint8_t G, uint8_t B);