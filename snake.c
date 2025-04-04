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
		snake[i] = 5 * 100 + (5 + i);
	}
}


//checking to see if snake's head collides with walls' symbols or its body's symbols
char collision_check(int snake[], int snake_size, char grid[ROW][COL]){
	switch (grid[snake[snake_size - 1] / 100][snake[snake_size - 1] % 100]){
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
			row = snake[*snake_size - 1] / 100;
			column = snake[*snake_size - 1] % 100 + 1;
			break;
		case '^':
			row = snake[*snake_size - 1] / 100 - 1;
			column = snake[*snake_size - 1] % 100;
			break;
		case '<':
			row = snake[*snake_size - 1] / 100;
			column = snake[*snake_size - 1] % 100 - 1;
			break;
		case 'v':
			row = snake[*snake_size - 1] / 100 + 1;
			column = snake[*snake_size - 1] % 100;
			break;
	}
	for(i = 0; i < *snake_size - 1; i++){
		snake[i] = snake[i + 1];
	}
	snake[*snake_size - 1] = row * 100 + column;
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

	clearGrid(grid);
	init_snake(snake, &snake_size);

	int i, j;
    char buf, run = 1, moved = 0,
		 move_delay = 12, move_delay_index = 0;
    while (run){
		buf = getch();
        switch (buf){
            case 'd':
				if((snake_facing != '<') && !moved){
					snake_facing = '>';
					moved = 1;
				}
                break;
            case 'w':
				if((snake_facing != 'v') && !moved){
					snake_facing = '^';
					moved = 1;
				}
                break;
            case 'a':
				if((snake_facing != '>') && !moved){
					snake_facing = '<';
					moved = 1;
				}
                break;
            case 's':
				if((snake_facing != '^') && !moved){
					snake_facing = 'v';
					moved = 1;
				}
                break;
            case 'q':
                run = 0;
        }
		clearGrid(grid);
		erase();

		if(move_delay < move_delay_index){
			move_snake(snake, &snake_size, snake_facing);
			move_delay_index = 0;
			moved = 0;
		}

		for(i = 0; i < snake_size - 1; i++){
			grid[snake[i] / 100][snake[i] % 100] = '#';
		} 
		//Check the head before overwriting.
		if (collision_check(snake,snake_size,grid) == 2){
			run = 0;
		}

		grid[snake[snake_size - 1] / 100][snake[snake_size - 1] % 100] = '@';

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
