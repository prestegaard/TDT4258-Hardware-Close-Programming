#define SCR          ((volatile uint32_t*)0xe000ed10)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "display.h"

int device;
volatile uint8_t pause_game=0;
uint16_t play_snake();
void snake_init();
void snake_update_head_position();
void snake_manage_food();
void snake_check_if_bites_tail();
void snake_calculate_new_trace();
void snake_update_display();

uint8_t done_playing_snake=0;
uint8_t change_body_color=0;
int init_gamepad_driver();
int deinit_gamepad_driver();
void gamepad_signal_handler(int);

enum Direction {up, down, left, rigth};

typedef struct Snake{
	uint8_t food_x;
	uint8_t food_y;
	enum Direction direction;
	uint8_t length;
	int8_t head_x; //position can be -1, then you have lost
	int8_t head_y; //position can be -1, then you have lost
	uint8_t tail_x;
	uint8_t tail_y;
	int8_t trace[160][2] ; //trace[n][0] is x, trace[n][1] is y
	uint8_t game_over;
}Snake;

Snake snake;

Color color_background= {0,0,31}; //Blue background
Color color_body = {0,60,20}; 	  //Yellow body
Color color_food = {31,0,0};	  //Red food


Rectangle rectang={
	60,
	20,
	260,
	220
};

Rectangle fullscreen={
	0,
	00,
	320,
	240
};

Color red={
	31,
	0,
	0
};


int init_gamepad_driver(){
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
	return EXIT_SUCCESS;
}
int deinit_gamepad_driver(){

	if(close(device)< 0){
		printf("Could not close gampead driver\n");
		return EXIT_FAILURE;
	}
	char start_gamepad_driver[50];
	strcpy( start_gamepad_driver, "modprobe -r driver-gamepad");
	system(start_gamepad_driver);
	printf("Gamepad driver is closed\n");
	return EXIT_SUCCESS;
}
int main(int argc, char *argv[]){
	init_gamepad_driver();
	init_display_fb();

	printf("Hello World, I'm game!\n");
	while(!done_playing_snake){
		play_snake();		
	}
	fill_background(20,10,3);
	update_display(fullscreen);
	deinit_gamepad_driver();
	deinit_display_fb();
	exit(EXIT_SUCCESS);
}

uint16_t play_snake(){
	do{
		
		snake_init();
		do{
			snake_update_head_position();
			snake_check_if_bites_tail();
			if(snake.game_over){
				printf("(GAME OVER :-) \n");
			}
			else{
				snake_calculate_new_trace();
				snake_manage_food();
				snake_update_display();
			}
			//printf("----------------------\n");
			usleep(50000); 		   //50 ms 
			while(pause_game){
				usleep(100000);    //100 ms
			}
		}while(!snake.game_over && !done_playing_snake); 
		printf("Congratulations, your score is: %d\n" , snake.length);
	}while(!done_playing_snake);  //restart game

	return snake.length;
}

void snake_init(){
	time_t seed;
	srand(seed);
	seed=time(0);
	done_playing_snake=0;
	pause_game=1;
	snake.game_over=0;
	snake.food_x=20;
	snake.food_y=20;
	snake.direction=rigth;
	snake.length=1;
	snake.head_x=20;
	snake.head_y=30; //position can be -1, then you have lost
	snake.trace[0][0] = snake.head_x; //trace[n][0] is x, trace[n][1] is y
	snake.trace[0][1] = snake.head_y;
	fill_background(0,0,0);
	fill_rectangle(rectang, color_background);
	update_display(fullscreen);
	printf("Playing snake\n");
}

void snake_update_head_position(){
	switch(snake.direction) {
		case up:
			if(snake.head_y > 39){
				snake.game_over=1;
			}
			else
				snake.head_y += 1;
			break;
		case down:
			if(snake.head_y < 0){
				snake.game_over=1;
			} 
			else
				snake.head_y -= 1;
			break;
		case left:
			if(snake.head_x < 0){
				snake.game_over=1;
			}
			else
				snake.head_x -= 1;
			break;
		case rigth:
			if(snake.head_x > 39){
				snake.game_over=1;
			}
			else	
				snake.head_x += 1;
			break;
	}
}
void snake_check_if_bites_tail(){
	uint8_t i;
	if(snake.length>1){
		for(i=0; i<snake.length -1; i++){
			if((snake.head_x == snake.trace[i][0]) && (snake.head_y == snake.trace[i][1])){
			 	snake.game_over=1;
			}
		}
	}
}
void snake_manage_food(){
	//Check if food is eaten, get point and spawn new food
	if((snake.head_x == snake.food_x) && (snake.head_y == snake.food_y)){
		snake.length += 1;
		//spawn new food
		snake.food_x=rand()%40;
		snake.food_y=(rand()% + 13)%40; //make y position somewht different than x position
	}
}

void snake_calculate_new_trace(){
	snake.tail_x = snake.trace[0][0];
	snake.tail_y = snake.trace[0][1];
	//update trace, remove tail, add head.
	//todo: something other when food is eaten
	uint8_t i;
	if(snake.length>1){
		//printf("Snake trace X: ");
		for(i=0; i<snake.length -1; i++){
			snake.trace[i][0] = snake.trace[i+1][0];
			//printf(" %d", snake.trace[i][0]);
		}
		//printf("\nSnake trace Y: ");
		for(i=0; i<snake.length -1; i++){	
			snake.trace[i][1] = snake.trace[i+1][1];
			//printf(" %d", snake.trace[i][1]);
		}
		//printf("\n");
	}
	snake.trace[snake.length-1][0]=snake.head_x;
	snake.trace[snake.length-1][1]=snake.head_y;
}
void snake_update_display(){
	Rectangle head, tail, food;
	//Convert snake tail and head game positions into display positions
	head.dx = 60  + (5*snake.head_x);
	head.dy = 216 - (5*snake.head_y);
	tail.dx = 60  + (5*snake.tail_x);
	tail.dy = 216 - (5*snake.tail_y);
	food.dx = 60  + (5*snake.food_x);
	food.dy = 216 - (5*snake.food_y);

	/*printf("direction: %d\n",snake.direction);
	printf("head_x.  y:  %d : %d\n",snake.head_x, snake.head_y);
	printf("head.dx, dy: %d : %d \n", head.dx, head.dy);
	printf("tail_x.  y:  %d : %d\n",snake.tail_x, snake.tail_y);
	printf("tail.dx, dy: %d : %d \n", tail.dx, tail.dy);
	*/


	head.width  = head.dx + 4;
	head.height = head.dy + 4;
	tail.width  = tail.dx + 4;
	tail.height = tail.dy + 4;
	
	food.width  = 2 + food.dx;
	food.height = 2 + food.dy;

	fill_rectangle(head, color_body);
	usleep(10);
	fill_rectangle(food, color_food);
	usleep(10);
	fill_rectangle(tail, color_background);
	usleep(10);
	update_display(head);
	usleep(10);
	update_display(food);
	usleep(10);
	update_display(tail);
	usleep(10);
}

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
		//printf("buttons[%d] = %d \n", i+1, buttons[i]);
    }
    if (buttons[0] == 1){
    	//Deepsleep -> game is paused
    	char suspend_to_ram[50];
		strcpy(suspend_to_ram, "echo mem > /sys/power/state");
		system(suspend_to_ram);
		fill_background(0,0,0);
		fill_rectangle(rectang, color_background);
		update_display(fullscreen);
    }
    if (buttons[1] == 1){
    		if(pause_game){
    			pause_game=0;
    		}
    		else{
    			pause_game=1;
    		}
    }
    if (buttons[2] == 1){
    	Color temp=color_body;
    	color_body=color_background;
    	color_background=temp;
    	fill_background(0,0,0);
		fill_rectangle(rectang, color_background);
		update_display(fullscreen);
    	
    }
    if (buttons[3] == 1){
    	done_playing_snake=1;
    }
    if ((buttons[4] == 1) && (snake.direction != rigth)){
    	snake.direction=left;
    }
    if ((buttons[5] == 1) && (snake.direction != down)){
    	snake.direction=up;
    }
    if ((buttons[6] == 1) && (snake.direction != left)){	
    	snake.direction=rigth;
    }
    if ((buttons[7] == 1) && (snake.direction != up)){
  		 snake.direction=down;
  	}
}

