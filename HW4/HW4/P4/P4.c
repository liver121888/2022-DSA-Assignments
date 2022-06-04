#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)

typedef struct node {
    int priority, serialNum, size;
    long long bootTime, timeSum, lazy;
    struct node* left, *right;
} Node;

int fileFlag = 1;
int debugFlag = 1;
int numMachineN, numRecordQ;
Node* server;

// ref: https://www.geeksforgeeks.org/treap-set-2-implementation-of-search-insert-and-delete/
//      https://oemiliatano.github.io/2020/04/19/Treap/

// A utility function to print tree
void printTreap(Node* root, bool isFirst)
{
    if (isFirst)
        printf("====printTreap====\n");

    if (root)
    {
        printTreap(root->left, false);
        printf("SN: %d | bT: %lld | pri: %d | size: %d | lazy: %lld | timeSum: %lld ", root->serialNum,
            root->bootTime, root->priority, root->size, root->lazy, root->timeSum);
        if (root->left)
            printf("| left: %d ", root->left->serialNum);
        if (root->right)
            printf("| right: %d ", root->right->serialNum);
        printf("\n");
        printTreap(root->right, false);
    }
}

int getSize(Node* t)
{
    int returnValue = t == NULL ? 0 : t->size;
    return returnValue;
}

long long getTimeSum(Node* t)
{
    long long returnValue = t == NULL ? 0 : t->timeSum;
    return returnValue;
}

void pull(Node* t)
{
    if (t == NULL)
        return;
    t->size = 1 + getSize(t->left) + getSize(t->right);
    t->timeSum = t->bootTime + getTimeSum(t->left) + getTimeSum(t->right);
}

void push(Node* t)
{
    if (t == NULL)
        return;
    t->bootTime += t->lazy;
    t->timeSum += t->lazy;
    if (t->left != NULL)
        t->left->lazy += t->lazy;
    if (t->right != NULL)
        t->right->lazy += t->lazy;
    t->lazy = 0;
}




Node* tmp;
Node* merge(Node* a, Node* b)
{
    if (a == NULL || b == NULL) return a ? a : b;
    if (a->priority > b->priority)
    {
        tmp = merge(a->right, b);
        a->right = tmp;
        pull(a);
        return a;
    }
    else
    {
        tmp = merge(a, b->left);
        b->left = tmp;
        pull(b);
        return b;
    }
}

//Node*& a, Node*& b
bool flag = 1;
void split(Node* t, int k, Node** a, Node** b)
{
    if (t == NULL)
    {
        *a = *b = NULL;
        return;
    }

    if (!flag)
    {
        if (t->serialNum <= k)
        {
            *a = t;
            split(t->right, k, &((*a)->right), b);
        }
        else
        {
            *b = t;
            split(t->left, k, a, &((*b)->left));
        }
    }
    else
    {
        push(t);
        //此節點的左子樹數量大於等於k
        if (getSize(t->left) >= k)
        {
            *b = t;
            push(*b);
            //將b指向整個樹，向左子樹處理。
            split(t->left, k, a, &((*b)->left));
            pull(*b);
        }
        else
        {
            *a = t;
            push(*a);
            split(t->right, k - getSize(t->left) - 1, &((*a)->right), b);
            pull(*a);
        }
    }
}

/* Utility function to add a new key */
Node* newNode(int key, int bT)
{
    Node* temp = malloc(sizeof(Node));
    temp->serialNum = key;
    temp->priority = rand();
    temp->left = temp->right = NULL;
    temp->size = 1;
    temp->lazy = 0;
    temp->bootTime = bT;
    temp->timeSum = bT;
    return temp;
}

int kth(Node* t, int k)
{
    int lsz = getSize(t->left) + 1;
    if (lsz < k)
        return kth(t->right, k - lsz);
    else if (lsz == k)
        return t->serialNum;
    else
        return kth(t->left, k);
}

Node* insertNode(Node* root, int key, int bT)
{
    Node* lt = 0, * rt = 0;
    split(root, key, &lt, &rt);
    return merge(merge(lt, newNode(key, bT)), rt);
}



void updateKey(Node* root, bool isPlus)
{
    // Base Cases: root is null or key is present at root
    if (root == NULL)
        return;
    updateKey(root->left, isPlus);
    if (isPlus)
        root->serialNum++;
    else
        root->serialNum--;
    updateKey(root->right, isPlus);
}


Node* addNode(Node* root, int place, int bT)
{
    Node* temp = 0, * temp2 = 0;
    Node* lt = 0, * rt = 0;
    split(root, place, &lt, &rt);
    temp2 = newNode(place + 1, bT);
    updateKey(rt, true);
    temp = merge(merge(lt, temp2), rt);
    numMachineN++;
    return temp;
}



Node* deleteNode(Node* root, int key)
{
    Node* temp = 0;
    Node* lt = 0, * rt = 0, *lt2, *rt2;
    split(root, key, &lt, &rt);
    //split(root, key - 1, &lt, &rt);
    split(lt, key - 1, &lt2, &rt2);
    updateKey(rt, false);
    temp = merge(lt2, rt);
    numMachineN--;
    return temp;
}

void reverseNode()
{

}

void swapNode()
{

}

void rebootNode()
{

}

Node* query(Node* root, int l, int r, long long* q)
{
    Node *lt, *rt, *lt2, *rt2;
    Node* nuroot;
    split(root, l - 1, &lt, &rt);
    split(rt, r, &lt2, &rt2);
    *q = lt2->timeSum;
    nuroot = merge (lt, merge(lt2, rt2));
    return nuroot;
}

// C function to search a given key in a given BST
Node* search(Node* root, int key)
{
    // Base Cases: root is null or key is present at root
    if (root == NULL || root->serialNum == key)
        return root;

    // Key is greater than root's key
    if (root->serialNum < key)
        return search(root->right, key);

    // Key is smaller than root's key
    return search(root->left, key);
}


int main() {
    srand(7777777);
    int sr;
    FILE* ptr = 0;
    if (fileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P4\\0.in", "r");
        sr = fscanf(ptr, "%d %d", &numMachineN, &numRecordQ);
    }
    else
        sr = scanf("%d %d", &numMachineN, &numRecordQ);


    server = NULL;
    int bT = 0;
    for (int i = 1; i < numMachineN + 1; i++)
    {
        if (fileFlag)
            sr = fscanf(ptr, "%d", &bT);
        else
            sr = scanf("%d", &bT);
        server = insertNode(server, i, bT);
    }
    // high priority at top
    if (debugFlag)
        printTreap(server, true);


    int type;
    int p, k, l, r, x, y;
    long long q;
    for (int i = 1; i < (numRecordQ + 1); i++)
    {
        if (fileFlag)
            sr = fscanf(ptr, "%d", &type);
        else
            sr = scanf("%d", &type);

        switch (type)
        {
        case 1:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d", &p, &k);
            else
                sr = scanf("%d %d", &p, &k);
            // insert between p, p+1, with bT = k
            server = addNode(server, p, k);
            if (debugFlag)
                printTreap(server, true);
            break;
        case 2:
            if (fileFlag)
                sr = fscanf(ptr, "%d", &p);
            else
                sr = scanf("%d", &p);
            // p: is the serial number
            server = deleteNode(server, p);
            if (debugFlag)
                printTreap(server, true);
            break;
        case 3:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d", &l, &r);
            else
                sr = scanf("%d %d", &l, &r);
            reverseNode();
            break;
        case 4:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d %d %d", &l, &r, &x, &y);
            else
                sr = scanf("%d %d %d %d", &l, &r, &x, &y);
            swapNode();
            break;
        case 5:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d %d", &l, &r, &k);
            else
                sr = scanf("%d %d %d", &l, &r, &k);
            rebootNode();
            break;
        case 6:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d", &l, &r);
            else
                sr = scanf("%d %d", &l, &r);
            server = query(server, l, r, &q);
            printf("%lld\n", q);
            break;
        default:
            break;
        }
    }
    if (fileFlag)
    {
        fclose(ptr);
    }
    return 0;
}
#pragma warning(pop)