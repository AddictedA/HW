#undef main //eliminates duplicate main within SDL libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "InstructorGameLogic.h"
//#include "MyGraphFunctions.h" include your graph library here

#define minMaxDepth 2 //how deep is the minmax going aka how smart is your computer

/*These two functions need to be modified */
int executeTurn(int turn, SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B);
int determineWinner();
/* You'll want to add a method for computerPlayerShort */
int executeShort(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B);
int executeCut(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B);
int executeShortCPU(SDL_Renderer *ren, Board *B);
int executeCutCPU(SDL_Renderer *ren, Board *B);


/* The main method may need to be modified to add the selection of Player vs Player or Computer vs Player -- see below */

int main(int argc, char **argv) {
	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;
	SDL_Event e;
	Board B;
	B.num = numNodes;

	int quit = GAME_UNDERWAY; 
	int turn = rand() % 2;

	

	// TODO!!!
	// Add a simple menu here that shows up in the black console window to choose PvP or Computer vs. Player
	int isCutCPU, isShortCPU;
	

	//THE MENU NEED IMPROVEMENT. iT TESTS FOR INPUT BUT IT EXITS IF WRONG.
	//A LOOP IS NEEDED TO GET INPUTS UNTIL SOMETHING VALID

	// Main menu
	printf("Make a choice:\n");
	printf("1) Player versus player\n");
	printf("2) Player versus CPU (Player as Short)\n");
	printf("3) Player versus CPU (Player as Cut)\n");
	printf("4) CPU versus CPU\n");
	printf("Enter your choice: ");

	//I found this one while looking for minimax algortihm.
	//I tought it was interesting so I gave it a try

	int choice;
	if (scanf_s("%i", &choice) != 1 || choice < 1 || choice > 4) {
		printf("Unable to read your input.\n");
		return 1;
	}
	choice--;

	isCutCPU = choice & 0x1;
	isShortCPU = (choice & 0x02) >> 1;

	// Probably don't need to modify anything else below

	initializeGraphicsWindow(&win, &ren);
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // creates a white background for the board
	SDL_RenderClear(ren); //clears the game board to ensure it is plain white
	SDL_RenderPresent(ren); //renders the gameboard to the screen
	
	if (turn){ stringColor(ren, 5, 5, "Short's Turn:", white); } //displays initial turn
	else{ stringColor(ren, 5, 5, "Cut's Turn:", black); }
	
	createAndDrawBoard(ren,&B);//generates random planar graph and draws it
	
	//This is the main loop. 
	//It calls executeTurn once per mouse click until the user quits or someone wins
	while (quit==GAME_UNDERWAY){ 
		while (SDL_PollEvent(&e) != 0) //Loops through event queue
		{
			//User requests quit
			if (e.type == SDL_QUIT) //allows user to x out of program
			{
				quit = USER_QUIT;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) //handles mouse button events
			{
				turn = executeTurn(turn, ren, e.button, &B);
				quit = determineWinner(&B);  //returns 0,2,or 3 as defined above inline with the declaration of quit
			}
		}
		SDL_RenderPresent(ren); //presents changes to the screen that result from the execution of one turn

		if (isCutCPU && turn == 0) {
			executeCutCPU(ren, &B);
			turn = 1;
			if (quit = determineWinner(&B)) break;
			SDL_RenderPresent(ren);
			SDL_Delay(1000);
		}
		else
			if (isShortCPU && turn == 1) {
				executeShortCPU(ren, &B);
				turn = 0;
				if (quit = determineWinner(&B)) break;
				SDL_RenderPresent(ren);
				SDL_Delay(1000);
			}

	}

	// Display who won
	displayWinBanner(ren, quit);

	while ((quit == CUT_WINS) || (quit == SHORT_WINS))  //if there was a winner hold the screen until the game window is closed.
	{
		SDL_RenderPresent(ren); //present changes to the screen

		// wait until the user closes the window
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = USER_QUIT;
			}
		}
	}

	freeGraphicsWindow(&win, &ren);//Terminate SDL and end program
	deleteBoard(&B); //deallocates dynamic memory
	return 0;
}


/*****************************************************************************************************************************/
/********MODIFY THE FOLLOWING TWO FUNCTIONS TO FINISH THE GAME                                                         *******/
/*****************************************************************************************************************************/
/*
//these functions should use your graph library to model the logical game. 
*/

// This method checks to see if someone has won
// Return CUT_WINS, SHORT_WINS, or GAME_UNDERWAY (if noone has won yet)
int determineWinner(Board *B)
{

	//if a path of locked arcs exist from A to B then switch wins. 
	//If there exists no path of remaining arcs from A to B then cut wins

	// We have to return 2 if Switch wins (locked path exists)
	// and 3 if Cut wins (there are no possible path at all from start to finish)

	// First of all, let's allocate some memory for data
	// 0 denotes that vertex was not visited, 
	// 1 - visited,
	// 2 - visited and is in current list.
	int * taken = calloc(B->num, sizeof(int));

	// Estimate ID of start and finish vertex
	int startVertexId, finishVertexId, finished;
	getStartFinish(B, &startVertexId, &finishVertexId);

	// Task 1: check if we have direct route of locked edges from start to finish
	taken[startVertexId] = 2;

	// Make steps for Short unless we can
	while (determineWinner_makeStep(B, taken, 0)) {};

	// Check if finish vertex was taken
	if (taken[finishVertexId] == 1) {
		free(taken);
		return 2;
	}

	// Re-initialize taken array before Task 2
	SDL_memset(taken, 0, sizeof(int) * B->num);

	// Task 2: check if we do not have direct route of no-empty edges from start to finish
	taken[startVertexId] = 2;
	while (determineWinner_makeStep(B, taken, 1)) {};

	// Check if finish vertex was not taken.
	if (taken[finishVertexId] != 1) {
		free(taken);
		return 3;
	}

	// Deallocate memory and return: no winner for now
	free(taken);


	return GAME_UNDERWAY;
}

/*
Simple decision logic to handle each SDL event depending on who's turn it is
Returns CUT_TURN or SHORT_TURN depending on who will go next
*/
int executeTurn(int turn, SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B){

	if (turn){
		return executePlayerShort(ren, e, B);
	}
	else{
		return executePlayerCut(ren, e, B);
	}
}

/*****************************************************************************************************************************/
/********ADD YOUR OWN NEW METHODS HERE                                                                                 *******/
/*****************************************************************************************************************************/


int executeShort(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B){
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
		return 0;
	}
	else{
		return 1;
	}
}


/* returns 1 to switch turn if Cut's turn successfully clicks on an arc that has not been locked by Short.
If the click is successfull the arch is erased*/

int executeCut(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B){
	//determine if mouse click was on an unlocked arc. 
	//if so erase the arc
	//else return 0 to sustain turn;
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
		return 1;
	}
	else{ return 0; }
}

/* returns 0 to switch turn if Swithc's turn successfully clicks on an arc that has not been erased by Cut.
If the click is successfull the arch is locked and redrawn in red*/

int minmaxImplementation(Board * B, int * bestI, int * bestJ, int maxDepth, int isCutMove) {
	// The main core of both CPU players, short and cut
	// The idea is: we simulate the game for some depth and on every step we pick the best move for current player,
	// which is the worst for another player. Previous player should take the best move depending on current player moves
	// and so on.

	// If we have won or reached maximum depth, let's just estimate the length of shortest path for Short
	if (determineWinner(B) || maxDepth <= 0)
		return heuristicsFunction(B);

	// Estimate maximum or minimum heuristics value on current step
	int extremeHeuristics;
	if (isCutMove)
		extremeHeuristics = -1;
	else
		extremeHeuristics = 0x7FFFFFFF;

	int i, j, res_I, res_J;;
	for (i = 0; i < B->num; i++)
		for (j = i + 1; j < B->num; j++)
			if (B->edges[i][j] == stdArc || B->edges[j][i] == stdArc) {
				// Temporarily set this arc to locked / noArc
				if (isCutMove) {
					B->edges[i][j] = noArc;
					B->edges[j][i] = noArc;
				}
				else {
					B->edges[i][j] = lockedArc;
					B->edges[j][i] = lockedArc;
				}

				// Check it again from the position of another player.
				int curHeuristics = minmaxImplementation(B, NULL, NULL, maxDepth - 1, !isCutMove);

				// Revert the arc back
				B->edges[i][j] = stdArc;
				B->edges[j][i] = stdArc;

				// If our heuristics is better (more or less than current dependending on who are we), set it up
				if ((curHeuristics <= extremeHeuristics && !isCutMove) || (curHeuristics >= extremeHeuristics && isCutMove)) {
					extremeHeuristics = curHeuristics;
					res_I = i;
					res_J = j;
				}
			}

	// Return best step if needed (needed only for top recursion level)
	if (bestI != NULL && bestJ != NULL) {
		*bestI = res_I;
		*bestJ = res_J;
	}

	return extremeHeuristics;
}

// CPU execution. Always successful.
int executeShortCPU(SDL_Renderer *ren, Board *B){
	if (determineWinner(B))
		return 0; // Nothing to do

	int i, j;
	minmaxImplementation(B, &i, &j, minMaxDepth, 0); // Estimate position of desired I and J starting with Short

	// The rest is the same as in Basic Execute Short
	B->edges[i][j] = lockedArc;
	B->edges[j][i] = lockedArc;
	lockArc(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
	if (B->Vertices[findIndex(B, i)].locked > 0){
		endNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y);
	}
	else{ drawNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y); }
	if (B->Vertices[findIndex(B, j)].locked > 0){
		endNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
	}
	else{ drawNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y); }

	//the next two lines rotate the turn display...you should not need to chagne this
	stringColor(ren, 5, 5, "Short's Turn:", white);
	stringColor(ren, 5, 5, "Cut's Turn:", black);
	return 0;
}

int executeCutCPU(SDL_Renderer *ren, Board *B){
	if (determineWinner(B))
		return 1; // Nothing to do

	int i, j;
	minmaxImplementation(B, &i, &j, minMaxDepth, 1); // Estimate position of desired I and J starting with Cut

	// The rest is the same as in Basic Execute Cut
	B->edges[i][j] = noArc;
	B->edges[j][i] = noArc;
	eraseArc(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
	if (B->Vertices[findIndex(B, i)].locked > 0){
		endNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y);
	}
	else{ drawNode(ren, B->Vertices[findIndex(B, i)].x, B->Vertices[findIndex(B, i)].y); }
	if (B->Vertices[findIndex(B, j)].locked > 0){
		endNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y);
	}
	else{ drawNode(ren, B->Vertices[findIndex(B, j)].x, B->Vertices[findIndex(B, j)].y); }

	//the next two lines rotate the turn display...you should not need to chagne this
	stringColor(ren, 5, 5, "Cut's Turn:", white);
	stringColor(ren, 5, 5, "Short's Turn:", black);
	return 1;
}

int heuristicsFunction(Board * b) {
	// This function returns Heuristics value for the current board.
	// Heuristics value represents minimum amount of steps for Short to win the game
	// If heuristics is 0, Short is a winner.
	// If heuristics is 0x7FFFFFFF (very large value), then path does not exist at all, i.e. Cut wins the game.
	// Otherwise heuristics represents minimum amount of steps to win (i.e. if Cut would make no moves at all).
	// While Short aims to reduce heuristics value to zero, Cut's goal is to increase it.

	// The idea is simple. We'll use simplified Dijkstra algorithm (assuming that edge length is 1)
	// to find shortest distance between starting and finishing vertex

	// First of all, let's allocate some memory for vertex minimum distance
	int * dist = calloc(b->num, sizeof(int));

	// Estimate ID of start and finish vertex
	int startVertexId, finishVertexId;
	getStartFinish(b, &startVertexId, &finishVertexId);

	// Fill vertex minimum distance array
	int i, j;
	for (i = 0; i < b->num; i++)
		dist[i] = -1;
	dist[startVertexId] = 0;
	int curDist = 0; // Current distance from start

	int finished;
	do {
		// Extend already opened vertices
		int innerFinished;
		do {
			innerFinished = 1;

			// Cycle through every vertex and check if it has non-visited neighbours connected with locked arc
			for (i = 0; i < b->num; i++)
				if (dist[i] == curDist)
					for (j = 0; j < b->num; j++)
						if (b->edges[i][j] == lockedArc && dist[j] == -1) {
							innerFinished = 0; // It can have neighbours too, so we need to make another iteration
							dist[j] = curDist;
						}
		} while (!innerFinished);

		// Now we need to find all the vertices connected through usial edges

		finished = 1;
		for (i = 0; i < b->num; i++)
			if (dist[i] == curDist)
				for (j = 0; j < b->num; j++)
					if (b->edges[i][j] == stdArc && dist[j] == -1) {
						finished = 0; // We need to make another step
						dist[j] = curDist + 1; // Can be reached by making a step
					}

		curDist++;
	} while (!finished);

	// Analyze distance of finishing vertex
	int res;
	if (dist[finishVertexId] == -1)
		res = 0x7FFFFFFF;
	else
		res = dist[finishVertexId];

	free(dist);
	return res;
}

int getStartFinish(Board * b, int * start, int * finish) {
	// Helper function to locate starting vertex ID and finishing ID
	// It is guaranteed that they both exists
	*start = -1;
	*finish = -1;

	int i;
	for (i = 0; *start == -1; i++)
		if (b->Vertices[i].locked == 2)
			*start = i;

	for (i = b->num - 1; *finish == -1; i--)
		if (b->Vertices[i].locked == 2)
			*finish = i;
}

int determineWinner_makeStep(Board * b, int * taken, int isCut) {
	// Helper function to make one step in depth-first algorithm
	// Returns 1 if step is successful, 0 if no possible steps can be made

	// Find first taken vertice in current list
	int curVertex = -1;
	int i;
	for (i = 0; i < b->num; i++)
		if (taken[i] == 2)
			curVertex = i;

	if (curVertex == -1)
		// We have not found any vertex in opened list. All the reachable vertices are already visited
		return 0;

	for (i = 0; i < b->num; i++) {
		// Short check: can go only through locked arcs
		if (b->edges[curVertex][i] == lockedArc && taken[i] == 0 && !isCut)
			taken[i] = 2;
		// Cut check: can go through everything
		if (b->edges[curVertex][i] != noArc && taken[i] == 0 && isCut)
			taken[i] = 2;
	}

	taken[curVertex] = 1;
	return 1;
}







