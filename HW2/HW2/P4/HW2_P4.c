#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_SIZE 3000000
#include <stdio.h>
#include <stdlib.h>
#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)

// References:
// Ref: https://www.programiz.com/dsa/graph-adjacency-list
// For the building of adjacency list to store the info of city connection
// Ref: https://www.geeksforgeeks.org/lca-n-ary-tree-constant-query-o1/
// For finding the LCA of two nodes


typedef struct node {
	struct node* next;
	int id;
}node;

typedef struct graph {
	int nodenumber;
	int* visited;
	struct node** adjlists;
}graph;

int FileFlag = 1;
int DebugFlag = 1;
int re;

int CityNumber, QueryNumber, CapitalNumber, ResortNumber;
int CityA, CityB, CityC;
int Euler[ARRAY_SIZE];
int Depth[ARRAY_SIZE];
int FirstIndex[ARRAY_SIZE];
int *MinDepthCity;
int EulerStamp = 0;

graph* citygraph;

void connect(int origin, int destination)
{
	node* no = malloc(sizeof(node));
	no->id = origin;
	node* nd = malloc(sizeof(node));
	nd->id = destination;

	no->next = citygraph->adjlists[destination];
	citygraph->adjlists[destination] = no;
	
	nd->next = citygraph->adjlists[origin];
	citygraph->adjlists[origin] = nd;
}

void printCity()
{
	for (int i = 0; i < citygraph->nodenumber; i++) {
		node* tmp = citygraph->adjlists[i];
		// plus 1 to cope with the index and the numbering difference
		printf("City %d: \n", i+1);
		while (tmp) 
		{
			printf("%d -> ", tmp->id+1);
			tmp = tmp->next;
		}
		printf("\n");
	}
}

void buildTree(int here, int previous, int d)
{
	struct node* adjList = citygraph->adjlists[here];
	struct node* temp = adjList;
	if (FirstIndex[here] == -1)
		FirstIndex[here] = EulerStamp;
	Depth[EulerStamp] = d;
	Euler[EulerStamp] = here;
	EulerStamp++;

	if (DebugFlag)
	{
		printf("Visited %d \n", here + 1);
	}
	
	citygraph->visited[here] = 1;

	temp = temp->next;
	while (temp != NULL) {
		int neighbor = temp->id;
		if (citygraph->visited[neighbor] == 0) {
			citygraph->visited[neighbor] = 1;
			buildTree(neighbor, here, d+1);
			Euler[EulerStamp] = here;
			Depth[EulerStamp] = d;
			EulerStamp++;
		}
		temp = temp->next;
	}
}

void CalcminDepth()
{
	// minus 1 to cope with the index and the numbering difference
	int ri = FirstIndex[ResortNumber - 1];
	// minus 1 to cope with the index and the numbering difference
	MinDepthCity[ResortNumber - 1] = ResortNumber - 1;

	int dmin = 2147483647;
	int dminindex = 0;
	// look after for Resort
	for (size_t i = ri; i < ARRAY_SIZE; i++)
	{		
		if (Depth[i] == -1)
			break;
		if (Depth[i] < dmin)
		{
			dmin = Depth[i];
			dminindex = i;
		}
		MinDepthCity[Euler[i]] = Euler[dminindex];
	}
	// reset dmin
	dmin = 2147483647;
	// look before for Resort
	for (int i = ri; i >=0; i--)
	{
		if (Depth[i] < dmin)
		{
			dmin = Depth[i];
			dminindex = i;
		}
		MinDepthCity[Euler[i]] = Euler[dminindex];
	}
}

void printArray(int* p, int plus)
{
	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		if (p[i] != -1)
		{
			if(plus == 1)
				printf("%d, ", p[i] + 1);
			else
				printf("%d, ", p[i]);
		}
		else
		{
			break;
		}
	}
	printf("\n");
}


int main()
{
	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW2\\HW2\\hw2_testdata\\P4\\2.in", "r");
		re = fscanf(ptr, "%d %d %d %d", &CityNumber, &QueryNumber, &CapitalNumber, &ResortNumber);
	}
	else
		re = scanf("%d %d %d %d", &CityNumber, &QueryNumber, &CapitalNumber, &ResortNumber);

	citygraph = malloc(sizeof(graph));
	citygraph->nodenumber = CityNumber;
	citygraph->adjlists = malloc(CityNumber * sizeof(node*));
	citygraph->visited = malloc(CityNumber * sizeof(int));
	MinDepthCity = malloc(sizeof(int) * CityNumber);

	for (int i = 0; i < CityNumber; i++)
	{
		citygraph->adjlists[i] = NULL;
		citygraph->visited[i] = 0;
		MinDepthCity[i] = -1;
	}
	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		Euler[i] = -1;
		Depth[i] = -1;
		FirstIndex[i] = -1;
	}
	int start, end;
	for (int k = 0; k < CityNumber-1; k++)
	{
		if (FileFlag)
			re = fscanf(ptr, "%d %d", &start, &end);
		else
			re = scanf("%d %d", &start, &end);
		// minus 1 to cope with the index and the numbering difference
		start--;
		end--;
		connect(start, end);
	}
	// at the start of every list, add itself
	for (int k = 0; k < CityNumber; k++)
	{
		node* nself = malloc(sizeof(node));
		nself->id = k;
		nself->next = citygraph->adjlists[k];
		citygraph->adjlists[k] = nself;
	}

	if (DebugFlag)
	{
		printCity();
	}

	// minus 1 to cope with the index and the numbering difference
	int Re = ResortNumber - 1;
	int Ca = CapitalNumber - 1;

	// use capital as root
	buildTree(Ca, -1, 0);

	if (DebugFlag)
	{
		printf("Euler: \n");
		printArray(Euler, 1);
		printf("Depth: \n");
		printArray(Depth, 0);
		printf("FirstIndex: \n");
		printArray(FirstIndex, 0);
	}

	CalcminDepth();

	int q;
	for (int k = 0; k < QueryNumber; k++)
	{
		if (FileFlag)
			re = fscanf(ptr, "%d", &q);
		else
			re = scanf("%d", &q);
		// minus 1 to cope with the index and the numbering difference
		q--;
		// plus 1 to cope with the index and the numbering difference
		printf("%d\n", MinDepthCity[q] + 1);
	}


	if (FileFlag && ptr != NULL)
	{
		fclose(ptr);
	}
	return 0;
}
#pragma   warning(pop)  