/*
 * Simple 2D polygon (3 vertex) renderer in text for use in terminals
 * Uses files with syntax :
 * 	v1x v1y v2x v2y v3x v3y colour
 * where colour is a single character to represent the brightness and vnx and vny refers to the coordinate of the n-vertex
 * I am aware my programming is bad, but it works.
 * I am using pnpoly because the Barycentric weights method (no idea what it's called) was pretty cool but it took so long to type out
 * 	I got bored and Ctrl+C and Ctrl+v'd the pnpoly code and called it a day
 * PYTHON REQUIRED, lol
*/

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifdef __unix__
	#define OS_Windows 0
#elif defined(_WIN32) || defined(WIN32)
	#define OS_Windows 1
#endif

#define MAXSIZE 20

typedef struct{
	int a;
	int b;
} Vector2;

typedef struct{
	int a;
	int b;
	int c;
} Vector3;

typedef struct{
	Vector2 Coords[3];
	char Colour;
} Poly;

typedef struct{
	int w;
	int h;
	Poly *Polygons;
	int PolyCount;
} Screen;

int eval(char *expr){
    int sum = 0;
    char numOne[10], numTwo[10];
    int i, j;
    char operator;
    for (i = j = 0; expr[i]>='0' && expr[i]<='9'; i++){
        numOne[i-j] = expr[i];
    }
    numOne[i] = 0;
    sum = atoi(numOne);
    for (j = i; i < strlen(expr);){
        operator = expr[i++];
        for (j = i; expr[i]>='0' && expr[i]<='9'; i++){
            numTwo[i-j] = expr[i];
        }
        numTwo[i-j] = 0;
        switch(operator){
            case '/':
                sum /= atoi(numTwo);
                break;
            case '+':
                sum += atoi(numTwo);
                break;
            case '-':
                sum -= atoi(numTwo);
                break;
            case '*':
                sum *= atoi(numTwo);
                break;
            default:
                errno = 1;
                perror("Unrecognized character");
                abort();
                break;
        }
    }
    return sum;
}

char* itoa(int num){
	char *areturnval = (char *) malloc(17);
	char *returnval = (char *) malloc(17);
	int i = 0;
	while (num != 0){
		int rem = num % 10;
		areturnval[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/10;
	}
	areturnval[i] = 0;
	returnval[i] = 0;
	for(int j = 0; --i > -1;j++){
		returnval[j] = areturnval[i];
	}
	free(areturnval);
	return returnval;
}

int pnpoly(Poly poly, Vector2 point){ // Quite a popular algorithm, modified some of the variables for my purposes, uses ints so some accuracy is lost
	int i, j, c = 0;
	for(i = 0, j = 2; i < 3; j = i++)
		if (((poly.Coords[i].b>point.b) != (poly.Coords[j].b>point.b)) && (point.a < (poly.Coords[j].a-poly.Coords[i].a) * (point.b-poly.Coords[i].b) / (poly.Coords[j].b-poly.Coords[i].b) + poly.Coords[i].a))
       			c = !c;
	return c;
}

Screen initScreen(){ // Defines all of the Screen attributes
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	Screen returnVal = {w.ws_col,w.ws_row,NULL,0};
	return returnVal;
}

void appendPolygon(Poly poly, Screen *screen){ // Increases the memory allocated to the polygons by 1 sizeof(Poly) and sets the new last memory to the new poly
	screen->PolyCount++;		       // Praise be for structs being able to copy between each other	
	screen->Polygons = (Poly *) realloc(screen->Polygons, sizeof(Poly)*screen->PolyCount);
	screen->Polygons[screen->PolyCount-1] = poly;
}

char **createRender(Screen screen){ // Generate the 2d Character Array from the polygons to text
	char **out = (char**) malloc(screen.h * sizeof(char*)); // Initialize the memory for this new 2d array (didn't use arrays because I want to use it later and it wouldn't point)
	for(int i = 0; i < screen.h; i++)
		out[i] = (char*) malloc(screen.w);
	for(int i = 0; i < screen.h; i++) // Now initialize all the values to a space
		for(int j = 0; j < screen.w; j++)
			out[i][j] = ' ';
	for(int i = 0; i < screen.PolyCount; i++) // iterate through all polygons
		for(int outY = screen.h-1, y = 0; y < screen.h; outY--, y++ ) // iterate through all pixels
			for(int x = 0; x < screen.w; x++)
				if(pnpoly(screen.Polygons[i], (Vector2) {x, y})) // set the pixel if it is in side the current polygon
					out[outY][x] = screen.Polygons[i].Colour;
	return out; // WOOP WOOP
}

void render(Screen screen, char** text){
	for(int i = 0; i < screen.h-1; i++) // just iterate through all the 1d arrays and printf them
		printf("%s\n",text[i]); // just realized that these strings have no null terminator, oops! tbh it works good enough and i cant be bothered to take 1 second and add a \0
}

void readAndAdd(char *fileName, Screen *screen){ // read from a file polygon data
	char cache[10], contents[MAXSIZE], color;
	int currentCache = 0;
	int currentVector[] = {0,0};
	int tempCoord[3][2] = {{0,0},{0,0},{0,0}};
	FILE *fptr;
	if((fptr = fopen(fileName,"r")) == NULL){
		printf("Error! opening file");
		exit(1);
	}
	while (fscanf(fptr, "%s", contents) != EOF) { // welcome to hell
		cache[0] = '\0'; cache[1] = '\0'; cache[2] = '\0'; cache[3] = '\0'; cache[4] = '\0'; cache[5] = '\0'; cache[6] = '\0'; cache[7] = '\0'; cache[8] = '\0'; cache[9] = '\0';
		if (contents[0] == 'x'){ // x here means the width of the terminal
			if (contents[1]){
				char* screenw = itoa(screen->w);
				char expression[strlen(contents) + strlen(screenw) + 5];
				int i = 0;
				int j = 0;
				for(int j = i; screenw[i]; i++){
					expression[i-j] = screenw[i];
				}
				for(int j = i; contents[i-j+1]; i++){
					expression[i-j+strlen(screenw)] = contents[i-j+1];
				}
				expression[i-j] = 0;
				free(screenw);
				tempCoord[currentVector[1]][currentVector[0]] = eval(expression);
				currentVector[0]++;
			} else {
				tempCoord[currentVector[1]][currentVector[0]] = screen->w;
				currentVector[0]++;
			}
		} else if (contents[0] == 'y'){ // same as x but for height
			if (contents[1]){
				char* screenh = itoa(screen->h);
				char expression[strlen(contents) + strlen(screenh) + 5];
				int i = 0;
				int j = 0;
				for(int j = i; screenh[i]; i++){
					expression[i-j] = screenh[i];
				}
				for(int j = i; contents[i-j+1]; i++){
					expression[i-j+strlen(screenh)] = contents[i-j+1];
				}
				expression[i-j+1] = 0;
				free(screenh);
				tempCoord[currentVector[1]][currentVector[0]] = eval(expression);
				currentVector[0]++;
                        } else {
                                tempCoord[currentVector[1]][currentVector[0]] = screen->h;
                                currentVector[0]++;
                        }
		} else if (contents[0] > '9' || contents[0] < '0'){ // this means that the character is a "colour character" so it has reached the end of the polygon
			color = contents[0];
			printf("(%d,%d) (%d,%d) (%d,%d) > %c\n", tempCoord[0][0], tempCoord[0][1], tempCoord[1][0], tempCoord[1][1], tempCoord[2][0], tempCoord[2][1], color);
			appendPolygon((Poly) {{{tempCoord[0][0], tempCoord[0][1]},{tempCoord[1][0], tempCoord[1][1]},{tempCoord[2][0], tempCoord[2][1]}},color}, screen);
			currentVector[0] = 0; // reset the position where the polygon data is written to
			currentVector[1] = 0;
		}
		else{
			tempCoord[currentVector[1]][currentVector[0]] = atoi(contents); // the current string is probably an integer, just write to the current coordinate value
			currentVector[0]++;
		}
		if(currentVector[0] > 1){ // if we have reached the end of the vertex, go onto the next field
			currentVector[1]++;
			currentVector[0] = 0;
		}
		if(currentVector[1] > 3){ // if it continues past the last position in tempCoord, just throw an error so it doesn't seg fault
			printf("Error! Syntax Error: colour not specified or more than 3 vertecies specified");
			exit(1);
		}
	}
	fclose(fptr);
}

int main(int argc, char** argv){
	Screen screen = initScreen();
	readAndAdd(argv[1], &screen);
	putchar('\n');
	char **activeRender = createRender(screen);
	render(screen,activeRender);
	for(int i = 0; i < screen.h; i++){
		free(activeRender[i]);
	}
	free(activeRender);
	free(screen.Polygons);
	return 0;
}
