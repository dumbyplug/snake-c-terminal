#include <stdio.h>

int main() {

	short menu = 0;
	short select = 1, subSelect = 3;
	short limit = 3, subLimit = 5;

	short gridSize = 3, difficulty = 2, speed = 2;

	char gridSizeText[11];
	char difficultyText[11];
	char speedText[11];
	char message[50];

	while (1) {
		if (menu == 0) {
			printf("+---- Snake Game ----+\n");

			switch (select) {
			case 1:
				printf("|      > Play <      |\tStarts the game with\n"
					"|      Settings      |\tgiven settings...\n"
					"|        Exit        |\n");
				break;
			case 2:
				printf("|        Play        |\tConfigure settings\n"
					"|    > Settings <    |\tfor the game...\n"
					"|        Exit        |\n");
				break;
			case 3:
				printf("|        Play        |\tPlease don't click me...\n"
					"|      Settings      |\n"
					"|      > Exit <      |\n");
				break;
			default:
				printf("|  An error occured  |\n");
				break;
			}

			printf("+--------------------+\n");
		}
		else {

			if (select == 1)
				gridSize = subSelect;
			else if (select == 2)
				difficulty = subSelect;
			else
				speed = subSelect;


			if (difficulty == 1)
				sprintf(difficultyText, "#    ");
			else if (difficulty == 2)
				sprintf(difficultyText, "# #  ");
			else
				sprintf(difficultyText, "# # #");

			if (speed == 1)
				sprintf(speedText, "#    ");
			else if (speed == 2)
				sprintf(speedText, "# #  ");
			else
				sprintf(speedText, "# # #");

			switch (gridSize) {
			case 1:
				sprintf(gridSizeText, "10x10");
				break;
			case 2:
				sprintf(gridSizeText, "15x15");
				break;
			case 3:
				sprintf(gridSizeText, "20x20");
				break;
			case 4:
				sprintf(gridSizeText, "25x25");
				break;
			case 5:
				sprintf(gridSizeText, "30x30");
				break;
			}

			printf("+----- Configuration -----+\n");



			printf("|   Grid size   ");

			if (select == 1 && subSelect != 1)
				printf("< ");
			else
				printf("  ");

			printf("%s", gridSizeText);

			if (select == 1 && subSelect != subLimit)
				printf(" >");
			else
				printf("  ");

			printf(" |\n");



			printf("|  Difficulty   ");

			if (select == 2 && subSelect != 1)
				printf("< ");
			else
				printf("  ");

			printf("%s", difficultyText);

			if (select == 2 && subSelect != subLimit)
				printf(" >");
			else
				printf("  ");

			printf(" |\n");



			printf("|       Speed   ");

			if (select == 3 && subSelect != 1)
				printf("< ");
			else
				printf("  ");

			printf("%s", speedText);

			if (select == 3 && subSelect != subLimit)
				printf(" >");
			else
				printf("  ");

			printf(" |\n");



			printf("+-------------------------+\n");
		}

		switch (getch())
		{
		case 'w':
			select--;
			if (menu == 1) {
				if (select == 1)
					subSelect = gridSize;
				else if (select == 2)
					subSelect = difficulty;
				else
					subSelect = speed;
			}
			break;
		case 's':
			select++;
			if (menu == 1) {
				if (select == 1)
					subSelect = gridSize;
				else if (select == 2)
					subSelect = difficulty;
				else
					subSelect = speed;
			}
			break;
		case 'a':
			if (menu == 1)
				subSelect--;
			break;
		case 'd':
			if (menu == 1)
				subSelect++;
			break;
		case ' ':
			if (menu == 0) {
				switch (select)
				{
				case 1:
					////////////// Game starts here
					//
					//
					//
					printf("Game starts...");
					//
					//
					//
					////////////////////////////// nigga
					break;
				case 2:
					menu = 1;
					select = 1;
					limit = 3;
					subLimit = 5;
					subSelect = gridSize;
					break;
				case 3:
					exit();
					break;
				}
			}
			else {
				menu = 0;
				select = 1;
				limit = 3;
				///////////////////////////////
				///// config function here
				///////////////////////////////
			}
			break;
		}

		if (select > limit) {
			select = 1;
			if (menu == 1) {
				subSelect = gridSize;
			}
		}
		else if (select <= 0) {
			select = limit;
			if (menu == 1) {
				subSelect = speed;
			}
		}


		switch (select)
		{
		case 1:
			subLimit = 5;
			break;
		case 2:
			subLimit = 3;
			break;
		case 3:
			subLimit = 3;
			break;
		default:
			subLimit = 5;
		}


		if (subSelect > subLimit)
			subSelect = subLimit;
		else if (subSelect <= 0)
			subSelect = 1;

		printf("\x1b[2J\x1b[H");

	}

	return 0;
}