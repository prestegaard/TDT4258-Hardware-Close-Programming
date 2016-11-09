

#define DISPLAY_H 240
#define DISPLAY_W 320
#define DISPLAY_PIXELS 26800

uint16_t *fbmmap;
int fbfd;
//uint16_t* fbp;
struct fb_copyarea rect;

struct monaLisa
{
	int x;
	int y;
	int width;
	int height;
	uint16_t *pixelarray;
};


void setupFB(){
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0){
		printf("error opening Framebuffer");
	}
	printf("Framebuffer opened\n");
	fbmmap = (uint16_t*)mmap(NULL,320*240*2,PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbmmap = MAP_FAILED) {
		printf(fbmmap failed);
		return EXIT_FAILURE;
	} 
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

	drawRect(0,0, DISPLAY_W, DISPLAY_H)

}