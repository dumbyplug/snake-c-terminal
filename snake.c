#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

#define COL 20
#define ROW 10
#define DIG 100

void clearGrid(char grid[ROW][COL]){
<<<<<<< HEAD
	// This function clears the grid. Fills every cell with whitespace
=======
>>>>>>> 092aebd004d1bf3388c350355f51dd4846dd9bb7
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			grid[i][j] = ' ';
		}
	}
}

void init_snake(int snake[], int *snake_size){
	*snake_size = 3;
	for(int i = 0; i < *snake_size; i++){
		snake[i] = 5 * 100 + (5 + i);
	}
}

void move_snake(int snake[], int *snake_size, int snake_facing){
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
	initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);  
    nodelay(stdscr, TRUE);

	char grid[ROW][COL];
	clearGrid(grid);

	int snake[ROW * COL];
	int snake_size;
	init_snake(snake, &snake_size);
	char snake_facing = '>';

	int i, j;
    int buf;
	char run = 1, move_delay = 12, move_delay_index = 0;
    while (run){
		buf = getch();
        switch (buf){
            case 'd':
				snake_facing = '>';
                break;
            case 'w':
				snake_facing = '^';
                break;
            case 'a':
				snake_facing = '<';
                break;
            case 's':
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
		}

		for(i = 0; i < snake_size - 1; i++){
			grid[snake[i] / 100][snake[i] % 100] = '#';
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
