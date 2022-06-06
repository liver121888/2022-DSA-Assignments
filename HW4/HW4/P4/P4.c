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
    int priority, size;
    bool rev;
    long long bootTime, timeSum, lazy;
    struct node* left, *right;
} Node;

int fileFlag = 1;
int debugFlag = 1;
int numMachineN, numRecordQ;
Node* server;

// ref: https://www.geeksforgeeks.org/treap-set-2-implementation-of-search-insert-and-delete/
//      https://oemiliatano.github.io/2020/04/19/Treap/
//      https://www.796t.com/content/1580044082.html
//      https://youtu.be/erKlLEXLKyY
//      https://youtu.be/8Rej5VVHVkY


// A utility function to print tree
void printTreap(Node* root, bool isFirst)
{
    if (isFirst)
        printf("====printTreap====\n");

    if (root)
    {
        printTreap(root->left, false);
        printf("bT: %lld | priority: %d | size: %d | lazy: %lld | timeSum: %lld ", 
            root->bootTime, root->priority, root->size, root->lazy, root->timeSum);
        if (root->left)
            printf("| left: %lld ", root->left->bootTime);
        if (root->right)
            printf("| right: %lld ", root->right->bootTime);
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

Node* tmp;
void push(Node* t)
{
    if (t == NULL)
        return;
    if (t->rev)
    {
        tmp = t->left;
        t->left = t->right;
        t->right = tmp;
        if (t->left)
            t->left->rev ^= 1;
        if (t->right)
            t->right->rev ^= 1;
        t->rev = 0;
    }
    t->bootTime += t->lazy;
    t->timeSum += t->lazy;
    if (t->left != NULL)
        t->left->lazy += t->lazy;
    if (t->right != NULL)
        t->right->lazy += t->lazy;
    t->lazy = 0;
}

Node* merge(Node* a, Node* b)
{
    if (a == NULL || b == NULL) 
        return a ? a : b;
    if (a->priority > b->priority)
    {
        push(a);
        a->right = merge(a->right, b);
        pull(a);
        return a;
    }
    else
    {
        push(b);
        b->left = merge(a, b->left);
        pull(b);
        return b;
    }
}

void split(Node* t, int k, Node** a, Node** b)
{
    if (t == NULL)
    {
        *a = *b = NULL;
        return;
    }
    push(t);
    // the size of this node's left subtree is bigger than k
    if (getSize(t->left) >= k)
    {
        *b = t;
        push(*b);
        // point b to the whole tree, hand over to left subtree
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

Node* newNode(int bT)
{
    Node* temp = malloc(sizeof(Node));
    temp->priority = rand();
    temp->left = temp->right = NULL;
    temp->size = 1;
    temp->lazy = 0;
    temp->rev = 0;
    temp->bootTime = bT;
    temp->timeSum = bT;
    return temp;
}

//int kth(Node* t, int k)
//{
//    //int lsz = getSize(t->left) + 1;
//    //if (lsz < k)
//    //    return kth(t->right, k - lsz);
//    //else if (lsz == k)
//    //    return t->serialNum;
//    //else
//    //    return kth(t->left, k);
//}

Node* insertNode(Node* root, int key, int bT)
{
    Node* lt = 0, * rt = NULL;
    split(root, key, &lt, &rt);
    return merge(merge(lt, newNode(bT)), rt);
}

Node* addNode(Node* root, int place, int bT)
{
    Node* temp = 0, * temp2 = 0;
    Node* lt = 0, * rt = 0;
    if (place == 0 || place ==  numMachineN)
        split(root, place, &lt, &rt);
    else
        split(root, place, &lt, &rt);
    temp2 = newNode(bT);
    temp = merge(merge(lt, temp2), rt);
    numMachineN++;
    return temp;
}


Node* deleteNode(Node* root, int key)
{
    Node* temp = 0;
    Node* lt = 0, * rt = 0, *lt2, *rt2;
    split(root, key, &lt, &rt);
    split(lt, key - 1, &lt2, &rt2);
    temp = merge(lt2, rt);
    numMachineN--;
    return temp;
}

Node* reverseNode(Node* root, int l, int r)
{
    Node* lt = 0, * rt = 0, * lt2, * rt2;
    split(root, l - 1, &lt, &rt);
    split(rt, r - l + 1, &lt2, &rt2);
    lt2->rev ^= 1;
    return merge(merge(lt, lt2), rt2);
}

Node* swapNode(Node* root, int l, int r, int x, int y)
{
    Node* lt = 0, * rt = 0, * lt2, * rt2, * lt3, *rt3, *lt4, *rt4;
    split(root, l - 1, &lt, &rt);
    split(rt, r - l + 1, &lt2, &rt2);

    split(rt2, x - r - 1, &lt3, &rt3);
    split(rt3, y - x + 1, &lt4, &rt4);
    return merge(merge(merge(merge(lt, lt4), lt3), lt2), rt4);
}

Node* upgradeNode(Node* root, int l, int r, int k)
{
    //Node* lt, * rt, * lt2, * rt2;
    //split(root, l - 1, &lt, &rt);
    //split(rt, r - l + 1, &lt2, &rt2);

    //inorderupgrade(lt2, k);

    //return merge(lt, merge(lt2, rt2));
}

Node* query(Node* root, int l, int r, long long* q)
{
    Node *lt, *rt, *lt2, *rt2;
    Node* nuroot;
    split(root, l - 1, &lt, &rt);
    //printTreap(rt, true);
    split(rt, r - l + 1, &lt2, &rt2);
    *q = lt2->timeSum;
    nuroot = merge(lt, merge(lt2, rt2));
    return nuroot;
}

int inorderupgrade(Node* root, int target)
{
    // Base Cases: root is null or key is present at root
    if (root == NULL)
        return 0;
    inorderupgrade(root->left, target);
    inorderupgrade(root->right, target);
}


int main() {
    srand(7777777);
    int sr;
    FILE* ptr = 0;
    if (fileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P4\\4.in", "r");
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
            server = reverseNode(server, l, r);
            break;
        case 4:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d %d %d", &l, &r, &x, &y);
            else
                sr = scanf("%d %d %d %d", &l, &r, &x, &y);
            server = swapNode(server, l, r, x, y);
            break;
        case 5:
            if (fileFlag)
                sr = fscanf(ptr, "%d %d %d", &l, &r, &k);
            else
                sr = scanf("%d %d %d", &l, &r, &k);
            //server = upgradeNode(server, l, r, k);
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
        if (debugFlag)
            printTreap(server, true);
    }
    if (fileFlag)
    {
        fclose(ptr);
    }
    return 0;
}
#pragma warning(pop)