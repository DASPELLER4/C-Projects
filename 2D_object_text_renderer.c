/*
 * Simple 2D polygon (3 vertex) renderer in text for use in terminals
 * Uses files with syntax :
 * 	v1x v1y v2x v2y v3x v3y colour
 * where colour is a single character to represent the brightness and vnx and vny refers to the coordinate of the n-vertex
 * I am aware my programming is bad, but it works.
 * I am using pnpoly because the Barycentric weights method (no idea what it's called) was pretty cool but it took so long to type out
 * 	I got bored and Ctrl+C and Ctrl+v'd the pnpoly code and called it a day
*/

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
		if (contents[0] == 'x'){ // x here means the width of the terminal
			switch(contents[1]){
				case('/'):{
					int i = 2;
					while(contents[i] != '\n' && contents[i] != '\0'){ // find what the value after the operator is
						cache[i-2] = contents[i];
						i++;
					}
					cache[i-1] = '\0'; // terminate the string
					tempCoord[currentVector[1]][currentVector[0]] = screen->w/atoi(cache); // apply the operator
					currentVector[0]++;
					break;
				}
                                case('-'):{
                                        int i = 2;
                                        while(contents[i] != '\n' && contents[i] != '\0'){ // find what the value after the operator is
                                                cache[i-2] = contents[i];
                                                i++;
                                        }
                                        cache[i-1] = '\0'; // terminate the string
                                        tempCoord[currentVector[1]][currentVector[0]] = screen->w-atoi(cache); // apply the operator
                                        currentVector[0]++;
                                        break;
                                }
				default: // the user just wants to use the width alone
					tempCoord[currentVector[1]][currentVector[0]] = screen->w;
					currentVector[0]++;
					break;
				// note i dont have + or * operator, this is because you cant go above the width and multiplying by a decimal doesn't work as i only handle integers
			}
		} else if (contents[0] == 'y'){ // same as x but for height
                        switch(contents[1]){
                                case('/'):{
                                        int i = 2;
                                        while(contents[i] != '\n' && contents[i] != '\0'){ // find what the value after the operator is
                                                cache[i-2] = contents[i];
                                                i++;
                                        }
                                        cache[i-1] = '\0'; // terminate the string
                                        tempCoord[currentVector[1]][currentVector[0]] = screen->h/atoi(cache); // apply the operator
                                        currentVector[0]++;
                                        break;
                                }
                                case('-'):{
                                        int i = 2;
                                        while(contents[i] != '\n' && contents[i] != '\0'){ // find what the value after the operator is
                                                cache[i-2] = contents[i];
                                                i++;
                                        }
                                        cache[i-1] = '\0'; // terminate the string
                                        tempCoord[currentVector[1]][currentVector[0]] = screen->h-atoi(cache); // apply the operator
                                        currentVector[0]++;
                                        break;
                                }
                                default: // the user just wants to use the height alone
                                        tempCoord[currentVector[1]][currentVector[0]] = screen->h;
                                        currentVector[0]++;
                                        break;
				// note i dont have + or * operator, this is because you cant go above the height and multiplying by a decimal doesn't work as i only handle integers
                        }
		} else if (contents[0] > '9' || contents[0] < '0'){ // this means that the character is a "colour character" so it has reached the end of the polygon
			color = contents[0];
			// printf("(%d,%d) (%d,%d) (%d,%d) > %c\n", tempCoord[0][0], tempCoord[0][1], tempCoord[1][0], tempCoord[1][1], tempCoord[2][0], tempCoord[2][1], color);
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