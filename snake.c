#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define COL 24
#define ROW 12
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

int random_food_appear(char grid[ROW][COL], int snake[], int snake_size){
    /*
    This function generates a food on random place in the screen. and each 
	time snake eats the food, it regenerates the food in another place and keeps score.
    Input:
        grid[ROW][COL]: char
    Output:
        the position of the food
    */	
	
	
	//int random_row, random_column;
	if(snake_size >= (ROW - 2) * (COL - 2))
		return -1;
	/*
	do {
		random_row = rand() % (ROW - 2) + 1;      
		random_column = rand() % (COL - 2) + 1;
	} while(grid[random_row][random_column] != ' ');

	return random_row * 100 + random_column;
	*/
	int occupied = ROW * 2 + (COL - 2) * 2 + snake_size; 
	int possible_places[ROW * COL];

	// getting coordinates of all possible places
	for(i = 0; i < ROW; i++){
		for(j = 0; j < COL; j++){
			// canceling the borders
			if((grid[i][j] == 'E') || (grid[i][j] == 'H')) 
				possible_places[i * COL + j] = -1;
			else
				possible_places[i * COL + j] = i * DIG + j;
		}
	}
	// canceling the body of snake 
	for(i = 0; i < snake_size; i++){
		possible_places[(snake[i] / DIG) * COL + snake[i] % DIG] = -1;
	}
	
	//endwin();

	//for(i = 0; i < ROW * COL; i++){
	//	printf("%d\n", possible_places[i]);
	//}

	char swapped = 0;
	int temp;
	for(i = 0; i < (ROW * COL) - 1; i++){
		swapped = 0;
		for(int j = 0; j < (ROW * COL) - i - 1; j++){
			if(possible_places[j] > possible_places[j + 1]){
				temp = possible_places[j];
				possible_places[j] = possible_places[j + 1];
				possible_places[j + 1] = temp;
				swapped = 1;
			}
		}
		if(!swapped)
			break;
	}
	/*
	printf("Testing 2\n");
	for(i = 0; i < ROW * COL; i++){
		printf("%d\n", possible_places[i]);
	}
	*/

	
	int random_number = rand() % (ROW * COL - occupied) + occupied;

	//printf("%d\n", possible_places[random_number]);

	return possible_places[random_number];
}

char collision_check(char grid[ROW][COL], int snake[]){
	//checking to see if snake's head collides with walls' symbols or its body's symbols
	switch (grid[snake[0] / DIG][snake[0] % DIG]){
		case 'H': 
		case 'E':
		case '#':
			return 2;
		case 'o':
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
	cbreak();
	noecho();
	keypad(stdscr, TRUE);  
	nodelay(stdscr, TRUE);

	srand(time(NULL));

	char grid[ROW][COL];
	int snake[ROW * COL], snake_size, food = 516, grow = 0;
	char snake_facing = '>';

	clearGrid(grid);
	init_snake(snake, &snake_size);

	char buf, run = 1, moved = '>', move_delay = 12, move_delay_index = 0;
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

	grid[food / DIG][food % DIG] = 'o';

	if(move_delay < move_delay_index){
		move_snake(snake, &snake_size, snake_facing, &grow);
		move_delay_index = 0;
		moved = snake_facing;
	}

	for(i = snake_size - 1; i > 0; i--){
		grid[snake[i] / DIG][snake[i] % DIG] = '#';
	} 
	//Check the head before overwriting.
	switch(collision_check(grid, snake)){
		case 2:
			run = 0;
			break;
		case 1:
			food = random_food_appear(grid, snake, snake_size);
			grow = 1;
	}

	grid[snake[0] / DIG][snake[0] % DIG] = '@';

	for(i = 0; i < ROW; i++){
		for(j = 0; j < COL; j++){
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
