#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


typedef struct xorNode
{
	int time;
	struct xorNode* prevNext;
} XorNode;


int N, Q ;
XorNode	* head, *tail, *tempNode;

void insertAMachine(int p, int k)
{
	int count;
	//unsigned int next, prev,  temp;
	XorNode* nextNode = 0, *prevNode = 0, *nextNode = 0;

	// Create an instance and assign time value
	XorNode* addNode = malloc(sizeof(XorNode));
	addNode->time = k;

	if (p == 0)
	{
		head->prevNext = (unsigned int)  head  ^ (unsigned int) addNode;
		addNode->prevNext = head;
		head = addNode;
		N = N + 1;
		return;
	}	
	
	if (p >= N)
	{
		tail->prevNext = (unsigned int)(tail->prevNext) ^ (unsigned int)addNode;
		addNode->prevNext = tail;
		tail = addNode;
		N = N + 1;
		return;
	}
		
	if (p < N / 2)
	{
		// from head

		tempNode = head;
		count = 1;
		prevNode = 0;
		while (count != p)
		{
			nextNode =  (unsigned int) tempNode->prevNext ^ (unsigned int)prevNode;
			prevNode = tempNode;
			tempNode = nextNode;
			count++;
		}
		nextNode =  (unsigned int)tempNode->prevNext ^ (unsigned int)prevNode;

		// Insert a node after temp
		addNode->prevNext = (unsigned int)tempNode ^ (unsigned int)nextNode;
		 tempNode->prevNext = (unsigned int)prevNode ^ (unsigned int)addNode;
		 nextNode->prevNext =  (unsigned int)( nextNode->prevNext ) ^ (unsigned int)tempNode ^ (unsigned int)addNode;
	}
	else
	{
		// from tail
		tempNode = tail;

		count = N;
		nextNode = 0;
		while (count != p)
		{
			prevNode = (unsigned int)(tempNode->prevNext) ^ (unsigned int)nextNode;
			nextNode = tempNode;
			tempNode = prevNode;
			count--;
		}
		prevNode = (unsigned int)(tempNode->prevNext) ^ (unsigned int)nextNode;

		// Insert a node after temp
		addNode->prevNext = (unsigned int)tempNode ^ (unsigned int)nextNode;
		tempNode->prevNext = (unsigned int)prevNode ^ (unsigned int)addNode;
		nextNode->prevNext = ((unsigned int)(nextNode->prevNext) ^ (unsigned int)tempNode) ^ (unsigned int)addNode;
	}

	N = N + 1;
}

void retireAMachine(int p)
{
	int count;
	unsigned int next, prev, temp;
	XorNode* nextNode, *prevNode;
 
	if (p == 1)
	{
		nextNode = head->prevNext;
		 nextNode->prevNext = (unsigned int)(nextNode->prevNext) ^ (unsigned int)head;
		head = nextNode;
		N = N - 1;
		return;
	}

	if (p >= N)
	{
		prev = (unsigned int)(tail->prevNext);
		prevNode = (XorNode*)prev;
		 prevNode->prevNext  =  (unsigned int)(prevNode->prevNext)  ^ (unsigned int)tail;
		tail = prevNode;
		N = N - 1;
		return;
	}

	if (p < N / 2)
	{
		// from head
		tempNode = head;
		temp = (unsigned int)tempNode;
		count = 1;
		prev = 0;
		while (count != p)
		{
			next = ((unsigned int)(tempNode->prevNext) ^ prev);
			prev = temp;
			temp = next;
			tempNode = (XorNode*)temp;
			count++;
		}
		prevNode = (XorNode*)prev;
		next = ((unsigned int)(tempNode->prevNext) ^ prev);
		nextNode = (XorNode*)next;
		// Discard tempNode
		prevNode->prevNext = (unsigned int)(prevNode->prevNext) ^ temp ^ next;
		nextNode->prevNext = (unsigned int)(nextNode->prevNext) ^ temp ^ prev;
	}
	else
	{
		// from tail
		tempNode = tail;
		temp = (unsigned int)tempNode;
		count = N;
		next = 0;
		while (count != p)
		{
			prev = (unsigned int)(tempNode->prevNext) ^ next;
			next = temp;
			temp = prev;
			tempNode = (XorNode*)temp;
			count--;
		}
		prev = (unsigned int)(tempNode->prevNext) ^ next;
		prevNode = (XorNode*)prev;
		nextNode = (XorNode*)next;
		// Discard tempNode
		prevNode->prevNext = (unsigned int)(prevNode->prevNext) ^ temp ^ next;
		nextNode->prevNext = (unsigned int)(nextNode->prevNext) ^ temp ^ prev;
	}
	N = N + 1;
}


void swapMachine(int l, int r)
{
	int count;
	unsigned int next, prev, add1, add2;
	XorNode* nextNode1=0, * prevNode1 =0, * nextNode2=0, * prevNode2=0;
	XorNode* node1, * node2;

	// Forward 
	if (l < N / 2)
	{
		// from head start with node1
		node1 = head;
		add1 = (unsigned int)node1;
		count = 1;
		prev = 0;
		while (count != l)
		{
			next = ((unsigned int)(node1->prevNext) ^ prev);
			prev = add1;
			add1 = next;
			node1 = (XorNode*)add1;
			count++;
		}
		prevNode1 = (XorNode*)prev;
		next = ((unsigned int)(node1->prevNext) ^ prev);
		nextNode1 = (XorNode*)next;

		if (r < N / 2)
		{
			node2 = node1;
			add2 = add1;
			while (count != r)
			{
				next = ((unsigned int)(node2->prevNext) ^ prev);
				prev = add2;
				add2 = next;
				node2 = (XorNode*)add2;
				count++;
			}
			prevNode2 = (XorNode*)prev;
			next = ((unsigned int)(node2->prevNext) ^ prev);
			nextNode2 = (XorNode*)next;
		}
		else
		{
			node2 = tail;
			add2 = (unsigned int)node2;
			count = N;
			next = 0;
			while (count != r)
			{
				prev = (unsigned int)(node2->prevNext) ^ next;
				next = add2;
				add2 = prev;
				node2 = (XorNode*)add2;
				count--;
			}
			nextNode2 = (XorNode*)next;
			prev = (unsigned int)(node2->prevNext) ^ next;
			prevNode2 = (XorNode*)prev;
		}
	}
	else
	{
		// Backward start with node2
		// from tail
		node2 = tail;
		add2 = (unsigned int)node2;
		count = N;
		next = 0;
		while (count != r)
		{
			prev = (unsigned int)(node2->prevNext) ^ next;
			next = add2;
			add2 = prev;
			node2 = (XorNode*)add2;
			count--;
		}
		nextNode2 = (XorNode*)next;
		prev = (unsigned int)(node2->prevNext) ^ next;
		prevNode2 = (XorNode*)prev;

		if (l >= N / 2)
		{
			node1 = node2;
			add1 = add2;
			while (count != l)
			{
				prev = ((unsigned int)(node1->prevNext) ^ next);
				next = add1;
				add1 = prev;
				node1 = (XorNode*)add1;
				count--;
			}
			nextNode1 = (XorNode*)next;
			prev = ((unsigned int)(node1->prevNext) ^ next);
			nextNode1 = (XorNode*)prev;
		}
		else
		{
			// from head start with node1
			node1 = head;
			add1 = (unsigned int)node1;
			count = 1;
			prev = 0;
			while (count != l)
			{
				next = ((unsigned int)(node1->prevNext) ^ prev);
				prev = add1;
				add1 = next;
				node1 = (XorNode*)add1;
				count++;
			}
			prevNode1 = (XorNode*)prev;
			next = ((unsigned int)(node1->prevNext) ^ prev);
			nextNode1 = (XorNode*)next;
		}

	}

	// 
	prevNode1->prevNext  = (unsigned int)( prevNode1->prevNext)^ add1 ^ add2;
	nextNode1->prevNext  = (unsigned int)( nextNode1->prevNext)^ add1 ^ add2;
	prevNode2->prevNext  = (unsigned int)( prevNode2->prevNext)^ add2 ^ add1;
	nextNode2->prevNext  = (unsigned int)( nextNode2->prevNext)^ add2 ^ add1;
	XorNode* t = node1->prevNext;
	node1->prevNext = node2->prevNext;
	node2->prevNext = t;
}
 
void swapMachineMachine(int l, int r, int x, int y)
{
	swapMachine(l, r);
	swapMachine(x,y);
}

void truncateTimes(int l, int r, int k)
{
	int count;
	unsigned int next, prev, add1, add2;
	XorNode* nextNode1, * prevNode1, * nextNode2, * prevNode2;
	XorNode* node1, * node2;

	// Forward 
	if (l < N / 2)
	{
		// from head start with node1
		node1 = head;
		add1 = (unsigned int)node1;
		count = 1;
		prev = 0;
		while (count != l)
		{
			next = ((unsigned int)(node1->prevNext) ^ prev);
			prev = add1;
			add1 = next;
			node1 = (XorNode*)add1;
			count++;
		}
		if (node1->time > k) node1->time = k;
		while (count != r + 1)
		{
			next = ((unsigned int)(node1->prevNext) ^ prev);
			prev = add1;
			add1 = next;
			node1 = (XorNode*)add1;
			if (node1->time > k) node1->time = k;
			count++;
		}
	}
	else
	{
		// Backward start with node2
		// from tail
		node2 = tail;
		add2 = (unsigned int)node2;
		count = N;
		next = 0;
		while (count != r)
		{
			prev = (unsigned int)(node2->prevNext) ^ next;
			next = add2;
			add2 = prev;
			node2 = (XorNode*)add2;
			count--;
		}
		if (node2->time > k) node2->time = k;
		if (l >= N / 2)
		{
			while (count != l-1)
			{
				prev = ((unsigned int)(node2->prevNext) ^ next);
				next = add2;
				add2 = prev;
				node2 = (XorNode*)add2;
				if (node2->time > k) node2->time = k;
				count--;
			}
		}
	}
}

void printRebootTime(int l, int r)
{
	int count;
	unsigned int next, prev, add1, add2;
	XorNode* nextNode1, * prevNode1, * nextNode2, * prevNode2;
	XorNode* node1, * node2;
	long long sum = 0;
	// Forward 
	if (l < N / 2)
	{
		// from head start with node1
		node1 = head;
		add1 = (unsigned int)node1;
		count = 1;
		prev = 0;
		while (count != l)
		{
			next = ((unsigned int)(node1->prevNext) ^ prev);
			prev = add1;
			add1 = next;
			node1 = (XorNode*)add1;
			count++;
		}
		sum += node1->time;
		while (count != r + 1)
		{
			next = ((unsigned int)(node1->prevNext) ^ prev);
			prev = add1;
			add1 = next;
			node1 = (XorNode*)add1;
			sum += node1->time;
			count++;
		}
	}
	else
	{
		// Backward start with node2
		// from tail
		node2 = tail;
		add2 = (unsigned int)node2;
		count = N;
		next = 0;
		while (count != r)
		{
			prev = (unsigned int)(node2->prevNext) ^ next;
			next = add2;
			add2 = prev;
			node2 = (XorNode*)add2;
			count--;
		}
		sum += node2->time;

		while (count != l - 1)
		{
			prev = ((unsigned int)(node2->prevNext) ^ next);
			next = add2;
			add2 = prev;
			node2 = (XorNode*)add2;
			sum += node2->time;
			count--;
		}

	}
	printf("%d\n", sum);
}



void main()
{
	scanf("%d %d", &N, &Q);
	// Create N nodes associated with random priority
	XorNode* previous = 0;
	
	for (int i = 1; i <= N; i++)
	{
		tempNode = malloc( sizeof( XorNode));
		scanf("%d", &tempNode->time);
		if (previous == 0)
		{
			(unsigned int)(tempNode->prevNext) = 0;
			head = tempNode;
		}
		else
		{
			previous->prevNext = (unsigned int)previous->prevNext ^ (unsigned int)tempNode;
			(unsigned int)(tempNode->prevNext) = tempNode;
		}
		if (i == N)
		{
			tail = tempNode;
		}
		previous = tempNode;
	}

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