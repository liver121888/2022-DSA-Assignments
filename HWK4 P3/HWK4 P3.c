#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int N, M, size;
int i, j, k;

char cmd[20];

typedef struct operation
{
	int first, second; // first = -1 --> print; first = -2 --> boom (second = day);  first, second > 0 --> merge
	int boomCount;
} Operation;

typedef struct node
{
	int elementCount;
	struct node* groupRoot;
	struct node* parent;
	struct node* leftChild;
	struct node* rightChild;
} Node;

Node** nodeArray;
Operation* commandArray;

void mergeItoJGroup(int i, int j)
{
	Node* iRoot = nodeArray[i]->groupRoot;
	Node* jRoot = nodeArray[j]->groupRoot;

	if (iRoot->elementCount > jRoot->elementCount)
	{
		// swap i and j
		Node* temp = iRoot;
		iRoot = jRoot;
		jRoot = temp;
	}
	// Merge small group i to large group j
	jRoot->elementCount += iRoot->elementCount;
	iRoot->elementCount = 0;
	size--; 
	// Traverse all element in iRoot group 


	
}



void main()
{	
	scanf("%d %d", N, M);

	commandArray = malloc(sizeof(Operation) * ( M+1) );
	commandArray[0].first = -1;

	for (int day = 1; day <= M; day++)
	{
		scanf("%s", cmd);
		switch (cmd[0])
		{
		case 'm': // merge
			scanf("%d %d", &commandArray[day].first, &commandArray[day].second);
			break;
		case 'q': // query
			commandArray[day].first = -1;
			break;
		case 'b': // boom
			commandArray[day].first = -2;
			int boomDay;
			scanf("%d", &boomDay);
			commandArray[day].second = boomDay;

			break;
		}
	}


	// Allocate array memory
	nodeArray = malloc(sizeof(Node*) * N);
	for( int c = 0; c < N; c++)
	{
		// Allocate element memory
		Node* a = malloc(sizeof(Node));
		a->elementCount = 1;
		a->groupRoot = a;
		a->parent = a->leftChild = a->rightChild = 0;
		nodeArray[c] = a;
	}

	// Initially there are N groups
	size = N;

	for (int day = 1; day <= M; day++)
	{
		scanf("%s", cmd);
		
		switch (cmd[0])
		{
		case 'm': // merge
			scanf("%d %d", &i, &j);
			mergeTwoGroups(i - 1, j - 1);
			break;
		case 'q': // query
			printf("%d\n", size);
			break;
		case 'b': // boom
			scanf("%d", &k);
			break;
		}
	}
}