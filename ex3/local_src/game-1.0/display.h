#include <stdint.h>

#define DISPLAY_H 240
#define DISPLAY_W 320
#define DISPLAY_PIXELS 76800
#define DISPLAY_BYTES 153600

typedef struct Rectangle{
	uint16_t dx;		//start X
	uint8_t dy;			//start Y
	uint16_t width;		//stop X
	uint8_t height;		//stop y
}Rectangle;



typedef struct Color{
	uint8_t R;
	uint8_t G;
	uint8_t B;
}Color;

int init_display_fb();
int deinit_display_fb();
void update_display(Rectangle rectangle);
uint16_t mapRGB(uint8_t R, uint8_t G, uint8_t B);
void fill_background(uint8_t R, uint8_t G, uint8_t B);
void fill_rectangle(Rectangle rectangle, Color color);