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


int fbfd;
//uint16_t* fbp;
struct fb_copyarea rect;
int device;
uint16_t *fbmmap;
int map_input(int);

struct monaLisa
{
	int x;
	int y;
	int width;
	int height;
	uint16_t *pixelarray;
};

void gamepad_signal_handler(int);
void setupFB();
void drawRect(dx, dy, width, height);

int main(int argc, char *argv[])
{
	char start_gamepad_driver[50];
	strcpy( start_gamepad_driver, "modprobe driver-gamepad");
	system(start_gamepad_driver);
	
	device = open("/dev/gamepad", O_RDONLY);
	if (!device) {
        printf("Unable to open driver device, maybe you didn't load the module?\n");
        return EXIT_FAILURE;
    }
	printf("GAMEPAD is opened\n");
    if (signal(SIGIO, &gamepad_signal_handler) == SIG_ERR) {
        printf("An error occurred while register a signal handler.\n");
        return EXIT_FAILURE;
    }
	printf("signal(SIGIO,.....) is good\n");

	if (fcntl(device, F_SETOWN, getpid()) == -1) {
        printf("Error setting pid as owner.\n");
        return EXIT_FAILURE;
    }

		long oflags = fcntl(device, F_GETFL);
    if (fcntl(device, F_SETFL, oflags | FASYNC) == -1) {
        printf("Error setting FASYNC flag.\n");
        return EXIT_FAILURE;
	}


	printf("Hello World, I'm game!\n");
	while(1){
		usleep(16667);
		
		//NOTHING YET
	}
	exit(EXIT_SUCCESS);
}

void setupFB(){
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0){
		printf("error opening Framebuffer");
	}
	printf("Framebuffer opened\n");
	fbmmap = (uint16_t*)mmap(NULL,320*240*2,PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

}


void drawRect(dx, dy, width, height){
	rect.dx = dx;
	rect.dy = dy;
	rect.width = width;
	rect.height = height;
	ioctl(fbfd, 0x4680, &rect); //update display
	printf("display is updated\n");
}
/*
void paintTest(struct monaLisa *Painting){
	int i;
	uint16_t mapPointer* fbmap;
	mapPointer += 320 * Painting.y + Painting.x;
	uint16_t *figPoint = Painting.pixelarray;

	int j;
	int i;
	for (i = 0; i < Painting.height; i++) {
		for (j = 0; j < Painting.width; j++) {
			*mapPointer = *figPoint;
			mapPointer++;
			figPoint++;
		}
		mapPointer += 320 - Painting.width;
	}
	drawRect(Painting.x, Painting.y, Painting.width, Painting.width, Painting.height);

}
*/

int map_input(int input){
    input = ~input;
    uint8_t i;
    for (i = 0; i < 8; i++) {
        uint8_t match = input & (1 << i);
        if ( (1 << i) == match ) {
            return (i+1);
        }
    }
    return 0;
}

void gamepad_signal_handler(int signo){
	//int input_signal=fgetc(device);
	int input_signal;
	read(device, input_signal,4);
	input_signal = ~input_signal;
    uint8_t i;
    uint8_t buttons[8]={0};
    for (i = 0; i < 8; i++) {
        if(input_signal & (1 << i)){
        	buttons[i] = 1; 
        }
		printf("buttons[%d] = %d \n", i+1, buttons[i]);
    }
}
