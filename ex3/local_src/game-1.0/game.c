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

void gamepad_signal_handler(int);

int main(int argc, char *argv[])
{
	char start_gamepad_driver[50];
	strcpy( start_gamepad_driver, "modprobe driver-gamepad");
	system(start_gamepad_driver);
	device = open("/dev/gamepad", O_RDONLY);
	printf("GAMEPAD is opened\n");
	 if (!device) {
	        printf("Unable to open driver device, maybe you didn't load the module?\n");
	        return EXIT_FAILURE;
	    }
	    if (signal(SIGIO, &gamepad_signal_handler) == SIG_ERR) {
	        printf("An error occurred while register a signal handler.\n");
	        return EXIT_FAILURE;
	    }
			printf("signal(SIGIO,.....) is good\n");

			if (fcntl(device, F_SETOWN, getpid()) == -1) {
	        printf("Error setting pid as owner.\n");
	        //return EXIT_FAILURE;
	    }


			long oflags = fcntl(device, F_GETFL);
	    if (fcntl(device, F_SETFL, oflags | FASYNC) == -1) {
	        printf("Error setting FASYNC flag.\n");
	        //return EXIT_FAILURE;
	}


	//printf("Button is pressed");
	fbfd = open("/dev/fb0", O_RDWR);
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	ioctl(fbfd, 0x4680, &rect);

	printf("Hello World, I'm game!\n");
	while(1){
		usleep(16667);
		//NOTHING YET
	}
	exit(EXIT_SUCCESS);
}

void gamepad_signal_handler(int signo){
	uint32_t button_state_raw;
	read(device, &button_state_raw, 1);
	uint8_t i;
	printf("Signal received, check which buttons are pressed\n");
	for(i=0; i<8; i++){
		uint8_t button_state_mapped=(uint8_t)(button_state_raw>>i) %2;
		if(button_state_mapped == i){
			printf("BUTTON %d is pressed\n", i);
		}
	}

}
