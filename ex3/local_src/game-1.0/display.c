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




int init_display_fb(){
	//remove cursor blink, OS default...
	char remove_cursor_blink[50];
	strcpy( remove_cursor_blink, "echo 0 > /sys/class/graphics/fbcon/cursor_blink");
	system(remove_cursor_blink);

	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0){
		printf("Error opening Framebuffer\n");
		return EXIT_FAILURE;
	}
	printf("Framebuffer opened\n");
	fbmmap = (uint16_t*)mmap(NULL,320*240*2,PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbmmap == MAP_FAILED) {
		printf("fbmmap failed\n");
		return EXIT_FAILURE;
	} 
	return EXIT_SUCCESS;
}
int deinit_display_fb(){
	if(close(fbfd < 0)){
		printf("Could not close framebuffer\n");
		return EXIT_FAILURE;
	}
	printf("Framebuffer is closed\n");
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

void update_display(Rectangle rectangle){
	rect.dx = rectangle.dx;
	rect.dy = rectangle.dy;
	rect.width = rectangle.width;
	rect.height = rectangle.height;
	ioctl(fbfd, 0x4680, &rect); //update display
}


void fill_background(uint8_t R, uint8_t G, uint8_t B){
	int pixels;
	uint16_t color = mapRGB(R,G,B);

	for (pixels = 0; pixels < DISPLAY_PIXELS; pixels++){
		fbmmap[pixels] = color;
	}



}

void fill_rectangle(Rectangle rectangle, Color color){
	uint16_t y;
	uint16_t x;
	uint16_t color_mapped = mapRGB(color.R, color.G, color.B);
	for(y=rectangle.dy; y<rectangle.height; y++){
		for(x=rectangle.dx; x < rectangle.width; x++){
			fbmmap[y*DISPLAY_W + x]=color_mapped;
		}
	}
}
