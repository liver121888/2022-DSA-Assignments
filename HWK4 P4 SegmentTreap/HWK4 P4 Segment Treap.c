#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct segTreapNode
{
	int start, end;
	int sum;
	struct segTreapNode* parent;
	struct segTreapNode* left;
	struct segTreapNode* right;
} SegTreapNode;

 
int N, Q;
SegTreapNode* root;

int leftsum, rightsum;

void printRebootTime(SegTreapNode* seg, int start, int end)
{

	if (seg->start == start && seg->end == end)
	{
		leftsum = seg->sum;
		rightsum = 0;
	}
	else if (start > seg->end || end < seg->start )
	{
		return;
	}
	else
	{
		if (seg->start == start && seg->end < end)
		{
			leftsum = seg->sum;
		}
		else if (seg->end == end && seg->start > start)
		{
			rightsum = seg->sum;
		}
		else
		{
			if (seg->left)printRebootTime(seg->left, start, end);
			if (seg->right)printRebootTime(seg->right, start, end);
		}
	}
}

void treeExtend(SegTreapNode* seg)
{
	seg->start++;
	seg->end++;
	if (seg->left) treeExtend(seg->left);
	if (seg->right) treeExtend(seg->right);
}

void insertAMachine(SegTreapNode* seg, int p, int k )
{
	SegTreapNode* temp;

	if (seg->end < p) return;
	if (seg->start > p)
	{
		treeExtend(seg);
	}
	else
	{
		if (seg->end == p)
		{
			// create a new parent node
			SegTreapNode* parent = malloc(sizeof(SegTreapNode));
			parent->parent = seg;
			parent->left = seg->right;
			seg->right = parent;
			SegTreapNode* inserted = malloc(sizeof(SegTreapNode));
			parent->right = inserted;
			parent->end = p;
			parent->start = parent->left->start;

			inserted->parent = parent;
			inserted->left = inserted->right = 0;
			inserted->sum = k;
			inserted->start = inserted->end = p;
			parent->sum = parent->left->sum + parent->right->sum;
			seg->sum = seg->left->sum + seg->right->sum;
			seg->end = seg->right->end;
		}
		else if (seg->start == p)
		{
			// create a new parent node
			SegTreapNode* parent = malloc(sizeof(SegTreapNode));
			parent->parent = seg;
			parent->right = seg->left;
			seg->left = parent;
			SegTreapNode* inserted = malloc(sizeof(SegTreapNode));
			parent->left = inserted;
			parent->start = p;
			parent->end = parent->right->end;

			inserted->parent = parent;
			inserted->left = inserted->right = 0;
			inserted->sum = k;
			inserted->start = inserted->end = p;
			parent->sum = parent->left->sum + parent->right->sum;
			seg->sum = seg->left->sum + seg->right->sum;
			seg->start = seg->parent->start;
		}
		else
		{
			seg->end++;
			seg->sum += k;
			if (seg->left) insertAMachine(seg->left, p, k);
			if (seg->right) insertAMachine(seg->right, p, k);
		}
	}
}

SegTreapNode* array0;
SegTreapNode* array1;
SegTreapNode* root;

void main()
{
	scanf("%d %d", &N, &Q );
	// Create N nodes associated with random priority
	array0 = malloc(sizeof(SegTreapNode) * (N));
	for (int i = 0; i < N; i++)
	{
		array0[i].start = array0[i].end = i+1;
		array0[i].parent = array0[i].left = array0[i].right = 0;
		scanf("%d", &array0[i].sum);
	}
	// create upper nodes orphan 
	int size = N;
	int odd = 0;
	SegTreapNode* orphan = 0;
	do
	{
		if (size % 2 == 1)
		{
			odd = 1;
			if( orphan == 0 ) orphan = &array0[size-1];
			size = size / 2;
			array1 = malloc(sizeof(SegTreapNode) * (size+1));
		}
		else
		{
			odd = 0;
			size = size / 2;
			array1 = malloc(sizeof(SegTreapNode) * (size));
		}
		for (int i = 0; i < size; i++)
		{
			int twoi = 2 * i;
			array1[i].left = &array0[twoi];
			array0[twoi].parent = &array1[i];
			if ( i == size - 1 && odd == 0 && orphan)
			{
				array1[i].right = orphan;
				orphan->parent = &array1[i];
				orphan = 0;
			}
			else
			{
				array1[i].right = &array0[twoi + 1];
				array0[twoi + 1].parent = &array1[i];
			}

			array1[i].start = array1[i].left->start;
			array1[i].end = array1[i].right->end;
			array1[i].sum = array1[i].left->sum + array1[i].right->sum;
		}
		if (odd) size = size + 1;
		array0 = array1;
	} while (size > 1);
	root = &array0[0];


	int cmd, p, k, l, r, x, y;
	for (int i = 0; i < Q; i++)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case 1:
			scanf("%d %d", &p, &k);
			insertAMachine(root, p, k);
			break;
		case 2:
			scanf("%d", &p);
		//	retireAMachine(p);
			break;
		case 3:
			scanf("%d %d", &l, &r);
	//		swapMachine(l, r);
			break;
		case 4:
			scanf("%d %d %d %d", &l, &r, &x, &y);
		//	swapTwoBlocks(l, r, x, y);
			break;
		case 5:
			scanf("%d %d %d ", &l, &r, &k);
	//		truncateTimes(l, r, k);
			break;
		case 6:
			scanf("%d %d", &l, &r);
			leftsum = -1;
			rightsum = -1;
			printRebootTime(root, l, r);
			printf("%d\n", leftsum + rightsum);
			break;
		}
	}


}