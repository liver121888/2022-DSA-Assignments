#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

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
int* MinDepthCity;
int EulerStamp = 0;

graph* citygraph;


int main()
{
	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW3\\HW3\\hw3_testdata\\P5\\0.in", "r");
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

	int start, end;
	for (int k = 0; k < CityNumber - 1; k++)
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