#ifndef INSTRUCTOR_GAME_LOGIC_H
#define INSTRUCTOR_GAME_LOGIC_H

#include "../SDL2-devel-2.0.3-VC/SDL2-2.0.3/include/SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL2_gfxPrimitives_font.h"
#include "Graph.h"
#undef main //eliminates duplicate main within SDL libraries

#define GAME_UNDERWAY 0
#define USER_QUIT 1
#define SHORT_WINS 2
#define CUT_WINS 3

#define CUT_TURN 0
#define SHORT_TURN 1

#define numNodes 6//this number can be lowered for troubleshooting...however it MUST BE > 5
#define width 1000
#define height 500
#define black 0xFF000000
#define red 0xFF0000FF
#define green 0xFF00AA00
#define white 0xFFFFFFFF

/* executePlayerShort handles a click during Short's turn

returns CUT_TURN to switch turn if Short's turn successfully clicks on an arc that has not been erased by Cut.
Otherwise, returns SHORT_TURN so that it will try again.
If the click is successful the arc is locked and redrawn in red*/
int executePlayerShort(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B);
/* executePlayerCut handles a click during Cut's turn

returns SHORT_TURN to switch turn if Cut's turn successfully clicks on an arc that has not been locked by Short.
returns CUT_TURN on invalid click so player can try again
If the click is successful the arc is erased*/
int executePlayerCut(SDL_Renderer *ren, SDL_MouseButtonEvent e, Board *B);

/*These are functions have been provided to give your program basic graphical capability.
You should not need to modify these functions. More detailed descritions can be found in
the comments precedeing the function definitions below */
void createAndDrawBoard(SDL_Renderer *ren, Board *B);
void initializeGraphicsWindow(SDL_Window **win, SDL_Renderer **ren);
void freeGraphicsWindow(SDL_Window **win, SDL_Renderer **ren);
int arcClicked(int x1, int y1, int x2, int y2, SDL_MouseButtonEvent e);
void drawArc(SDL_Renderer *ren, int x1, int y1, int x2, int y2);
void eraseArc(SDL_Renderer *ren, int x1, int y1, int x2, int y2);
void lockArc(SDL_Renderer *ren, int x1, int y1, int x2, int y2);
void drawNode(SDL_Renderer *ren, int x, int y);
void endNode(SDL_Renderer *ren, int x, int y);
void eraseNode(SDL_Renderer *ren, int x, int y);
void lockNode(SDL_Renderer *ren, int x, int y);
void displayWinBanner(SDL_Renderer *ren, int winner);

#endif