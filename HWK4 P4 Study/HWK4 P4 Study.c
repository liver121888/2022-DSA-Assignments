#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


typedef struct node
{
	int priority;
	int id;
	int time;
	struct node* parent;
	struct node* left;
	struct node* right;
} Node;

 
int N, Q, tenN;
Node* root;
Node* tempNode;
Node* left, * right, *current;
Node** initialArray;

void constructInitialTreap(char lr, Node* parent, int left, int right)
{
	Node* center;
	int idx = (left + right) / 2;
	center = initialArray[idx];
	center->priority = parent->priority - 10;
	center->parent = parent;
	if (lr == 'l') parent->left = center;
	else parent->right = center;
	if (right - left == 1) return;

	if( left < idx ) constructInitialTreap('l', center, left, idx);
	if( idx+1 < right ) constructInitialTreap('r', center, idx + 1, right);
}


Node* GetPthNode(int p)
{
	int count = 1;
	tempNode = root;
	while (count != p)
	{

	}



}
void insertAMachine(p, k)
{
	int count = 1;
	tempNode = root;
	while (count != p)
	{

	}

}



void main()
{
	scanf("%d %d", &N, &Q );
	// Create N nodes associated with random priority
	initialArray = malloc(sizeof(Node*) * N);
	tenN = 10 * N;
	for (int i = 1; i <= N; i++)
	{
		tempNode = malloc(sizeof(Node));
		tempNode->id = i;
		tempNode->left = tempNode->right = tempNode->parent = 0;
		scanf("%d", &tempNode->time);
		initialArray[i - 1] = tempNode;
	}
	int idx = N / 2;
	root = initialArray[idx];
	root->parent = 0;
	root->priority = tenN;
	constructInitialTreap('l', root, 0, idx);
	constructInitialTreap('r', root, idx + 1, N);
	int cmd, p, k, l, r, x, y;
	for (int i = 0; i < Q; i++)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case 1:
			scanf("%d %d", &p, &k);
			insertAMachine(p, k);
			break;
		case 2:
			scanf("%d", &p);
			retireAMachine(p);
			break;
		case 3:
			scanf("%d %d", &l, &r);
			swapMachine(l, r);
			break;
		case 4:
			scanf("%d %d %d %d", &l, &r, &x, &y);
			swapMachineMachine(l, r, x, y);
			break;
		case 5:
			scanf("%d %d %d ", &l, &r, &k);
			truncateTimes(l, r, k);
			break;
		case 6:
			scanf("%d %d", &l, &r);
			printRebootTime(l, r);
			break;
		}
	}


}