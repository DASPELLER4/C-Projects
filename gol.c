// SIMPLE GAME OF LIFE RENDITION IN C
#include <stdio.h>
#include <stdlib.h>

#define findSurroundingCount(x,y) (old[y-1][x-1]+old[y-1][x]+old[y-1][x+1]+old[y][x-1]+old[y][x+1]+old[y+1][x-1]+old[y+1][x]+old[y+1][x+1])

void seeIfAliveOrDead(char **board, char **old, int x, int y){
	if(board[y][x]){
		if(findSurroundingCount(x, y)<2)
			board[y][x] = 0;
		else if(findSurroundingCount(x, y)>3)
		        board[y][x] = 0;
	}
	else{
		if(findSurroundingCount(x, y)==3)
        		board[y][x] = 1;
	}
}

void calculateBoard(char **board, int sizeOfBoard){
	char **temp=calloc(sizeOfBoard, sizeof(char*));
	for(int i = 0; i < sizeOfBoard; i++)
	        temp[i] = calloc(sizeOfBoard,sizeof(char));
	for(int y = 0; y < sizeOfBoard; y++)
	        for(int x = 0; x < sizeOfBoard; x++)
        	        temp[y][x] = board[y][x];
	for(int y = 1; y < sizeOfBoard-2; y++)
        	for(int x = 1; x < sizeOfBoard-2; x++)
			seeIfAliveOrDead(board, temp, x, y);
	for(int i = 0; i < sizeOfBoard; i++)
	        free(temp[i]);
	free(temp);
}

void printBoard(char **board, int sizeOfBoard){
	for(int iy = 1; iy < sizeOfBoard-2; iy++){
		for(int ix = 1; ix < sizeOfBoard-2; ix++){
			putchar(board[iy][ix]+'0');
			putchar(' ');
		}
		putchar('\n');
	}
}

int main(){
	char **board = calloc(12,sizeof(char*));
	for(int i = 0; i < 12; i++)
		board[i] = calloc(12,sizeof(char)); // actual width is 10, but there are 1 element wide margins so we can reliably look around a point on the matrix
	char image[12][12] = /*IGNORE OUTER EDGES, THEY DO NOT CONTAIN VALUE*/ {\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,1,0,0,0,0,0},\
		{0,0,0,0,0,1,0,0,0,0,0,0},\
		{0,0,0,0,0,1,1,1,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0},\
		{0,0,0,0,0,0,0,0,0,0,0,0}\
	};
	for(int y = 0; y < 12; y++)
		for(int x = 0; x < 12; x++)
			board[y][x] = image[y][x];
	for(int i = 0; i<20; i++){
		system("clear");
		printBoard(board,12);
		calculateBoard(board,12);
		system("sleep 0.5");
	}
	for(int i = 0; i < 12; i++)
        	free(board[i]);
	free(board);
}
