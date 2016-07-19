#include "Graph.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

#define minDistance 75.0 //minimum distance between two points



//used to calculate the distance between a vertex and a set of coordinates
//Function is used in the creation of the game board
double distance(Vertex V, double x, double y)
{
	double x1, y1;
	x1 = (double)V.x - x;
	y1 = (double)V.y - y;
	x1 = (x1*x1) + (y1*y1);
	return sqrt(x1);
}

//used for diagnositcs only. Will print the Vertices and adjecency matrix to stdout
void printBoard(Board *B)
{
	for (int i = 0; i < B->num; i++){
		printf("VERTEX %d:   X=%d    Y=%d  LOCKED=%d\n", B->Vertices[i].edgeInd, B->Vertices[i].x, B->Vertices[i].y, B->Vertices[i].locked);
	}

	for (int i = 0; i < B->num; i++){
		printf("FROM %d   TO: ", i);
		for (int j = 0; j < B->num; j++)
		{
			printf(" %d", B->edges[i][j]);
			if ((B->edges[i][j] == stdArc) || (B->edges[i][j] == lockedArc)){
				printf(" %d", j);
			}
		}
		printf("\n");
	}
}

//returns 1 if a new point (j,k) is too close to Vertex tmp.  
//Function is used in the creation of the game board
int tooClose(int j, int k, Vertex *tmp)
{
	double x = (double)tmp[j].x - (double)tmp[k].x;
	double y = (double)tmp[j].y - (double)tmp[k].y;
	if (sqrt(x*x + y*y) >= minDistance){ return 0; }

	return 1;
}

// the next two methods are a merge sort algorithm to sort the vertex list by distance from a point (x,y)
//Functions are used in the creation of the game board
void merge(Vertex *V1, Vertex *V2, int n1, int n2, double x, double y){
	int i = 0;
	int j = 0;
	int k = 0;
	Vertex *tmp = (Vertex *)malloc(sizeof(Vertex)*(n1 + n2 + 2));
	while ((i <= n1) && (j <= n2)){
		if (distance(V1[i], x, y) > distance(V2[j], x, y))
		{
			tmp[k].locked = V2[j].locked;
			tmp[k].x = V2[j].x;
			tmp[k].y = V2[j].y;
			tmp[k].edgeInd = V2[j].edgeInd;
			j++;
			k++;
		}
		else{
			tmp[k].locked = V1[i].locked;
			tmp[k].x = V1[i].x;
			tmp[k].y = V1[i].y;
			tmp[k].edgeInd = V1[i].edgeInd;

			i++;
			k++;
		}

	}
	while (i <= n1)
	{
		tmp[k].locked = V1[i].locked;
		tmp[k].x = V1[i].x;
		tmp[k].y = V1[i].y;
		tmp[k].edgeInd = V1[i].edgeInd;

		i++;
		k++;

	}

	while (j <= n2)
	{
		tmp[k].locked = V2[j].locked;
		tmp[k].x = V2[j].x;
		tmp[k].y = V2[j].y;
		tmp[k].edgeInd = V2[j].edgeInd;
		j++;
		k++;
	}
	for (i = 0; i <= n1 + n2 + 1; i++)
	{
		V1[i].locked = tmp[i].locked;
		V1[i].x = tmp[i].x;
		V1[i].edgeInd = tmp[i].edgeInd;
		V1[i].y = tmp[i].y;
	}
	free(tmp);
}
void mergeSort(Vertex *V, int max, double x, double y)
{
	if (max < 1)
	{
		return;
	}

	Vertex *T1, *T2;
	int N1, N2;
	N1 = max / 2;
	N2 = max - N1 - 1;
	T1 = V;
	T2 = &V[N1 + 1];
	mergeSort(T1, N1, x, y);
	mergeSort(T2, N2, x, y);
	merge(T1, T2, N1, N2, x, y);
}

//returns a pointer to an array of Vertices. Takes the number of Vertices to be created,
//the max dimensions of the board and the margin on the sides of the board as arguments
Vertex* createVertices(int num, int max_X, int max_Y, int margin){
	Vertex *tmp = NULL;
	if (num < 2){ return NULL; }
	tmp = (Vertex *)malloc(sizeof(Vertex)*num);
	if (tmp == NULL){ return tmp; }
	time_t t;
	int dis = 1;
	srand((unsigned)time(&t));
	tmp[0].locked = 2;
	tmp[0].x = rand() % 50 + margin;
	tmp[0].y = rand() % 50 + 2 * margin;
	tmp[0].edgeInd = 0;
	while (dis){
		tmp[1].locked = 2;
		tmp[1].x = max_X - rand() % 50 - margin;
		tmp[1].y = max_Y - rand() % 50 - margin;
		tmp[1].edgeInd = num - 1;
		dis = tooClose(0, 1, tmp);
	}


	for (int i = 2; i < num; i++)
	{
		dis = 1;
		while (dis){
			tmp[i].locked = 0;
			tmp[i].x = (max_X - margin) - (rand() % (max_X - 2 * margin));
			tmp[i].y = (max_Y - margin) - (rand() % (max_Y - 2 * margin));
			tmp[i].edgeInd = i - 1;
			for (int j = 0; j < i; j++){
				dis = tooClose(j, i, tmp);
				if (dis){ j=i; }
			}
		}
	}

	return tmp;
}

//returns the degree of vertex i in board B. (how many arcs leave vertex i)
//used to genereate the adjacency matrix
int degree(Board *B, int i){
	int count = 0;
	for (int j = 0; j < B->num; j++)
	{
		if (B->edges[i][j]<noArc){ count++; }
	}
	return count;
}

//returns the index of a vertex in the vertex list given the inted in the adjacency matrix.
int findIndex(Board *B, int x)
{
	for (int j = 0; j < B->num; j++)
	{
		if (B->Vertices[j].edgeInd == x){
			return j;
		}
	}
	return -1;
}

void fixVertices(Board *B) {
	Vertex *oldVertices;
	Vertex *newVertices = (Vertex *)malloc(sizeof(Vertex)*B->num);
	for (int i = 0; i < B->num; i++) {
		newVertices[B->Vertices[i].edgeInd] = B->Vertices[i];
	}
	oldVertices = B->Vertices;
	B->Vertices = newVertices;
	free(oldVertices);
}

//returns 1 if two line segments cross, otherwise it returns 0. takes four coodinates as arguments
//the firste two x,y pairs are the start and end of segment one and the second two x,y pairs are the start and end of the second segment
int lineSegmentIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy) {

	double X = 0.0; double Y = 0.0;
	double  distAB, theCos, theSin, newX, ABpos;

	//  Fail if either line segment is zero-length.
	if (Ax == Bx && Ay == By || Cx == Dx && Cy == Dy) return 0;

	//  Fail if the segments share an end-point.
	if (Ax == Cx && Ay == Cy || Bx == Cx && By == Cy
		|| Ax == Dx && Ay == Dy || Bx == Dx && By == Dy) {
		return 0;
	}

	//  (1) Translate the system so that point A is on the origin.
	Bx -= Ax; By -= Ay;
	Cx -= Ax; Cy -= Ay;
	Dx -= Ax; Dy -= Ay;

	//  Discover the length of segment A-B.
	distAB = sqrt(Bx*Bx + By*By);

	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos = Bx / distAB;
	theSin = By / distAB;
	newX = Cx*theCos + Cy*theSin;
	Cy = Cy*theCos - Cx*theSin; Cx = newX;
	newX = Dx*theCos + Dy*theSin;
	Dy = Dy*theCos - Dx*theSin; Dx = newX;

	//  Fail if segment C-D doesn't cross line A-B.
	if (Cy < 0. && Dy < 0. || Cy >= 0. && Dy >= 0.) return 0;

	//  (3) Discover the position of the intersection point along line A-B.
	ABpos = Dx + (Cx - Dx)*Dy / (Dy - Cy);

	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if (ABpos<0. || ABpos>distAB) return 0;

	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	X = Ax + ABpos*theCos;
	Y = Ay + ABpos*theSin;

	//  Success.
	return 1;
}

//performs testing to see if a possible new edge overlaps any existing edges
//function is used in creating the adjacency matrix for the board
int overlaps(Board *B, int start, int end)
{
	int x1, y1;
	int x2, y2;
	int indS = findIndex(B, start);  //extract vertex list indexes
	int indE = findIndex(B, end);
	x1 = B->Vertices[indS].x;
	y1 = B->Vertices[indS].y;
	x2 = B->Vertices[indE].x;
	y2 = B->Vertices[indE].y;
	int x3 = 0;
	int y3 = 0;
	int x4 = 0;
	int y4 = 0;
	int indTS;
	int indTE;

	for (int i = 0; i < B->num; i++) //loop through edges starts
	{
		for (int j = 0; j < B->num; j++) //loop through edge ends
		{
			indTS = findIndex(B, i); //find index in vertex list
			indTE = findIndex(B, j);
			if ((indS != indE) && (indTS != indTE)){ //edge does not start and end on same node
				if (!((B->edges[start][end] != noArc) || (B->edges[end][start] != noArc))) //edge does not already exists
				{
					if (!(((indTS == indS) && (indTE == indE)) || ((indTE == indS) && (indTS == indE)))){//ensure there is no tests for same edge
						if (((B->edges[i][j] != noArc) || (B->edges[j][i] != noArc))){ //ensure an edge exists between two nodes before looking to calc intersect
							x3 = B->Vertices[indTS].x; //extract coordinates of line segment for test edge
							y3 = B->Vertices[indTS].y;
							x4 = B->Vertices[indTE].x;
							y4 = B->Vertices[indTE].y;
							if (lineSegmentIntersection((double)x1, (double)y1, (double)x2, (double)y2, (double)x3, (double)y3, (double)x4, (double)y4)){//determine if two segments intersect
								return 1;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

//Creates a random number of edges from Vertex edgeInd within the adjacency matrix 
void createEdges(Board *B, int edgeInd){
	int numEdges = 0;
	numEdges = rand() % 3 +3; //random number of edges from 3-5
	//numEdges = 3; //random number of edges from 1-3
	int j = findIndex(B, edgeInd);

	mergeSort(B->Vertices, B->num - 1, (double)B->Vertices[j].x, (double)B->Vertices[j].y);

	for (int i = 1; i <= numEdges; i++)
	{
		if (degree(B, B->Vertices[i].edgeInd) < 5){
			if (!overlaps(B, edgeInd, B->Vertices[i].edgeInd)){
				B->edges[edgeInd][B->Vertices[i].edgeInd] = stdArc;
				B->edges[B->Vertices[i].edgeInd][edgeInd] = stdArc;
			}
		}
	}
}

//Creates the adjacency matrix for the game board. The game board is a planar graph 
int** createAdjacencyMatrix(Board *B){
	int start, end;
	start = -1;
	end = -1;
	B->edges = NULL;
	mergeSort(B->Vertices, B->num - 1, 0.0, 0.0);


	B->edges = (int**)malloc(sizeof(int*)*B->num);
	for (int i = 0; i < B->num; i++)
	{
		B->edges[i] = (int*)malloc(sizeof(int)*B->num);
		for (int j = 0; j < B->num; j++){
			B->edges[i][j] = noArc;
		}
		if (B->Vertices->locked == 2){
			if (start != -1){
				end = i;
			}
			else{ start = i; }
		}
	}

	for (int i = 0; i < B->num; i++)//loop down the ajacency matrix to create edges for each vertex
	{
		createEdges(B, i);

	}
	return B->edges;
}

//deallocates space for all dynamic elements of the game board.
void deleteBoard(Board *B)
{
	if (B->edges != NULL){
		for (int i = 0; i < B->num; i++)
		{
			free(B->edges[i]);
		}
		free(B->edges);
	}
	free(B->Vertices);
}


