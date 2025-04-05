#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

#define COL 24
#define ROW 12
#define DIG 100

void clearGrid(char grid[ROW][COL]){
	// This function clears the grid. Fills every cell with whitespace
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			if((i == 0) || (i == ROW - 1))
				grid[i][j] = '-';
			else if((j == 0) || (j == COL - 1))
				grid[i][j] = '|';
			else
				grid[i][j] = ' ';
		}
	}
}

void init_snake(int snake[], int *snake_size){
	// This function initialize snake. It sets it body parts next to each other
	*snake_size = 5;
	for(int i = 0; i < *snake_size; i++){
		snake[i] = 5 * DIG + (5 + i);
	}
}


//checking to see if snake's head collides with walls' symbols or its body's symbols
char collision_check(int snake[], int snake_size, char grid[ROW][COL]){
	switch (grid[snake[snake_size - 1] / DIG][snake[snake_size - 1] % DIG]){
		case '|':
			return 2;
		case '-':
			return 2;
		case '#':
			return 2;
	}

	return 0;
}

void move_snake(int snake[], int *snake_size, char snake_facing){
	int i, row, column;
	switch(snake_facing){
		case '>':
			row = snake[*snake_size - 1] / DIG;
			column = snake[*snake_size - 1] % DIG + 1;
			break;
		case '^':
			row = snake[*snake_size - 1] / DIG - 1;
			column = snake[*snake_size - 1] % DIG;
			break;
		case '<':
			row = snake[*snake_size - 1] / DIG;
			column = snake[*snake_size - 1] % DIG - 1;
			break;
		case 'v':
			row = snake[*snake_size - 1] / DIG + 1;
			column = snake[*snake_size - 1] % DIG;
			break;
	}
	for(i = 0; i < *snake_size - 1; i++){
		snake[i] = snake[i + 1];
	}
	snake[*snake_size - 1] = row * DIG + column;
}



int main(void){
	// Initializing ncurses for getting real time input
	initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);  
    nodelay(stdscr, TRUE);

	char grid[ROW][COL];
	int snake[ROW * COL], snake_size;
	char snake_facing = '>';
	int i, j;
    char buf;
	char run = 1;

	char moved = '>', move_delay = 12, move_delay_index = 0;

	clearGrid(grid);
	init_snake(snake, &snake_size);

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

		if(move_delay < move_delay_index){
			move_snake(snake, &snake_size, snake_facing);
			move_delay_index = 0;
			moved = snake_facing;
		}

		for(i = 0; i < snake_size - 1; i++){
			grid[snake[i] / DIG][snake[i] % DIG] = '#';
		} 
		//Check the head before overwriting.
		if (collision_check(snake,snake_size,grid) == 2){
			run = 0;
		}

		grid[snake[snake_size - 1] / DIG][snake[snake_size - 1] % DIG] = '@';

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
