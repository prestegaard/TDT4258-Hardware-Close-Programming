uint16_t mapRGB(uint8_t r, uint8_t g, uint8_t b){
	// r max 5 bit => 0-31
	// g max 6 bit => 0-63
	// b max 5 bit => 0-31
	uint16_t mapped_rgb = 0;
	if( r >= 31 ){
		r = 31;
	}
	if(g >= 63){
		g = 63;
	}
	if( b >= 31){
		b= 31;
	}
	mapped_rgb |= (r << 11);
	mapped_rgb |= (g << 5);
	mapped_rgb |= (b << 0);

	return mapped_rgb;
}

void fill_rectangle(uint16_t x_pos_left, uint16_t x_pos_right, uint16_t y_pos_top, uint16_t y_pos_bottom, uint8_t R, uint8_t G, uint8_t B){
	uint16_t y;
	uint16_t x;
	uint16_t color = mapRGB(R,G,B);
	for(y=y_pos_top; y<=y_pos_bottom; y++){
		for(x=x_pos_left; x <= x_pos_right; x++){
			fbmmap[y*DISPLAY_W + x]=color;
		}
	}
}



/* display 


0: 0 ---------------- 319
1: 0 ---------------- 319
2: 0 ---------------- 319
3: 0 ---------------- 319
4:
-
-
-
-
239: 0 -------------- 319


-draw squre for snake body 5x5

void draw_snake(...){
	...
	for loop...
		fill_rectangle(5 x 5, rgb)
		...	

}

enum uint8_t direction = {up, down, left, rigth};

struct snake{
	uint8_t direction;
	uint8_t length;
	int8_t head_x;
	int8_t head_y;
	uint8_t [2]* snake_trace;
}

game (..)
 	...
	...

	play_snake(..);


play_snake(..){
	uint16_t* snake;

	while(not_looser){
		draw_snake(...);
		update_display();
		u_sleep(difficulty);
	}
	restart_game(..);

}


firkant slange er 5x5 inkl blank plass.
0,0 på firkant er oppe venstre,  4x4 er fyll på slangen
5,5 på firkant er nede venstre
altså nede på 5 og høyre på 5 er blank

spillbrett på 200x200 pixler, det gir et brett på 40x40 slangedeler

uint8_t spillbrett[40][40];

	uint8_t x,y;
	switch snake.direction{
	case up:
		snake.head_y=snake.head_y+1;'
		if(snake.head_y>=40){
			you_loose==1;
		}
	case down:
		snake.head_y=snake.head_y-1;'
		if(snake.head_y<=0){
			you_loose==1;
		} 
	

	snake.trace[head_x][head_y]=1;
	for(x=0;x<40;x++){
		for(y=0;y<40;y++){
			if(snake.trace[x][y]){

				update_display();
			}
		}
	}



*/