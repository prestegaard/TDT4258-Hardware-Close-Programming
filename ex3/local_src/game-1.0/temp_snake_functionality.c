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
	Color color_background;
	Color color_body;
	Color color_food;
	uint8_t food_x;
	uint8_t food_y;
	uint8_t direction;
	uint8_t length;
	int8_t head_x; //position can be -1, then you have lost
	int8_t head_y; //position can be -1, then you have lost
	uint8_t tail_x;
	uint8_t tail_y;
	uint8_t [40][2] snake_trace; //trace[n][0] is x, trace[n][1] is y

}

game (..)
 	...
	...

	play_snake(..);


play_snake(..){
	uint16_t* snake;

	while(not_looser){
		
		calculate_next_round();
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

	uint8_t i,x,y;
	switch snake.direction{
		case up:
			snake.head_y += 1;
			if(snake.head_y > 39){
				you_loose=1;
			}
		case down:
			snake.head_y -= 1;
			if(snake.head_y < 0){
				you_loose=1;
			} 
		case left:
			snake.head_x -= 1;
			if(snake.head_x < 0){
				you_loose=1;
			}
		case rigth:
			snake.head_x += 1;
			if(snake.head_x > 39){
				you_loose=1;
			}
	if(you_loose == 1){
		return snake.length;
	}

	tail_x = snake.trace[0][0];
	tail_y = snake.trace[0][y];
	//update trace, remove tail, add head.
	//todo: something other when food is eaten
	for(i=0; i<snake.length -1; i++){
		snake.trace[i][0] = snake.trace[i+1][0];
		snake.trace[i][1] = snake.trace[i+1][1];
	}
	snake.trace[snake.length-1][head_x]=1;
	snake.trace[snake.length-1][head_y]=1;


	//this could be done before trace is updated ..
	if(snake.head_x == food_x && snake.head_y == food_y){
		snake.length += 2;
	}

	update_snake_on_diplay(&snake){
		uint16_t head_x, head_y, tail_x, tail_y;
		Rectangle head, tail, food;
		//Convert snake tail and head game positions into display positions
		head.x = 60  + (5*snake.head_x);
		head.y = 220 - (5*snake.head_y);
		tail.x = 60  + (5*snake.tail_x);
		tail.y = 220 - (5*snake.tail_y);
		food.x = 61  + (5*snake.food_x);
		food.y = 220 - (5*snake.food_y);
		
		head.width  = 4;
		head.height = 4;
		tail.width  = 4;
		tail.height = 4;

		food.width  = 2;
		food.height = 2;

		//add new head
		//remove tail from display
		fill_rectangle(food, snake.food_food);
		fill_rectangle(tail, snake.color_background);
		fill_rectangle(head, snake.color_body);
		update_display(food);
		update_display(tail);
		update_display(head);
	}




