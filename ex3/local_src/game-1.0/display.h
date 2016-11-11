#include <stdint.h>

#define DISPLAY_H 240
#define DISPLAY_W 320
#define DISPLAY_PIXELS 76800
#define DISPLAY_BYTES 153600

typedef struct Rectangle{
	uint16_t dx;			//start X
	uint8_t dy;			//Start Y
	uint16_t width;		//Bredde X
	uint8_t height;		//høyde y
	//uint8_t color;
}Rectangle;



typedef struct Color{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}Color;

int setupFB();
void updateDisplay(Rectangle rectangle);
uint16_t mapRGB(uint8_t R, uint8_t G, uint8_t B);
void fillBackground(uint8_t R, uint8_t G, uint8_t B);
void fill_rectangle(Rectangle rectangle, Color color);