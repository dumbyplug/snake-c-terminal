#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define COL 64
#define ROW 18
#define DIG 100

int i, j;

void clearGrid(char grid[ROW][COL]){
	// This function clears the grid. Fills every cell with whitespace
	for(i = 0; i < ROW; i++){
		for(j = 0; j < COL; j++){
			if((i == 0) || (i == ROW - 1))
				grid[i][j] = 'E';

			else if((j == 0) || (j == COL - 1))
				grid[i][j] = 'H';
			
			else
				grid[i][j] = ' ';
		}
	}
}

void init_snake(int snake[], int *snake_size){
	// This function initialize snake. It sets it body parts next to each other
	*snake_size = 5;
	for(i = 0; i < *snake_size; i++){
		snake[*snake_size - i - 1] = 5 * DIG + (9 + i);
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


char collision_check(char grid[ROW][COL], int snake[]){
	//checking to see if snake's head collides with walls' symbols or its body's symbols
	switch (grid[snake[0] / DIG][snake[0] % DIG]){
		case 'H': 
		case 'E':
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
	for(i = *snake_size - 1; i > 0; i--){
		snake[i] = snake[i - 1];
	}

	// setting snake's head to its new location
	snake[0] = row * DIG + column;
}



int main(void){
	// Initializing ncurses for getting real time input
	initscr();
	//cbreak();
	noecho();
	start_color();
	use_default_colors();
	//keypad(stdscr, TRUE);  
	nodelay(stdscr, TRUE);

	init_pair(1, COLOR_RED,   -1);// COLOR_WHITE);
	init_pair(2, COLOR_YELLOW,-1);// COLOR_WHITE);
	init_pair(3, COLOR_GREEN, -1);// COLOR_WHITE);
	init_pair(4, COLOR_BLUE,  -1);// COLOR_WHITE);


	srand(time(NULL));

	char grid[ROW][COL];
	int snake[ROW * COL], snake_size, food = 516, grow = 0, size = 0;
	int blank_spaces[ROW * COL];
	char snake_facing = '>';

	clearGrid(grid);
	init_snake(snake, &snake_size);

	blank(grid, blank_spaces, &size);

	char buf, run = 1, moved = '>', move_delay = 8, move_delay_index = 0;
	while (run){
		buf = getch();
		switch (buf){
		case 'd':
			if(moved != '<')
				snake_facing = '>';
            break;
        case 'w':
			if(moved != 'v')
				snake_facing = '^';
            break;
        case 'a':
			if(moved != '>')
				snake_facing = '<';
        	break;
        case 's':
		    if(moved != '^')
			    snake_facing = 'v';
			break;
        case 'q':
            run = 0;
        }
	clearGrid(grid);
	erase();

	grid[food / DIG][food % DIG] = '@';

	if(move_delay < move_delay_index){
		move_snake(snake, &snake_size, snake_facing, &grow);
		move_delay_index = 0;
		moved = snake_facing;
	}

	for(i = snake_size - 1; i > 0; i--){
		grid[snake[i] / DIG][snake[i] % DIG] = '*';
	} 
	//Check the head before overwriting.
	switch(collision_check(grid, snake)){
		case 2:
			run = 0;
			break;
		case 1:
			size = 0;
			blank(grid, blank_spaces, &size);
			food = random_food_appear(blank_spaces, size, snake_size);
			grow = 1;
	}

	grid[snake[0] / DIG][snake[0] % DIG] = 'O';

	for(i = 0; i < ROW; i++){
		for(j = 0; j < COL; j++){
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
			printw("%c", grid[i][j]);
		}
		printw("\n");
	}

	refresh();
	move_delay_index++;
	usleep(16000);

    }
    endwin();
    return 0;
}
