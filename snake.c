#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define DIG 100


void clearGrid(char grid[ROW][COL], int wall_places[ROW*COL], int wall_size){
	// This function clears the grid. Fills every cell with whitespace
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			if((i == 0) || (i == ROW - 1))
				grid[i][j] = 'E';

			else if((j == 0) || (j == COL - 1))
				grid[i][j] = 'H';
			
			else
				grid[i][j] = ' ';
		}
	}for(int i = 0; i < wall_size; i++){
		grid[wall_places[i] / DIG][wall_places[i] % 100] = 'E';
	}
}

void init_snake(int snake[], int *snake_size){
	// This function initialize snake. It sets it body parts next to each other
	*snake_size = 3;
	for(int i = 0; i < *snake_size; i++){
		snake[*snake_size - i - 1] = (ROW / 2 - 1) * DIG + (1 + i);
	}
}

void blank(char grid[ROW][COL], int blank_spaces[], int *size){
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			if(grid[i][j] == ' '){
				blank_spaces[*size] = i * 100 + j;
				(*size)++;
			}
		}
	}
}


int random_food_appear(int blank_spaces[], int size, int snake_size){
    /*
    This function generates a food on random place in the screen, 
	and each time snake eats the food, it regenerates the food in 
	another place and keeps score.
    Input:
        grid[ROW][COL]: char
    Output:
        void
    */	
	if(snake_size < (ROW - 1) * (COL - 1)){
		int random_number = rand() % size;
		return blank_spaces[random_number];
	}else{
		return -1;
	}
}

int random_wall_appear(int blank_spaces[], int size, int snake_size){
    /*
    This function generates a wall on random place in the screen, 
	and each time snake eats the food, it regenerates the wall in 
	another place.
    */	
	if(snake_size < (ROW - 1) * (COL - 1)){
		int random_number = rand() % size;
		return blank_spaces[random_number];
	}else{
		return -1;
	}
}

char collision_check(char grid[ROW][COL], int snake[]){
	//checking to see if snake's head collides with walls' symbols or its body's symbols
	switch (grid[snake[0] / DIG][snake[0] % DIG]){
		case 'H': 
		case 'E':
			if(DIFFICULTY == 0)
				return 0;
		case '*':
			return 2;
		case '@':
			return 1;
	}

	return 0;
}

void move_snake(int snake[], int *snake_size, char snake_facing, int *grow){
	// moves snake to the direction it is looking 
	int row = snake[0] / DIG, column = snake[0] % DIG;
	switch(snake_facing){
		case '>':
			column++;
			break;
		case '^':
			row--;
			break;
		case '<':
			column--;
			break;
		case 'v':
			row++;
			break;
	}
	// if snake has eaten a food, increse its size
	if(*grow){
		*grow = 0;
		(*snake_size)++;
	}

	// overwriting every element with one before.  
	for(int i = *snake_size - 1; i > 0; i--){
		snake[i] = snake[i - 1];
	}

	if(DIFFICULTY == 0){
		if(row < 1)
			row = ROW - 2;
		else if(row > ROW - 2)
			row = 1;
		if(column < 1)
			column = COL - 2;
		else if(column > COL - 2)
			column = 1;
	}

	// setting snake's head to its new location
	snake[0] = row * DIG + column;
}



int game(void){
	// Initializing ncurses for getting real time input
	
	/*initscr();
	//cbreak();
	noecho();
	start_color();
	use_default_colors();
	keypad(stdscr, TRUE);  
	nodelay(stdscr, TRUE);
	*/

	init_pair(1, COLOR_RED,   -1);
	init_pair(2, COLOR_YELLOW,-1);
	init_pair(3, COLOR_GREEN, -1);
	init_pair(4, COLOR_BLUE,  -1);
	init_pair(5, -1,  -1);

	srand(time(NULL));

	char grid[ROW][COL];
	int snake[ROW * COL], snake_size, food = 516, grow = 0, size = 0, wall = 0;
	int blank_spaces[ROW * COL];
	char snake_facing = '>';
	int wall_places[ROW*COL], wall_size = 0;

	clearGrid(grid, wall_places, wall_size);
	init_snake(snake, &snake_size);

	blank(grid, blank_spaces, &size);

	char run = 1, moved = '>', move_index = 0;
    int times_snake_moves = 0;
	while (run){
		switch (getch()){
		case 'd': case KEY_RIGHT:
			if(moved != '<')
				snake_facing = '>';
            break;
		case 'w':case KEY_UP:
			if(moved != 'v')
				snake_facing = '^';
            break;
        case 'a':case KEY_LEFT:
			if(moved != '>')
				snake_facing = '<';
        	break;
        case 's': case KEY_DOWN:
		    if(moved != '^')
			    snake_facing = 'v';
			break;
        case 'q':
            run = 0;
        }
    clearGrid(grid, wall_places, wall_size);
    erase();

    grid[food / DIG][food % DIG] = '@';
    grid[wall / DIG][wall % DIG] = 'E';
    

	if(SPEED < move_index){
		move_snake(snake, &snake_size, snake_facing, &grow);
		move_index = 0;
		moved = snake_facing;
	}

	for(int i = snake_size - 1; i > 0; i--){
		grid[snake[i] / DIG][snake[i] % DIG] = '*';
	} 

	//Check the head before overwriting.
    int collision = collision_check(grid, snake);
    if(DIFFICULTY == 2){
        if(collision != 1 && collision != 2){
            if(times_snake_moves >= size){
                collision = 1;
                times_snake_moves = 0;
            }else{
                times_snake_moves++;
            }
        }else{
            times_snake_moves = 0;
        }
    }
	switch(collision){
		case 2:
			run = 0;
			break;
		case 1:
            size = 0;
            blank(grid, blank_spaces, &size);
            food = random_food_appear(blank_spaces, size, snake_size);size = 0;
            if(DIFFICULTY == 2){
                blank(grid, blank_spaces, &size);
                wall = random_wall_appear(blank_spaces, size, snake_size);
                wall_places[wall_size] = wall;
                wall_size++;
            }grow = 1;
	}

	grid[snake[0] / DIG][snake[0] % DIG] = 'O';

	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			switch(grid[i][j]){
				case '@':
					attron(COLOR_PAIR(1));
					break;
				case 'O':
					attron(COLOR_PAIR(2));
					break;
				case '*':
					attron(COLOR_PAIR(3));
					break;
				default:
					attron(COLOR_PAIR(4));
			}
			printw("%c ", grid[i][j]);
		}
		printw("\n");
	}

	refresh();
	move_index++;
	usleep(16000);

    }
    endwin();
	attron(COLOR_PAIR(5));
    return 0;
}
