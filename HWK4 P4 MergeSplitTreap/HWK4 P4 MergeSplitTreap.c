#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct smtpNode
{
	int priority;
	int sum;
	int subTreeSize; // 
	struct smtpNode* left;
	struct smtpNode* right;
} SmtpNode;


int N, Q;
SmtpNode* root, * temp;

int leftsum, rightsum;
int rangeStart, otherStart;
int rangeEnd, otherEnd;
int total;
int timeLimit;
int trimedAmount;




SmtpNode* array;
SmtpNode* root;
int total;

SmtpNode* merge(SmtpNode* a, SmtpNode* b) //將根節點為 a 和 b 的 treap 合併
{
	if (!a) return b;
	if (!b) return a; //記得處理 base case
	if ( a->priority > b->priority )	 
	{ 
		//a 的 pri 比較大，應該當 b 的祖先
		a->right = merge( a->right, b ); //b 的 key 較大，應和 a 的右子樹合併
		return a;
	}
	// otherwise
	b->left = merge(a, b->left);
	 return b;
}

void push(SmtpNode* node)
{
	if (node == 0) return;
	
}

void updateSubTreeSize(SmtpNode* node)
{
	node->subTreeSize = 1 + node->left->subTreeSize + node->right->subTreeSize;
}
void split(SmtpNode* node, int k, SmtpNode** a, SmtpNode** b) //依照 k 將 treap 分割至 a, b
{
	// Find the k-th node 
	SmtpNode* kthNode;

	if (!node)
	{
		a = b = 0; //base case
		return;
	}

	// If node left subtree size is smaller than k;  
	// the target node is in the righ subtree
 
	if (node->left->subTreeSize < k)
	{
		a = node;
		push(a);
		split(node->right, k - node->left->subTreeSize - 1, (*a)->right, b); //只剩右子樹要分割
		pull(a);
	}
	else
	{
		b = node;
		push(b);
		split(node->left, k, a, (*b)->left); //只剩左子樹要分割
		pull(b);
	}

}

SmtpNode* construct(int start, int end)
{
	if (start > end) return 0;
	int mid = ( start + end ) / 2;
	SmtpNode* left = construct(start, mid - 1);
	array[mid].left = left;
	array[mid].right = construct(mid + 1, end);
	return &array[mid];
}
 
int recursiveAssignChildrenPriority(SmtpNode* node)
{
	node->subTreeSize = 1;
	if (node->left)
	{
		do node->left->priority = rand();
		while (node->left->priority >= node->priority);
		node->subTreeSize += recursiveAssignChildrenPriority(node->left);
	}
	if (node->right)
	{
		do node->right->priority = rand();
		while (node->right->priority >= node->priority);
		node->subTreeSize += recursiveAssignChildrenPriority(node->right);
	}
	return node->subTreeSize;
}

void main()
{
	scanf("%d %d", &N, &Q);
	// Create N nodes associated with random priority
	array = malloc(sizeof(SmtpNode) * (N));
	for (int i = 0; i < N; i++)
	{
		array[i].left = array[i].right = 0;
		scanf("%d", &array[i].sum);

	}

	root = construct(0, N-1);
	root->priority = rand();
	root->subTreeSize = recursiveAssignChildrenPriority(root);


	int cmd, p, k, l, r, x, y;
	for (int i = 0; i < Q; i++)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case 1:
			scanf("%d %d", &p, &k);
		//	insertAMachine(root, p, k);
			N = N + 1;
			break;
		case 2:
			scanf("%d", &p);
		//	retireAMachine(root, p);
			N = N - 1;
			break;
		case 3:
			scanf("%d %d", &l, &r);
		//	k = retireAMachine(root, r);
		//	insertAMachine(root, p, k);
			break;
		case 4:
			scanf("%d %d %d %d", &l, &r, &x, &y);
			int t;
			if (x < l)
			{
				t = x;
				x = l;
				l = t;
				t = y;
				y = r;
				r = t;
			}


			rangeStart = l;	rangeEnd = r;
			otherStart = x; otherEnd = y;
		//	swapTwoBlocks(root);
			break;
		case 5:
			scanf("%d %d %d ", &l, &r, &k);
			rangeStart = l; rangeEnd = r;
			timeLimit = k;
		//	truncateTimes(root);
			break;
		case 6:
			scanf("%d %d", &l, &r);
			total = 0;
			rangeStart = l; rangeEnd = r;
		//	printRebootTime(root);
			printf("%d\n", total);
			break;
		}
	}


}