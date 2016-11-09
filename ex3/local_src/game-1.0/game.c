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


int device;

int map_input(int);



void gamepad_signal_handler(int);


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
	setupFB();



	printf("Hello World, I'm game!\n");
	while(1){
		usleep(16667);
		
		//NOTHING YET
	}
	exit(EXIT_SUCCESS);
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



void gamepad_signal_handler(int signo){
	//int input_signal=fgetc(device);
	int input_signal;
	read(device, &input_signal,1);
	input_signal = ~input_signal;
    uint8_t i;
    uint8_t buttons[8]={0};
    for (i = 0; i < 8; i++) {
        if(input_signal & (1 << i)){
        	buttons[i] = 1; 
        }
		printf("buttons[%d] = %d \n", i+1, buttons[i]);
    }
    if (buttons[0] == 1){
    	fillBackground(31,0,25);
    }
    if (buttons[1] == 1){
    	fillBackground(0,63,0);
    }
}

