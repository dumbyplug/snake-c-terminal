#include <stdio.h>

int main() {

	short menu = 0;
	short select = 1, subSelect = 1;
	short limit = 3, subLimit = 1;

	short gridSize = 1, difficulty = 1;

	char gridSizeText[11];
	char difficultyText[11];

	while (1) {
		if (menu == 0) {
			limit = 3;
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
			limit = 2;

			if (difficulty == 1)
				sprintf(difficultyText, "  #     >");
			else if (difficulty == 2)
				sprintf(difficultyText, "< # #   >");
			else
				sprintf(difficultyText, "< # # #  ");

			switch (gridSize) {
			case 1:
				sprintf(gridSizeText, "  10x10 >");
				break;
			case 2:
				sprintf(gridSizeText, "< 15x15 >");
				break;
			case 3:
				sprintf(gridSizeText, "< 20x20 >");
				break;
			case 4:
				sprintf(gridSizeText, "< 25x25 >");
				break;
			case 5:
				sprintf(gridSizeText, "< 30x30  ");
				break;
			}

			printf("+----- Configuration -----+\n");

			switch (select)
			{
			case 1:
				subLimit = 5;
				break;
			case 2:
				subLimit = 3;
				break;
			default:
				break;
			}

			printf("|   Grid size   %s |\n|  Difficulty   %s |\n",gridSizeText,difficultyText);

			printf("+-------------------------+\n");
		}

		switch (getch())
		{
		case 'w':
			select--;
			break;
		case 's':
			select++;
			break;
		case 'a':
			if (menu != 0)
				subSelect--;
			break;
		case 'd':
			if (menu != 0)
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
					break;
				case 3:
					exit();
					break;
				}
			}
			else {
				// later
			}
			break;
		}

		if (select > limit)
			select = 1;
		else if (select <= 0)
			select = 3;

		if (subSelect > subLimit)
			subSelect = subLimit;
		else if (subSelect <= 0)
			subSelect = 1;

		printf("\x1b[2J\x1b[H");

	}

	return 0;
}