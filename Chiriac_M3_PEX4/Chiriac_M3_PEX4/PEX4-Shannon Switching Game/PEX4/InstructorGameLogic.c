#include "InstructorGameLogic.h"
#include <stdio.h>

#define clickTolerance 5.0
#define rad 5
#define margin 10

/*Creates the planar graph used to play the game. Game board MUST have more than five Vertices*/
void createAndDrawBoard(SDL_Renderer *ren, Board *B){
	//write this function to generate a random graph with an A and B node for the switching game. 
	//Use the drawNode and drawArc functions to add the graphical representations to the board.
	//You need to determine a mechanism for tracking which arcs
	B->Vertices = createVertices(numNodes, width, height, margin);
	B->edges = createAdjacencyMatrix(B);
	fixVertices(B);
	char s[8];
	for (int i = 0; i < B->num; i++)
	{
		sprintf_s(s, 8, "%d", B->Vertices[i].edgeInd);
		if (B->Vertices[i].locked == 2){
			endNode(ren, B->Vertices[i].x, B->Vertices[i].y);
		}
		else{
			drawNode(ren, B->Vertices[i].x, B->Vertices[i].y);
		}
		stringColor(ren, B->Vertices[i].x, B->Vertices[i].y + 10, s, black);

	}

	for (int i = 0; i < B->num; i++){
		for (int j = 0; j < B->num; j++){
			if (B->edges[i][j] == stdArc){
				if (findIndex(B, i) != i) {
					printf("Help me!\n");
					exit(1);
				}
				drawArc(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
			}
		}
	}




}

/*Shuts down SDL processes at program's end.*/
void freeGraphicsWindow(SDL_Window **win, SDL_Renderer **ren)
{
	SDL_DestroyRenderer(*ren);
	SDL_DestroyWindow(*win);
	SDL_Quit();
}
/*Sets up the SDL window and renderer to process graphics via the GPU.
If SDL initiation fails program is automatically exited with arror code 1. SDL error is sent to stderr.*/
void initializeGraphicsWindow(SDL_Window **win, SDL_Renderer **ren)
{
	*win = SDL_CreateWindow("Shannon Switching Game!", 100, 100, width, height, SDL_WINDOW_SHOWN);
	if (*win == NULL){
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	*ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (*ren == NULL){
		SDL_DestroyWindow(*win);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
}

/* returns 1 if the detected mouse click is within the distance tolerance of a drwan arc */
int arcClicked(int x1, int y1, int x2, int y2, SDL_MouseButtonEvent e){
	double d = 10000.0;

	d = abs((y2 - y1)*e.x - (x2 - x1)*e.y + x2*y1 - y2*x1) / sqrt((y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1));

	if ((e.x<min(x1, x2)) || (e.x>max(x1, x2))){
		return 0;
	}
	if ((e.y<min(y1, y2)) || (e.y>max(y1, y2))){
		return 0;
	}

	if (d <= clickTolerance){ return 1; }
	return 0;
}
/*Draws an line between two points. Points should be the center points of two nodes */
void drawArc(SDL_Renderer *ren, int x1, int y1, int x2, int y2){
	lineColor(ren, x1, y1, x2, y2, black);
}
/*Erases the line between two points. Points should be the center points of two nodes
redraws nodes to eliminate white fractures - if erasing nodes erase nodes after erasing arc*/
void eraseArc(SDL_Renderer *ren, int x1, int y1, int x2, int y2){
	lineColor(ren, x1, y1, x2, y2, white);

}
/*Redraws the line between two points in red. Points should be the center points of two nodes
redraws nodes to eliminate red fractures - if locking nodes lock nodes after locking arc*/
void lockArc(SDL_Renderer *ren, int x1, int y1, int x2, int y2){
	lineColor(ren, x1, y1, x2, y2, red);

}
/*Draws a node as a black circle*/
void drawNode(SDL_Renderer *ren, int x, int y){
	filledCircleColor(ren, x, y, rad, black);
}
/*Draws a node as a red circle*/
void lockNode(SDL_Renderer *ren, int x, int y){
	filledCircleColor(ren, x, y, rad, red);
}
/*Draws end nodes*/

void endNode(SDL_Renderer *ren, int x, int y){
	//filledCircleRGBA(ren, x, y, rad, 0, 0, 255, 1);
	filledCircleColor(ren, x, y, rad, green);
}
/*erases a node by drawing a white circle*/
void eraseNode(SDL_Renderer *ren, int x, int y){
	filledCircleColor(ren, x, y, rad, white);
}
/*displays the winner where winner=2 => "Switch wins" winner=3 => "Cut wins" */
void displayWinBanner(SDL_Renderer *ren, int winner){
	//SDL_RenderClear(ren);
	stringColor(ren, 5, 5, "Short's Turn:", white);
	stringColor(ren, 5, 5, "Cut's Turn:", white);
	if (winner == SHORT_WINS){ 
		stringColor(ren, 5, 5, "Congratulations Short Wins!", black);
		stringColor(ren, (Sint16)(width / 2.75), height / 2, "GAME OVER - Short Wins", black);
	}
	else if (winner == CUT_WINS){ 
		stringColor(ren, 5, 5, "Congratulations Cut Wins!", black);
		stringColor(ren, (Sint16)(width / 2.75), height / 2, "GAME OVER - Cut Wins", black);
	}

}

/*****************************************************************************************************************************/
/********THESE METHODS SHOULD NOT REQUIRE MODIFICATION                                                                 *******/
/*****************************************************************************************************************************/


/* executePlayerShort handles a click during Short's turn

returns CUT_TURN to switch turn if Short's turn successfully clicks on an arc that has not been erased by Cut.
Otherwise, returns SHORT_TURN so that it will try again.
If the click is successful the arc is locked and redrawn in red*/
int executePlayerShort(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B){
	//determine if mouse click was on an unlocked arc. 
	//if so lock the arc
	//else return 1 to sustain turn;

	int success = 0;
	for (int i = 0; i < B->num; i++)
	{
		for (int j = 0; j < B->num; j++)
		{
			if ((B->edges[i][j] == stdArc) || (B->edges[j][i] == stdArc)) //if edge exists test distance
			{
				if (arcClicked(B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y, e)){
					B->edges[i][j] = lockedArc;
					B->edges[j][i] = lockedArc;
					lockArc(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
					if (B->Vertices[findIndex(B, i)].locked>0){
						endNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y);
					}
					else{ drawNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y); }
					if (B->Vertices[findIndex(B, j)].locked>0){
						endNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
					}
					else{ drawNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y); }

					success = 1;
					j = B->num;
					i = B->num;
				}
			}
		}
	}
	//the next two lines rotate the turn display...you should not need to chagne this
	if (success){
		stringColor(ren, 5, 5, "Short's Turn:", white);
		stringColor(ren, 5, 5, "Cut's Turn:", black);
		return CUT_TURN;
	}
	else{
		return SHORT_TURN;
	}
}


/* executePlayerCut handles a click during Cut's turn

returns SHORT_TURN to switch turn if Cut's turn successfully clicks on an arc that has not been locked by Short.
returns CUT_TURN on invalid click so player can try again
If the click is successful the arc is erased*/

int executePlayerCut(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B){
	//determine if mouse click was on an unlocked arc. 
	//if so erase the arc
	//else return CUT_TURN to sustain turn;
	int success = 0;
	for (int i = 0; i < B->num; i++)
	{
		for (int j = 0; j < B->num; j++)
		{
			if ((B->edges[i][j] == stdArc) || (B->edges[j][i] == stdArc)) //if edge exists test distance
			{
				if (arcClicked(B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y, e)){
					B->edges[i][j] = noArc;
					B->edges[j][i] = noArc;
					eraseArc(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
					success = 1;
					if (B->Vertices[findIndex(B, i)].locked>0){
						endNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y);
					}
					else{ drawNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y); }
					if (B->Vertices[findIndex(B, j)].locked>0){
						endNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
					}
					else{ drawNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y); }
					j = B->num;
					i = B->num;
				}
			}
		}
	}

	//the next two lines rotate the turn display...you should not need to chagne this
	if (success){
		stringColor(ren, 5, 5, "Cut's Turn:", white);
		stringColor(ren, 5, 5, "Short's Turn:", black);
		return SHORT_TURN;
	}
	else{ return CUT_TURN; }
}