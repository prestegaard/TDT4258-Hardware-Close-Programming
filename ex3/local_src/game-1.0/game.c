#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


int fbfd;
//uint16_t* fbp;
struct fb_copyarea screen;


int main(int argc, char *argv[])
{

	fbfd = open("/dev/fb0", O_RDWR);
	screen.dx = 0;
	screen.dy = 0;
	screen.width = 320;
	screen.height = 240;
	ioctl(fbfd, 0x4680, &screen);

	printf("Hello World, I'm game!\n");

	exit(EXIT_SUCCESS);
}
