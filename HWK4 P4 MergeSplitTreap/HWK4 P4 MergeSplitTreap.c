#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct smtpNode
{
	int priority;
	int time;
	unsigned long long groupTime;
	int groupSize; // 
	struct smtpNode* left;
	struct smtpNode* right;
} SmtpNode;


int N, Q;
int TimeLimit;
SmtpNode* array;
SmtpNode* root;


//  注意！因為應用於 ordered 的 list，左邊的 subtree 排在 右邊的 subtree 前面。
//  因此 frontPart 必須是 排序較小 左側的 子樹， rearPart 必須是排序較大 右側的子樹
SmtpNode* merge(SmtpNode* frontPart, SmtpNode* rearPart) //將根節點為 a 和 b 的 treap 合併
{
	if (!frontPart) return rearPart;
	if (!rearPart) return frontPart; 

	// frontPart 的 priority 比較大，應該當 rearPart 的parent
	if ( frontPart->priority > rearPart->priority )	 
	{ 
		// rearPart 的排序較大，必須結合於 frontPart 的右子樹。
		frontPart->right = merge( frontPart->right, rearPart );

		frontPart->groupSize = ( frontPart->left ? frontPart->left->groupSize : 0  ) + ( frontPart->right ? frontPart->right->groupSize : 0 ) + 1;
		frontPart->groupTime = (frontPart->left ? frontPart->left->groupTime : 0) + (frontPart->right ? frontPart->right->groupTime : 0) + frontPart->time;
		return frontPart; // New Parent; New root;
	}
	
	// otherwise, frontPart 的 priority 比較小，應該當 rearPart 的 child
	// frontPart 在前面，因此需接於 rearPart 的左子樹
	rearPart->left = merge(frontPart, rearPart->left);

	rearPart->groupSize = ( rearPart->left ? rearPart->left->groupSize : 0 ) + ( rearPart->right ? rearPart->right->groupSize : 0  ) + 1;
	rearPart->groupTime = (rearPart->left ? rearPart->left->groupTime : 0) + (rearPart->right ? rearPart->right->groupTime : 0) + rearPart->time;
	return rearPart; // New Parent; New root;
}


// Split the tree after the k-th listed ordered node.
// If k = 0; no split. If k = N; no split.
void split(SmtpNode* node, int k, SmtpNode** frontPart, SmtpNode** rearPart) //依照 k 將 treap 分割至 a, b
{
	// Find the k-th node; split the tree between k and k+1 nodes.
	SmtpNode* kthNode;

	if (!node) //base case
	{
		// Write reference value  to the variables of the caller.
		// Terminate the tree branches
		*frontPart = *rearPart = 0; 
		return;
	}
	int leftCount = 0;
	if (node->left) leftCount = node->left->groupSize;
 
	// If node left part is smaller than k;  
	// the k-th node is in the right subtree, including node itself
	if ( leftCount < k)
	{
		// Find right subtree
		if (node->right)
		{
			// 左(前)部 front part 一定是 node 本身；從右側分出 rear part。
			*frontPart = node;

			// find the location in the right subtree and cut the rear part out.
			// 保留左側 node->left->subTreeSize 個 nodes；後續要找的序號扣減成 k - node->left->subTreeSize。
			// 右側含有 node 本身，因此交給 right 子節點時序號 需再扣 1。
			// 分割後扇出的 rear 是 本 node 分割的 rear part。 分割產出的 front part 是 本 node 新的右子節點。
			// 交右側子節點進行分割，序號經扣減，產出的 front 成為 本node的新右子節點，rear是本 node 分割的 右後半部。
			split(node->right, k - leftCount - 1, &node->right, rearPart);
			// 更新 size (只有右側子樹變動）
			node->groupSize = ( node->right ? node->right->groupSize : 0 )+
				( node->left ? node->left->groupSize : 0  ) + 1;
			node->groupTime = (node->right ? node->right->groupTime : 0) +
				(node->left ? node->left->groupTime : 0) + node->time;
		}
		else
		{
			// no right child
			*frontPart = node; // return itself as the front part
			*rearPart = 0; // no rear part
		}
	}
	else
	// k-th node is in the left subtree
	{
		// front part 在左側子樹上，分出後，本 node變成 rear part 
		if (node->left)
		{
			*rearPart = node;

			// 由左側子節點找 k 序位節點分割。
			// 分割出的左側成為本node分割的 左前 front part。分割出的後側，成為本node更新的左子節點。
			split(node->left, k, frontPart, &node->left); //只剩左子樹要分割
			// 更新 size (只有左側子樹變動）
			node->groupSize = ( node->right ? node->right->groupSize : 0  ) +
				(node->left ? node->left->groupSize : 0 ) + 1;
			node->groupTime = (node->right ? node->right->groupTime : 0) +
				(node->left ? node->left->groupTime : 0) + node->time;
		}
		else
		{
			// no left child
			*rearPart = node; // return itself as the rear part
			*frontPart = 0; // no front part
		}
	}
}


void inverse(SmtpNode* node)
{
	if (node->left == 0 && node->right == 0) return;
	SmtpNode* temp = node->left;
	node->left = node->right;
	node->right = temp;
	if (node->left) inverse(node->left);
	if (node->right) inverse(node->right);
}


SmtpNode* recursiveConstruct(int start, int end)
{
	if (start > end) return 0;
	int mid = ( start + end ) / 2;
	SmtpNode* left = recursiveConstruct(start, mid - 1);
	array[mid].left = left;
	array[mid].right = recursiveConstruct(mid + 1, end);
	return &array[mid];
}
 

unsigned long long recursiveSuppressTime(SmtpNode* node )
{
	// TimeLimit
	if (node->time > TimeLimit) node->time = TimeLimit;
	node->groupTime = node->time;
	if (node->left)
	{
		node->groupTime += recursiveSuppressTime(node->left);
	}
	if (node->right)
	{
		node->groupTime += recursiveSuppressTime(node->right);
	}
	return node->groupTime;
}


int recursiveAssignChildrenPriority(SmtpNode* node, unsigned long long *total)
{
	unsigned long long subTotal = 0;
	node->groupSize = 1;
	node->groupTime = node->time;
	if (node->left)
	{
		do node->left->priority = rand();
		while (node->left->priority >= node->priority);
		node->groupSize += recursiveAssignChildrenPriority(node->left, &subTotal);
		node->groupTime += subTotal;
	}
	if (node->right)
	{
		do node->right->priority = rand();
		while (node->right->priority >= node->priority);
		node->groupSize += recursiveAssignChildrenPriority(node->right, &subTotal);
		node->groupTime += subTotal;
	}
	*total = node->groupTime;
	return node->groupSize;
}


void main()
{
	SmtpNode* front, * rear, * head, * middle, * tail, * newOne;
	int cmd, p, k, l, r, x, y;
	unsigned long long totalTime;

	scanf("%d %d", &N, &Q);
	// Create N nodes associated with random priority
	array = malloc(sizeof(SmtpNode) * (N));
	for (int i = 0; i < N; i++)
	{
		array[i].left = array[i].right = 0;
		scanf("%d", &array[i].time);
	}

	root = recursiveConstruct(0, N-1);
	root->priority = rand();
	root->groupSize = recursiveAssignChildrenPriority(root, &totalTime );

	for (int i = 0; i < Q; i++)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case -1:
			inverse(root);
			inverse(root);
		case 0:
			scanf("%d", &p);
			split(root,p, &front, &rear);
			root = merge(front, rear);
			break;
		case 1: // add a new server to location p with boot time k
			newOne = malloc(sizeof(SmtpNode));
			newOne->priority = rand(); newOne->left = newOne->right = 0;
			newOne->groupSize = 1;
			scanf("%d %d", &p, &newOne->time);
			newOne->groupTime = newOne->time;

			if (p == 0) root = merge(newOne, root);
			else if (p == N) root = merge(root, newOne);
			else
			{
				split(root, p, &front, &rear);
				front =	merge(front, newOne);
				root = merge(front, rear);
			}
			N = N + 1;
			break;
		case 2: // Retire a machine at p
			scanf("%d", &p);
			if (p == N)
			{
				split(root, p - 1, &front, &rear);
				root = front;
			}
			else if (p == 1)
			{
				split(root, p, &front, &rear);
				root = rear;
			}
			else
			{
				split(root, p, &front, &tail);
				split(front, p - 1, &head, &middle);
				root = merge(head, tail);
			}
			N = N - 1;
			break;
		case 3: // Reverse the orders
			scanf("%d %d", &l, &r);
			split(root, r, &front, &tail);
			split(front, l-1, &head, &middle);
			inverse(middle);
			front = merge(head, middle);
			root = merge(front, tail);
			break;
		case 4: // Block swap
			scanf("%d %d %d %d", &l, &r, &x, &y);
			int temp;
			if (l > x)
			{
				temp = l; l = x; x = temp;
				temp = r; r = y; y = temp;
			}
			split(root, y, &head, &tail);
			split( head, x-1, & head, & rear);
			split(head, r, &head, &middle);
			split(head, l-1, &head, &front);
			root = merge(head, rear);
			root = merge(root, middle);
			root = merge(root, front);
			root = merge(root, tail);
			break;
		case 5: // trim times
			scanf("%d %d %d ", &l, &r, &TimeLimit);
			split(root, r, &front, &tail);
			split(front, l - 1, &head, &middle);
			recursiveSuppressTime(middle);
			front = merge(head, middle);
			root = merge(front, tail);
			break;
		case 6: // Print infor
			scanf("%d %d", &l, &r);
			split(root, r, &front, &tail);
			split(front, l - 1, &head, &middle);
			printf("%d\n", middle->groupTime);
			front = merge(head, middle);
			root = merge(front, tail);
			break;
		}
	}

}