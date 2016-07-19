#ifndef GRAPH_H_
#define GRAPH_H_
#include<stdlib.h>
#define lockedArc 0 //value of an edge that has been locked
#define noArc 100 //value of an edge that does not exist
#define stdArc 1 //value of an edge that exists but is not locked

typedef struct vertex{
	int x;
	int y;
	int locked;   //0=not locked 1=locked 2=start or end node
	int edgeInd;  //index of vertex in adjacency matrix
}Vertex;

typedef struct board{
	Vertex *Vertices;
	int **edges;
	int num;
}Board;

Vertex* createVertices(int num, int max_X, int max_Y, int margin);
int** createAdjacencyMatrix(Board *B);
void printBoard(Board *B);
void deleteBoard(Board *B);
void fixVertices(Board *B);

#endif