// SIMPLE MINESWEEPER GAME IN C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // for atoi, ik it's super easy to make but why do i care?

#define checkMines(a,b) (board->cells[b][a].bomb) ? currCell.surrounding++ : 0
#define checkMinesAndReveal(a,b) (!board->cells[b][a].shown) ? revealFrom(board, a, b) : 0
#define currCell board->cells[y][x]

typedef struct Cell{
	unsigned bomb:1;
	char surrounding;
	unsigned shown:1;
	unsigned marked:1;
} cell_t;

typedef struct Board{
	int square_width;
	cell_t** cells;
	int mineContent;
	int mineLeft;
	int* latestReveal;
} board_t;

board_t* genBoard(int size, int seed, int mines){
	srand(seed);
	board_t *board = calloc(1,sizeof(board_t));
	board->square_width = size+2;
	board->cells = calloc(size*sizeof(cell_t *), sizeof(cell_t));
	board->mineContent = mines;
	board->mineLeft = mines;
	board->latestReveal = calloc(2,sizeof(int));
	for (int i = 0; i < size+2; i++)
		board->cells[i] = (cell_t *)calloc(size+2,sizeof(cell_t));
	int minePos[board->mineContent][2];
	for (int i = 0; i < board->mineContent; i++){
		int incorrect = 1;
		while(incorrect){
			int randone = (rand()%(board->square_width-2+1)+1);
			int randtwo = (rand()%(board->square_width-2+1)+1);
			if(board->cells[randone][randtwo].bomb == 0){
				incorrect = 0;
				board->cells[randone][randtwo].bomb = 1;
			}
		}
	}
	for (int y = 1; y < board->square_width-1; y++){
		for (int x = 1; x < board->square_width-1; x++){
			if (currCell.bomb == 0){
				currCell.surrounding = board->cells[y-1][x-1].bomb + board->cells[y-1][x].bomb + board->cells[y-1][x+1].bomb + board->cells[y][x-1].bomb + board->cells[y][x+1].bomb + board->cells[y+1][x-1].bomb + board->cells[y+1][x].bomb + board->cells[y+1][x+1].bomb;
			}
		}
	}
	return board;
}

int revealFrom(board_t* board, int x, int y){
	currCell.shown = 1;
	if(currCell.surrounding == 0){
		checkMinesAndReveal(x-1,y-1); // god i love macros
		checkMinesAndReveal(x,y-1);
		checkMinesAndReveal(x+1,y-1);
		checkMinesAndReveal(x-1,y);
		checkMinesAndReveal(x+1,y);
		checkMinesAndReveal(x-1,y+1);
		checkMinesAndReveal(x,y+1);
		checkMinesAndReveal(x+1,y+1);
		return 1;
	}
	return 0;
}

void printBoard(board_t board){
	char colors[][9] = {"\e[1;37m", "\e[1;94m", "\e[0;32m", "\e[0;91m", "\e[1;34m", "\e[1;31m", "\e[0;36m", "\e[0;107m", "\e[1;93m"};
	for(int y = 1; y < board.square_width-1; y++){
		printf("%d:\t", y);
		for(int x = 1; x < board.square_width-1; x++){
			if(board.cells[y][x].marked){
				printf("\e[0;101m*\e[0m ");
			}
			else if(x==board.latestReveal[0] && y==board.latestReveal[1] && board.cells[y][x].shown){
				printf("%s%d\e[0m ", "\e[1;93m", board.cells[y][x].surrounding);
			}
			else{
			(!board.cells[y][x].shown) ? printf("\e[0;95m#\e[0m ") : (board.cells[y][x].bomb) ? printf("\e[0;101mx\e[0m ") : printf("%s%d\e[0m ", colors[board.cells[y][x].surrounding], board.cells[y][x].surrounding);
			}
		}
		putchar('\n');
	}
	putchar('\t');
	int i,colChar;
	i = 0;
	for(colChar = 'a'; i < board.square_width-2 && colChar <= 'z'; i++){
		putchar(colChar++);
		putchar(' ');
	}
	for(colChar = 'A'; i < board.square_width-2 && colChar <= 'Z'; i++){
        	putchar(colChar++);
		putchar(' ');
	}
	for(colChar = '0'; i < board.square_width-2 && colChar <= '9'; i++){
	        putchar(colChar++);
        	putchar(' ');
	}
	putchar('\n');
	printf("Mines Left (unmarked) - %d\n", board.mineLeft);
}

int main(int argc, char **argv){
	if(argc==2)
		if(argv[1][0]=='-'&&argv[1][1]=='h'){
			printf("Help page:\n\tControls:\n\t\tm <x> <y> - marks the cell\n\t\tr <x> <y> - reveals the cell\n\t\tq <x> <y> - Quit, wait, args?????? Yes, i am a moron\n\tArgs:\n\t\t-h - display this page\n\t\tif it's not -h, arg 1 is size and arg 2 is seed\n");
			return 1;
		}
	if(argc<3){
		printf("Arguments Expected: Size, Seed (int, int)\n");
		printf("Help page:\n\tControls:\n\t\tm <x> <y> - marks the cell\n\t\tr <x> <y> - reveals the cell\n\t\tq <x> <y> - Quit, wait, args?????? Yes, i am a moron\n\tArgs:\n\t\t-h - display this page\n\t\tif it's not -h, arg 1 is size and arg 2 is seed\n");
		return 1;
	}
	int startTime = (int)time(NULL);
	int size = atoi(argv[1]);
	int seed = atoi(argv[2]);
	if(size == 0 || seed == 0){
		printf("Invalid Values Found, Both Args Have To Be Ints And > Than 1\n");
		return 1;
	}
	int mines = (size*size)/6;
	int quit = 0;
	board_t *board = genBoard(size,seed,mines);
        char selectionx = 0;
      	char option = 0;
       	int selectiony = 0;
	while(!quit){
		printBoard(*board);
		if(board->mineLeft == 0){
			printf("You WON\n");
			quit = 1;
			break;
		}
		selectionx = 0;
		option = 0;
		selectiony = 0;
		scanf("%c %c %d", &option, &selectionx, &selectiony);
		(selectionx >= 'a' && selectionx <= 'z') ? selectionx = selectionx - 'a' : 0;
		(selectionx >= 'A' && selectionx <= 'Z') ? selectionx = selectionx - 'A' + 1 + ('z'-'a') : 0;
		(selectionx >= '0' && selectionx <= '9') ? selectionx = selectionx - '0' + 2 + ('z'-'a') + ('Z'-'A'): 0;
		if(option == 'm' || option == 'M'){
			board->cells[selectiony][selectionx+1].marked = (board->cells[selectiony][selectionx+1].marked) ? 0 : 1;
			board->mineLeft -= (board->cells[selectiony][selectionx+1].bomb) ? 1 : 0;
		}else if(option == 'r' || option == 'R'){
			if(!board->cells[selectiony][selectionx+1].bomb){
				revealFrom(board, selectionx+1, selectiony);
				board->latestReveal[0] = selectionx+1;
				board->latestReveal[1] = selectiony;
			}else{
				printf("You LOST, mines left: %d\n", board->mineLeft);
				quit = 1;
			}
		}else if(option == 'q' || option == 'Q')
			quit=1;
		while ((getchar()) != '\n');
	}
	printf("Time Played: %d seconds.\n", (int)time(NULL) - startTime);
	for(int i = 0; i < board->square_width; i++)
		free(board->cells[i]);
	free(board->cells);
	free(board->latestReveal);
	free(board);
}
