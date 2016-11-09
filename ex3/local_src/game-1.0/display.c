#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include "display.h"



uint16_t *fbmmap;
int fbfd;
struct fb_copyarea rect;

struct gameBoard
{
	int x;
	int y;
	int width;
	int height;
	uint16_t *pixelarray;
};


int setupFB(){
	char remove_cursor_blink[50];
	strcpy( remove_cursor_blink, "echo 0 > /sys/class/graphics/fbcon/cursor_blink");
	system(remove_cursor_blink);

	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0){
		printf("error opening Framebuffer\n");
	}
	printf("Framebuffer opened\n");
	fbmmap = (uint16_t*)mmap(NULL,320*240*2,PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbmmap == MAP_FAILED) {
		printf("fbmmap failed\n");
		return EXIT_FAILURE;
	} 
	return EXIT_SUCCESS;
}

uint16_t mapRGB(uint8_t R, uint8_t G, uint8_t B){
	// R max 5 bit => 0-31
	// G max 6 bit => 0-63
	// B max 5 Bit => 0-31
	uint16_t mapped_RGB = 0;
	if( R >= 31 ){
		R = 31;
	}
	if(G >= 63){
		G = 63;
	}
	if( B >= 31){
		B= 31;
	}
	mapped_RGB |= (R << 11);
	mapped_RGB |= (G << 5);
	mapped_RGB |= (B << 0);

	return mapped_RGB;
}

void drawRect(uint16_t dx,uint16_t dy,uint16_t width,uint16_t height){
	rect.dx = dx;
	rect.dy = dy;
	rect.width = width;
	rect.height = height;
	ioctl(fbfd, 0x4680, &rect); //update display
	printf("display is updated\n");
}


void fillBackground(uint8_t R, uint8_t G, uint8_t B){
	int pixels;
	uint16_t color = mapRGB(R,G,B);

	for (pixels = 0; pixels < DISPLAY_PIXELS; pixels++){
		fbmmap[pixels] = color;
	}

	drawRect(0,0, DISPLAY_W, DISPLAY_H);

}

void fill_rectangle(uint16_t x_pos_left, uint16_t x_pos_right, 
		uint16_t y_pos_top, uint16_t y_pos_bottom, uint8_t R, uint8_t G, uint8_t B){
	uint16_t y;
	uint16_t x;
	uint16_t color = mapRGB(R,G,B);
	for(y=y_pos_top; y<=y_pos_bottom; y++){
		for(x=x_pos_left; x <= x_pos_right; x++){
			fbmmap[y*DISPLAY_W + x]=color;
		}
	}
}