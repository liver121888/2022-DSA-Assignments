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
SegTreapNode* root, * temp;

int leftsum, rightsum;
int rangeStart;
int rangeEnd;
int total;
int timeLimit;
int trimedAmount;

void trimDownSegmentServers(SegTreapNode* stop, SegTreapNode* seg)
{
	if (seg->start == seg->end)
	{
		if (seg->sum > timeLimit)
		{
			int delta = seg->sum - timeLimit;
			trimedAmount += delta;
			// change sum and update parents
			temp = seg;
			while (temp != stop)
			{
				temp->sum -= delta;
				temp = temp->parent;
			}
		}
	}
	else
	{
		if (seg->left) trimDownSegmentServers(stop, seg->left);
		if (seg->right) trimDownSegmentServers(stop,seg->right);
	}
}

void truncateTimes(SegTreapNode* seg)
{

	if (seg->start == rangeStart && seg->end == rangeEnd)
	{
		// One body block (or the complete block, or head+first body, or last body +tail) is found

		trimedAmount = 0;
		trimDownSegmentServers(seg, seg);
		// update parents
		if (trimedAmount > 0)
		{
			temp = seg;
			while (temp)
			{
				temp->sum -= trimedAmount;
				temp = temp->parent;
			}
		}
	}
	else if (rangeStart > seg->end || rangeEnd < seg->start)
	{
		return;
	}
	else
	{
		if (seg->start == rangeStart && seg->end < rangeEnd)
		{
			// Head block found
			trimedAmount = 0;
			trimDownSegmentServers(seg, seg);
			// update parents
			if (trimedAmount > 0)
			{
				temp = seg;
				while (temp)
				{
					temp->sum -= trimedAmount;
					temp = temp->parent;
				}
			}

			// update search range
			rangeStart = seg->end + 1;

		}
		else if (seg->end == rangeEnd && seg->start > rangeStart)
		{
			// Tail block found
			trimedAmount = 0;
			trimDownSegmentServers(seg, seg);
			// update parents
			if (trimedAmount > 0)
			{
				temp = seg;
				while (temp)
				{
					temp->sum -= trimedAmount;
					temp = temp->parent;
				}
			}

			// update search range
			rangeEnd = seg->start - 1;
		}
		else
		{
			if (seg->left)truncateTimes(seg->left);
			if (seg->right)truncateTimes(seg->right);
		}
	}
}


void printRebootTime(SegTreapNode* seg )
{

	if (seg->start == rangeStart && seg->end == rangeEnd)
	{
		// One body block (or the complete block, or head+first body, or last body +tail) is found
		total += seg->sum;
	}
	else if (rangeStart > seg->end || rangeEnd < seg->start )
	{
		return;
	}
	else
	{
		if (seg->start == rangeStart && seg->end < rangeEnd)
		{
			// Head block found
			total += seg->sum;
			// update search range
			rangeStart = seg->end + 1;
		}
		else if (seg->end == rangeEnd && seg->start > rangeStart)
		{
			// Tail block found
			total += seg->sum;
			// update search range
			rangeEnd = seg->start - 1;
		}
		else
		{
			if (seg->left)printRebootTime(seg->left );
			if (seg->right)printRebootTime(seg->right );
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

void treeReduced(SegTreapNode* seg)
{
	seg->start--;
	seg->end--;
	if (seg->left) treeReduced(seg->left);
	if (seg->right) treeReduced(seg->right);
}

void insertAMachine(SegTreapNode* seg, int p, int k )
{
	SegTreapNode* temp;
	if (p == 0)
	{
		treeExtend(seg);

		// create a new parent node
		SegTreapNode* inserted = malloc(sizeof(SegTreapNode));
		inserted->left = inserted->right = 0; inserted->sum = k;
		inserted->start = inserted->end = 1;

		SegTreapNode* parent = malloc(sizeof(SegTreapNode));
		parent->right = seg->left; seg->left->parent = parent;
		parent->left = inserted; inserted->parent = parent;
		parent->sum = parent->left->sum + parent->right->sum;
		parent->start = 1;	parent->end = parent->right->end;

		seg->left = parent; parent->parent = seg; 
		seg->sum = seg->left->sum + seg->right->sum;
		seg->start = 1;

		// END
		return;
	}
	else if (p == N)
	{
		// create a new node and a parent node
		SegTreapNode* inserted = malloc(sizeof(SegTreapNode));
		inserted->left = inserted->right = 0; inserted->sum = k;
		inserted->start = inserted->end = N+1;

		SegTreapNode* parent = malloc(sizeof(SegTreapNode));
		parent->left = seg->right; seg->right->parent = parent;
		parent->right = inserted; inserted->parent = parent;
		parent->sum = parent->left->sum + parent->right->sum;
		parent->start = parent->left->start; parent->end = parent->right->end;

		seg->right = parent; parent->parent = seg;
		seg->sum += k; 
		seg->end = seg->right->end;
		// End
		return;
	}

	if (seg->end < p) return;
	if (seg->start > p)
	{
		treeExtend(seg);
	}
	else
	{
		if (seg->end == p)
		{
		// create a new node and a parent node
			SegTreapNode* inserted = malloc(sizeof(SegTreapNode));
			inserted->left = inserted->right = 0; inserted->sum = k;
			inserted->start = inserted->end = p;

			SegTreapNode* parent = malloc(sizeof(SegTreapNode));
			parent->left = seg->right; seg->right->parent = parent;
			parent->right = inserted; inserted->parent = parent;
			parent->sum = parent->left->sum + parent->right->sum;
			parent->start = parent->left->start; parent->end = parent->right->end;

			seg->right = parent; parent->parent = seg;
			seg->sum += k;
			seg->end = seg->right->end;
		}
		else if (seg->start == p)
		{
			// create a new parent node
			SegTreapNode* inserted = malloc(sizeof(SegTreapNode));
			inserted->left = inserted->right = 0; inserted->sum = k;
			inserted->start = inserted->end = p;

			SegTreapNode* parent = malloc(sizeof(SegTreapNode));
			parent->right = seg->left; seg->left->parent = parent;
			parent->left = inserted; inserted->parent = parent;
			parent->sum = parent->left->sum + parent->right->sum;
			parent->start = p;	parent->end = parent->right->end;

			seg->left = parent; parent->parent = seg;
			seg->sum = seg->left->sum + seg->right->sum;
			seg->start = p;
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

void retireAMachine(SegTreapNode* seg, int p)
{
	if ( seg->end < p) return;
	if (seg->start > p)
	{
		treeReduced(seg);
	}
	else
	{
		if (seg->end == p && seg->start == p)
		{
			if (seg->parent->left == seg)
			{
				// left child
				if (seg->parent->parent == 0)
				{
					root = seg->parent->right;
				}
				else
				{
					if (seg->parent == seg->parent->parent->right)
						seg->parent->parent->right = seg->parent->right;
					else
						seg->parent->parent->left = seg->parent->right;
				}
			}
			else
			{
				// right child
				if (seg->parent->parent == 0)
				{
					root = seg->parent->left;
				}
				else
				{
					if (seg->parent == seg->parent->parent->right)
						seg->parent->parent->right = seg->parent->left;
					else
						seg->parent->parent->left = seg->parent->left;
				}
			}
			SegTreapNode* temp = seg->parent->parent;
			while (1)
			{
				temp->sum -= seg->sum;
				//temp->end--;
				if (temp->parent == 0)break;
				temp = temp->parent;
			}
			//free(seg->parent);
			//free(seg);
		}
		else
		{
			// drill down
			seg->end--;
			if (seg->left) retireAMachine(seg->left, p);
			if (seg->right)retireAMachine(seg->right, p);
		}

	}


}


SegTreapNode* array0;
SegTreapNode* array1;
SegTreapNode* root;
int total;

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
			array1[i].parent = 0;
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
			N = N + 1;
			break;
		case 2:
			scanf("%d", &p);
		   retireAMachine(root, p);
		   N = N - 1;
			break;
		case 3:
			scanf("%d %d", &l, &r);
	       // swapMachine(l, r);
			break;
		case 4:
			scanf("%d %d %d %d", &l, &r, &x, &y);
		//	swapTwoBlocks(l, r, x, y);
			break;
		case 5:
			scanf("%d %d %d ", &l, &r, &k);
			rangeStart = l; rangeEnd = r;
			timeLimit = k;
			truncateTimes(root);
			break;
		case 6:
			scanf("%d %d", &l, &r);
			total = 0;
			rangeStart = l; rangeEnd = r;
			printRebootTime(root );
			printf("%d\n", total );
			break;
		}
	}


}