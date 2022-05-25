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
	//unsigned long long next, prev,  temp;
	XorNode* nextNode = 0, *prevNode = 0 ;

	// Create an instance and assign time value
	XorNode* addNode = malloc(sizeof(XorNode));
	addNode->time = k;

	if (p == 0)
	{
		head->prevNext = (unsigned long long)  (head->prevNext)  ^ (unsigned long long) addNode;
		addNode->prevNext = head;
		head = addNode;
		N = N + 1;
		return;
	}	
	
	if (p >= N)
	{
		tail->prevNext = (unsigned long long)(tail->prevNext) ^ (unsigned long long)addNode;
		addNode->prevNext = tail;
		tail = addNode;
		N = N + 1;
		return;
	}
		
	if (p < N / 2.0 )
	{
		// from head

		tempNode = head;
		count = 1;
		prevNode = 0;
		while (count != p)
		{
			nextNode =  (unsigned long long)( tempNode->prevNext ) ^ (unsigned long long)prevNode;
			prevNode = tempNode;
			tempNode = nextNode;
			count++;
		}
		nextNode =  (unsigned long long)( tempNode->prevNext) ^ (unsigned long long)prevNode;

		// Insert a node after temp
		//addNode->prevNext = (unsigned long long)tempNode ^ (unsigned long long)nextNode;
		// tempNode->prevNext = (unsigned long long)prevNode ^ (unsigned long long)addNode;
		// nextNode->prevNext =  (unsigned long long)( nextNode->prevNext ) ^ (unsigned long long)tempNode ^ (unsigned long long)addNode;
	}
	else
	{
		// from tail
		tempNode = tail;

		count = N;
		nextNode = 0;
		while (count != p)
		{
			prevNode = (unsigned long long)(tempNode->prevNext) ^ (unsigned long long)nextNode;
			nextNode = tempNode;
			tempNode = prevNode;
			count--;
		}
		prevNode = (unsigned long long)(tempNode->prevNext) ^ (unsigned long long)nextNode;

		//// Insert a node after temp
		//addNode->prevNext = (unsigned long long)tempNode ^ (unsigned long long)nextNode;
		//tempNode->prevNext = (unsigned long long)prevNode ^ (unsigned long long)addNode;
		//nextNode->prevNext =  (unsigned long long)(nextNode->prevNext) ^ (unsigned long long)tempNode  ^ (unsigned long long)addNode;
	}
	// Insert a node after temp
	addNode->prevNext = (unsigned long long)tempNode ^ (unsigned long long)nextNode;
	tempNode->prevNext = (unsigned long long)prevNode ^ (unsigned long long)addNode;
	nextNode->prevNext = (unsigned long long)(nextNode->prevNext) ^ (unsigned long long)tempNode ^ (unsigned long long)addNode;

	N = N + 1;
}

void retireAMachine(int p)
{
	int count;
	//unsigned long long next, prev, temp;
	XorNode* nextNode, *prevNode;
	XorNode* hitNode;

	if (p == 1)
	{
		// get next node of head
		nextNode = head->prevNext;
		// update next node; remove head by xor operation
		 nextNode->prevNext = (unsigned long long)(nextNode->prevNext) ^ (unsigned long long)head;
		 // reassign new head
		head = nextNode;
		N = N - 1;
		return;
	}

	if (p >= N)
	{
		// get prev node of tail
		prevNode = tail->prevNext;
		// update prev node; remove tail by xor operation
		 prevNode->prevNext  =  (unsigned long long)(prevNode->prevNext)  ^ (unsigned long long)tail;
		 // reassign new tail
		tail = prevNode;
		N = N - 1;
		return;
	}

	if (p < N / 2.0 )
	{
		// start from head
		hitNode = head;
		count = 1;
		prevNode = 0;
		while (count != p)
		{
			nextNode = (unsigned long long)(hitNode->prevNext) ^ (unsigned long long)prevNode;
			prevNode = hitNode;
			hitNode = nextNode;
			count++;
		}
		nextNode =  (unsigned long long)(hitNode->prevNext) ^ (unsigned long long) prevNode ;
	}
	else
	{
		// from tail
		hitNode = tail;
		count = N;
		nextNode = 0;
		while (count != p)
		{
			prevNode = (unsigned long long)(hitNode->prevNext) ^ (unsigned long long)nextNode;
			nextNode = hitNode;
			hitNode = prevNode;
			count--;
		}
		prevNode = (unsigned long long)(hitNode->prevNext) ^ (unsigned long long)nextNode;

	}
	// Discard tempNode
	prevNode->prevNext = (unsigned long long)(prevNode->prevNext) ^ (unsigned long long)hitNode ^ (unsigned long long)nextNode;
	nextNode->prevNext = (unsigned long long)(nextNode->prevNext) ^ (unsigned long long)hitNode ^ (unsigned long long)prevNode;
	N = N + 1;
}


void swapMachine(int l, int r)
{
	int count;
	XorNode* nextNode1=0, * prevNode1 =0, * nextNode2 = 0, * prevNode2 = 0;
	XorNode* node1, * node2;

	// Forward 
	if (l < N / 2.0 )
	{
		// from head start with node1
		node1 = head;
		count = 1;
		prevNode1 = 0;
		while (count != l)
		{
			nextNode1 = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode1;
			prevNode1 = node1;
			node1 = nextNode1;
			count++;
		}
		nextNode1 = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode1;

		if (r < N / 2.0)
		{
			node2 = node1;
			prevNode2 = prevNode1;
			while (count != r)
			{
				nextNode2 =  (unsigned long long)(node2->prevNext) ^ (unsigned long long)prevNode2;
				prevNode2 = node2;
				node2 = nextNode2;
				count++;
			}
			nextNode2 = (unsigned long long)(node2->prevNext) ^ (unsigned long long)prevNode2;
		}
		else
		{
			node2 = tail;
			count = N;
			nextNode2 = 0;
			while (count != r)
			{
				prevNode2 = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode2;
				nextNode2 = node2;
				node2 = prevNode2;
				count--;
			}
			prevNode2 = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode2;
		}
	}
	else
	{
		// Backward start with node2
		// from tail
		node2 = tail;
		count = N;
		nextNode2 = 0;
		while (count != r)
		{
			prevNode2 = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode2;
			nextNode2 = node2;
			node2 = prevNode2;
			count--;
		}
		prevNode2 = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode2;

		if (l >= N / 2.0)
		{
			node1 = node2;
			nextNode1 = nextNode2;

			while (count != l)
			{
				prevNode1 = (unsigned long long)(node1->prevNext) ^ (unsigned long long)nextNode1;
				nextNode1 = node1;
				node1 = prevNode1;
				count--;
			}
			prevNode1 = (unsigned long long)(node1->prevNext) ^ (unsigned long long)nextNode1;
		}
		else
		{
			// from head start with node1
			node1 = head;
			count = 1;
			prevNode1 = 0;
			while (count != l)
			{
				nextNode1 = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode1;
				prevNode1 = node1;
				node1 = nextNode1;
				count++;
			}
			nextNode1 = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode1;
		}

	}

	//
	unsigned long long node1xornode2 = (unsigned long long) node1  ^ (unsigned long long) node2;
	if (prevNode1)
	{
		prevNode1->prevNext = (unsigned long long)(prevNode1->prevNext) ^ node1xornode2;
	}
	else
	{
		// update head
		head = node2;
	}
	if (nextNode1)
	{
		nextNode1->prevNext = (unsigned long long)(nextNode1->prevNext) ^ node1xornode2;
	}
	else
	{

	}
	if (prevNode2)
	{
		prevNode2->prevNext = (unsigned long long)(prevNode2->prevNext) ^ node1xornode2;
	}
	else
	{

	}
	if (nextNode2)
	{
		nextNode2->prevNext = (unsigned long long)(nextNode2->prevNext) ^ node1xornode2;
	}
	else
	{
		// update tail
		tail = node1;
	}

	// Swap node1 and node2
	XorNode* t = node1->prevNext;
	node1->prevNext = node2->prevNext;
	node2->prevNext = t;
}
 


void swapTwoBlocks(int l, int r, int x, int y)
{
	int temp;

	if (l > x)
	{
		temp = l;
		l = x;
		x = temp;
		temp = r;
		r = y;
		y = temp;
	}
	int* lengths;
	int traverseCase = 0;
	// case 0   1->l->r->x->y 
	//lengths[0] = y;
	int minLength = y;
	// case 1 1->l->r->x  y<-N
	int length = x + N - y + 1;
	if (length < minLength)
	{
		traverseCase = 1;
		minLength = length;
	}
	// case 2 1->l->r  x<-y<-N;
	length = r + N - x + 1;
	if (length < minLength)
	{
		traverseCase = 2;
		minLength = length;
	}

	// case 3 1->l r<-x<-y<-N;
	length = l + N - r + 1;
	if (length < minLength)
	{
		traverseCase = 3;
		minLength = length;
	}	
	// case 4   l<-r<-x<-y<-N;
	length = N - l + 1;
	if (length < minLength)
	{
		traverseCase = 4;
		minLength = length;
	}

	XorNode* left=0, * leftPrev = 0, * right = 0, * rightNext = 0, * X = 0, * XPrev = 0, * Y = 0, * YNext = 0;

	XorNode* prevNode, * nextNode;
	int count;
	// Traverse the list
	switch (traverseCase)
	{
		case 0: // case 0   1->l->r->x->y
			prevNode = 0;
			left = head;
			count = 1;
			while (count != l) // Find left node
			{
				nextNode = (unsigned long long)( left->prevNext ) ^ (unsigned long long) prevNode;
				prevNode = left;
				left = nextNode;
				count++;
			}
			leftPrev = prevNode; // confirmed
			right = left;
			while (count != r) // Find right node
			{
				nextNode = (unsigned long long)(right->prevNext) ^ (unsigned long long) prevNode;
				prevNode = right;
				right = nextNode;
				count++;
			}
			rightNext = (unsigned long long)(right->prevNext) ^ (unsigned long long) prevNode; // confirm
			X = right;
			while (count != x) // Find x node
			{
				nextNode = (unsigned long long)(X->prevNext) ^ (unsigned long long) prevNode;
				prevNode = X;
				X = nextNode;
				count++;
			}
			XPrev = prevNode;
			Y = X;
			while (count != y) // Find y node
			{
				nextNode = (unsigned long long)(Y->prevNext) ^ (unsigned long long) prevNode;
				prevNode = Y;
				Y = nextNode;
				count++;
			}
			YNext = (unsigned long long)(Y->prevNext) ^ (unsigned long long) prevNode;
			break;
		case 1: // case 1 1->l->r->x  y<-N
			prevNode = 0;
			left = head;
			count = 1;
			while (count != l) // Find left node
			{
				nextNode = (unsigned long long)(left->prevNext) ^ (unsigned long long) prevNode;
				prevNode = left;
				left = nextNode;
				count++;
			}
			leftPrev = prevNode; // confirmed
			right = left;
			while (count != r) // Find right node
			{
				nextNode = (unsigned long long)(right->prevNext) ^ (unsigned long long) prevNode;
				prevNode = right;
				right = nextNode;
				count++;
			}
			rightNext = (unsigned long long)(right->prevNext) ^ (unsigned long long) prevNode; // confirm
			X = right;
			while (count != x) // Find x node
			{
				nextNode = (unsigned long long)(X->prevNext) ^ (unsigned long long) prevNode;
				prevNode = X;
				X = nextNode;
				count++;
			}
			XPrev = prevNode;

			Y = tail;
			nextNode = 0;
			count = N;
			while (count != y) // Find y node
			{
				prevNode = (unsigned long long)(Y->prevNext) ^ (unsigned long long) nextNode;
				nextNode = Y;
				Y = prevNode;
				count--;
			}
			YNext = nextNode;
			break;
		case 2: // case 2 1->l->r  x<-y<-N;

			prevNode = 0;
			left = head;
			count = 1;
			while (count != l) // Find left node
			{
				nextNode = (unsigned long long)(left->prevNext) ^ (unsigned long long) prevNode;
				prevNode = left;
				left = nextNode;
				count++;
			}
			leftPrev = prevNode; // confirmed
			right = left;
			while (count != r) // Find right node
			{
				nextNode = (unsigned long long)(right->prevNext) ^ (unsigned long long) prevNode;
				prevNode = right;
				right = nextNode;
				count++;
			}
			rightNext = (unsigned long long)(right->prevNext) ^ (unsigned long long) prevNode; // confirm

			Y = tail;
			nextNode = 0;
			count = N;
			while (count != y) // Find y node
			{
				prevNode = (unsigned long long)(Y->prevNext) ^ (unsigned long long) nextNode;
				nextNode = Y;
				Y = prevNode;
				count--;
			}
			YNext = nextNode;

			X = Y;
			while (count != x) // Find x node
			{
				prevNode = (unsigned long long)(X->prevNext) ^ (unsigned long long) nextNode;
				nextNode = X;
				X = prevNode;
				count--;
			}
			XPrev = (unsigned long long)(X->prevNext) ^ (unsigned long long) nextNode;
			break;
		case 3: // case 3 1->l r<-x<-y<-N;
			prevNode = 0;
			left = head;
			count = 1;
			while (count != l) // Find left node
			{
				nextNode = (unsigned long long)(left->prevNext) ^ (unsigned long long) prevNode;
				prevNode = left;
				left = nextNode;
				count++;
			}
			leftPrev = prevNode; // confirmed

			Y = tail;
			nextNode = 0;
			count = N;
			while (count != y) // Find y node
			{
				prevNode = (unsigned long long)(Y->prevNext) ^ (unsigned long long) nextNode;
				nextNode = Y;
				Y = prevNode;
				count--;
			}
			YNext = nextNode;

			X = Y;
			while (count != x) // Find x node
			{
				prevNode = (unsigned long long)(X->prevNext) ^ (unsigned long long) nextNode;
				nextNode = X;
				X = prevNode;
				count--;
			}
			XPrev = (unsigned long long)(X->prevNext) ^ (unsigned long long) nextNode;

			right = X;
			while (count != r) // Find right node
			{
				prevNode = (unsigned long long)(right->prevNext) ^ (unsigned long long) nextNode;
				nextNode = right;
				right = prevNode;
				count++;
			}
			rightNext = nextNode; 
			break;
		case 4: // case 4   l<-r<-x<-y<-N;
			Y = tail;
			nextNode = 0;
			count = N;
			while (count != y) // Find y node
			{
				prevNode = (unsigned long long)(Y->prevNext) ^ (unsigned long long) nextNode;
				nextNode = Y;
				Y = prevNode;
				count--;
			}
			YNext = nextNode;

			X = Y;
			while (count != x) // Find x node
			{
				prevNode = (unsigned long long)(X->prevNext) ^ (unsigned long long) nextNode;
				nextNode = X;
				X = prevNode;
				count--;
			}
			XPrev = (unsigned long long)(X->prevNext) ^ (unsigned long long) nextNode;

			right = X;
			while (count != r) // Find right node
			{
				prevNode = (unsigned long long)(right->prevNext) ^ (unsigned long long) nextNode;
				nextNode = right;
				right = prevNode;
				count--;
			}
			rightNext = nextNode;

			left = right;
			while (count != l) // Find left node
			{
				prevNode = (unsigned long long)(left->prevNext) ^ (unsigned long long) nextNode;
				nextNode = left;
				left = prevNode;
				count--;
			}
			leftPrev = prevNode = (unsigned long long)(left->prevNext) ^ (unsigned long long) nextNode;
			break;
	}
	
	// Reconnect swapped blocks

	if (leftPrev)
	{
		// disengage with left node;  connect X
		leftPrev->prevNext = (unsigned long long)(leftPrev->prevNext) ^ (unsigned long long)left ^ (unsigned long long)X;
	}
	else
	{
		// original left is head; new head is X
		head = X;
	}
	// disengage with leftPrev; connect Xprev
	left->prevNext = (unsigned long long)(left->prevNext) ^ (unsigned long long)leftPrev ^ (unsigned long long)XPrev;

	// disengage with right node; connect Y
	rightNext->prevNext = (unsigned long long)(rightNext->prevNext) ^ (unsigned long long)right ^ (unsigned long long)Y;
	// disengage with rightNext; connect Ynext
	right->prevNext = (unsigned long long)(right->prevNext) ^ (unsigned long long)rightNext ^ (unsigned long long)YNext;

	// disengage with x node; connect left
	XPrev->prevNext = (unsigned long long)(XPrev->prevNext) ^ (unsigned long long)X ^ (unsigned long long)left;
	// disengage with x prev; connect left prev
	X->prevNext = (unsigned long long)(X->prevNext) ^ (unsigned long long)XPrev ^ (unsigned long long)leftPrev;

	if (YNext)
	{
		// disengage with Y node; connect 
		YNext->prevNext = (unsigned long long)(YNext->prevNext) ^ (unsigned long long)Y ^ (unsigned long long)right;
	}
	else
	{
		// original y is tail; tail now is right
		tail = right;
	}
	// disengage with Ynext; connect right next
	Y->prevNext = (unsigned long long)(Y->prevNext) ^ (unsigned long long)YNext ^ (unsigned long long)rightNext;

}


void truncateTimes(int l, int r, int k)
{
	int count;
	XorNode* nextNode, * prevNode, * nextNode2, * prevNode2;
	XorNode* node1, * node2;

	// Forward 
	if (l < N / 2.0)
	{
		// from head start with node1
		node1 = head;
		count = 1;
		prevNode = 0;
		while (count != l)
		{
			nextNode = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode;
			prevNode = node1;
			node1 = nextNode;
			count++;
		}
		if (node1->time > k) node1->time = k;
		while (count < r)
		{
			nextNode = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode;
			prevNode = node1;
			node1 = nextNode;
			if (node1->time > k) node1->time = k;
			count++;
		}
	}
	else
	{
		// Backward start with node2
		// from tail
		node2 = tail;
		count = N;
		nextNode = 0;
		while (count != r)
		{
			prevNode = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode;
			nextNode = node2;
			node2 = prevNode;
			count--;
		}
		if (node2->time > k) node2->time = k;
 
		while (count > l)
		{
			prevNode = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode;
			nextNode = node2;
			node2 = prevNode;
			if (node2->time > k) node2->time = k;
			count--;
		}	 
	}
}

void printRebootTime(int l, int r)
{
	int count;
	XorNode* nextNode, *prevNode ;
	XorNode* node1, * node2;
	long long sum = 0;
	// Forward 
	if (l < N / 2.0 )
	{
		// from head start with node1
		node1 = head;
		count = 1;
		prevNode = 0;
		while (count != l)
		{
			nextNode =  (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode;
			prevNode = node1;
			node1 = nextNode;
			count++;
		}
		sum += node1->time;
		while (count < r  )
		{
			nextNode = (unsigned long long)(node1->prevNext) ^ (unsigned long long)prevNode;
			prevNode = node1;
			node1 = nextNode;
			sum += node1->time;
			count++;
		}
	}
	else
	{
		// Backward start with node2
		// from tail
		node2 = tail;
		count = N;
		nextNode = 0;
		while (count != r)
		{
			prevNode = (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode;
			nextNode = node2;
			node2 = prevNode;
			count--;
		}
		sum += node2->time;

		while (count > l)
		{
			prevNode =  (unsigned long long)(node2->prevNext) ^ (unsigned long long)nextNode;
			nextNode = node2;
			node2 = prevNode;
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
			tempNode->prevNext = 0;
			head = tempNode;
		}
		else
		{
			previous->prevNext = (unsigned long long)previous->prevNext ^ (unsigned long long)tempNode;
			tempNode->prevNext = previous;
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
			swapTwoBlocks(l, r, x, y);
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