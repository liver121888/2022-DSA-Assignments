#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#pragma warning(push)
//#pragma warning(disable:6385)
//#pragma warning(disable:6386)
//#pragma warning(disable:6011)

typedef struct node {
    long long priority, size;
    bool rev;
    long long bootTime, timeSum, lazy;
    long long maxBT, secondBT;
    long long maxCnt;
    struct node* left, *right;
} Node;

int fileFlag = 1;
int debugFlag = 0;
long long numMachineN, numRecordQ;
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
        printf("bT: %lld | pri: %lld | sz: %lld | lazy: %lld | tS: %lld | mBT: %lld | sBT: %lld | mC: %lld ", 
            root->bootTime, root->priority, root->size, root->lazy, root->timeSum,
            root->maxBT, root->secondBT, root->maxCnt);
        if (root->left)
            printf("| left: %lld, l_pri: %lld ", root->left->bootTime, root->left->priority);
        if (root->right)
            printf("| right: %lld, r_pri: %lld ", root->right->bootTime, root->right->priority);
        printf("\n");
        printTreap(root->right, false);
    }
}

long long getSize(Node* t)
{
    long long returnValue = t == NULL ? 0 : t->size;
    return returnValue;
}

long long getTimeSum(Node* t)
{
    long long returnValue = t == NULL ? 0 : t->timeSum;
    return returnValue;
}

long long getMaxBT(Node* t)
{
    long long returnValue = t == NULL ? 0 : t->maxBT;
    return returnValue;
}

long long getMaxCnt(Node* t)
{
    long long returnValue = t == NULL ? 0 : t->maxCnt;
    return returnValue;
}

long long getSecondBT(Node* t)
{
    long long returnValue = t == NULL ? 0 : t->secondBT;
    return returnValue;
}


void jiry(Node* t, long long k)
{
    if (t->bootTime > k)
    {
        t->bootTime = k;

    }
    else
    {
        t->bootTime = t->bootTime;
    }
    t->timeSum -= t->maxCnt * (t->maxBT - k);
    t->maxBT = k;

    if (t->left != NULL)
    {
        if (t->left->lazy == 0)
            t->left->lazy = k;
        else
            //t->left->lazy = t->lazy;
            t->left->lazy = t->left->lazy < k ? t->left->lazy : k;
    }
    if (t->right != NULL)
    {
        if (t->right->lazy == 0)
            t->right->lazy = k;
        else
            //t->right->lazy = t->lazy
            t->right->lazy = t->right->lazy < k ? t->right->lazy : k;
    }
    t->lazy = 0;
}



// At first I try to use only one push function incorporating lazy and rev operations, 
// but that is chaotic and hard to debug, not to mention sometimes we need to check 
// whether we should do lazy operations, so I discuss with Nick and change it to two functions 
// for better reusability.
Node* tmp;
long long vtmp;
void pushLazy(Node* t)
{
    if (t == NULL)
        return;
    if (t->lazy == 0)
        return;

    if (t->maxBT <= t->lazy)
    {
        t->lazy = 0;
        return;
    }
    else
    {
        jiry(t, t->lazy);
        return;
    }
}


void pushRev(Node* t)
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
}



// ref: NICK, mine fighting pull is too hard to complete, and hard to debug
// I discuss with my classmate Nick and I appreciate the divide and conquer method Nick used.
void pull(Node* t)
{
    if (t == NULL)
        return;
    pushRev(t);
    pushLazy(t);
    // reset to calculate
    //t->size = 1;
    //t->timeSum = t->bootTime;
    // TODO: reset below or not? no.
    t->maxBT = t->bootTime;
    t->secondBT = 0;
    t->maxCnt = 1;
    if (t->left)
    {
        pushLazy(t->left);
    }
    if (t->right)
    {
        pushLazy(t->right);
    }

    t->size = 1 + getSize(t->left) + getSize(t->right);
    t->timeSum = t->bootTime + getTimeSum(t->left) + getTimeSum(t->right);
    // separate to two groups to judge
    if (t->left)
    {
        if (t->left->maxBT > t->maxBT)
        {
            t->secondBT = t->left->secondBT > t->maxBT ? t->left->secondBT : t->maxBT;
            t->maxBT = t->left->maxBT;
            t->maxCnt = t->left->maxCnt;
        }
        else if (t->left->maxBT == t->maxBT)
        {
            t->maxCnt += t->left->maxCnt;
            t->secondBT = t->left->secondBT > t->secondBT ? t->left->secondBT : t->secondBT;
        }
        else
        {
            t->secondBT = t->left->maxBT > t->secondBT ? t->left->maxBT : t->secondBT;
        }
    }

    if (t->right)
    {
        if (t->right->maxBT > t->maxBT)
        {
            t->secondBT = t->right->secondBT > t->maxBT ? t->right->secondBT : t->maxBT;
            t->maxBT = t->right->maxBT;
            t->maxCnt = t->right->maxCnt;
        }
        else if (t->right->maxBT == t->maxBT)
        {
            t->maxCnt += t->right->maxCnt;
            t->secondBT = t->right->secondBT > t->secondBT ? t->right->secondBT : t->secondBT;
        }
        else
        {
            t->secondBT = t->right->maxBT > t->secondBT ? t->right->maxBT : t->secondBT;
        }
    }
}

//long long m, noZero;
//long long mc;
//long long maxtmp;
//long long maxcnttmp;
//long long secondtmp;
//void pullold(Node* t)
//{
//    if (t == NULL)
//        return;
//    push(t);
//    if (t->left)
//    {
//        push(t->left);
//    }
//    if (t->right)
//    {
//        push(t->right);
//    }
//
//    t->size = 1 + getSize(t->left) + getSize(t->right);
//    t->timeSum = t->bootTime + getTimeSum(t->left) + getTimeSum(t->right);
//    // update the data for operation 5
//    // left and right, fight!
//    maxtmp = getMaxBT(t->left) > getMaxBT(t->right) ? getMaxBT(t->left) : getMaxBT(t->right);
//    bool isleft = getMaxBT(t->left) > getMaxBT(t->right) ? true : false;
//    bool isequal = getMaxBT(t->left) == getMaxBT(t->right) ? true : false;
//    if (isequal)
//    {
//        maxcnttmp = getMaxCnt(t->left) + getMaxCnt(t->right);
//    }
//    else
//    {
//        if (isleft)
//        {
//            maxcnttmp = getMaxCnt(t->left);
//        }
//        else
//        {
//            maxcnttmp = getMaxCnt(t->right);
//        }
//    }
//
//    if (isequal)
//    {
//        bool issecondequal = getSecondBT(t->left) == getSecondBT(t->right) ? true : false;
//        if (issecondequal)
//        {
//            secondtmp = getSecondBT(t->left);
//        }
//        else
//        {
//            bool issecondleft = getSecondBT(t->left) > getSecondBT(t->right) ? true : false;
//            if (issecondleft)
//            {
//                secondtmp = getSecondBT(t->left);
//            }
//            else
//            {
//                secondtmp = getSecondBT(t->right);
//            }
//        }
//    }
//    else
//    {
//
//        if (isleft)
//        {
//            // left is bigger, select right as second
//            // compare the left's second and the max of right
//            secondtmp = getSecondBT(t->left) > getMaxBT(t->right) ? getSecondBT(t->left) : getMaxBT(t->right);
//        }
//        else
//        {
//            // right is bigger, select left as second
//            //             //secondtmp = getMaxBT(t->left);
//            // compare the right's second and the max of left
//            secondtmp = getSecondBT(t->right) > getMaxBT(t->left) ? getSecondBT(t->right) : getMaxBT(t->left);
//        }
//    }
//
//    // root and bigger, fight!
//    bool isroot = t->bootTime > maxtmp ? true : false;
//    isequal = t->bootTime == maxtmp ? true : false;
//    if (isequal)
//    {
//        t->maxBT = t->bootTime;
//    }
//    else
//    {
//        if (isroot)
//        {
//            t->maxBT = t->bootTime;
//        }
//        else
//            t->maxBT = maxtmp;
//    }
//
//    if (isequal)
//    {
//        t->maxCnt = maxcnttmp + 1;
//    }
//    else
//    {
//        if (isroot)
//        {
//            t->maxCnt = 1;
//        }
//        else
//        {
//            t->maxCnt = maxcnttmp;
//        }
//
//    }
//    if (isequal)
//    {
//        t->secondBT = secondtmp;
//    }
//    else
//    {
//        if (isroot)
//        {
//            t->secondBT = maxtmp;
//        }
//        else
//        {
//            t->secondBT = t->bootTime > secondtmp ? t->bootTime : secondtmp;
//        }
//    }
//}

// ref: discord, three types of situation
void upgradeCheck(Node* t, long long k)
{
    if (t == NULL)
        return;
    pushRev(t);
    pushLazy(t);
    if (k >= t->maxBT)
    {
        return;
        // does nothing
    }
    else if (k >= t->secondBT)
    {
        //t->timeSum -= t->maxCnt * (t->maxBT - k);
        jiry(t, k);
        pull(t);
        // whether to go down? no
    }
    else if (k < t->secondBT)
    {
        //t->lazy = k;
        //recursiveUpgradeMODI(t);
        t->bootTime = t->bootTime < k ? t->bootTime : k;
        upgradeCheck(t->left, k);
        upgradeCheck(t->right, k);
        pull(t);
    }
    else
    {
        if (debugFlag)
        {
            printf("wrong");
        }
    }
}


Node* merge(Node* a, Node* b)
{
    if (a == NULL || b == NULL) 
        return a ? a : b;
    if (a->priority > b->priority)
    {
        pushRev(a);
        pushLazy(a);
        a->right = merge(a->right, b);
        pull(a);
        return a;
    }
    else
    {
        pushRev(b);
        pushLazy(b);
        b->left = merge(a, b->left);
        pull(b);
        return b;
    }
}

void split(Node* t, long long k, Node** a, Node** b)
{
    if (t == NULL)
    {
        *a = *b = NULL;
        return;
    }
    pushRev(t);
    pushLazy(t);
    // the size of this node's left subtree is bigger than k
    if (getSize(t->left) >= k)
    {
        *b = t;
        // maybe can be comment
        pushRev(*b);
        pushLazy(*b);
        // point b to the whole tree, hand over to left subtree
        split(t->left, k, a, &((*b)->left));
        pull(*b);
    }
    else
    {
        *a = t;
        pushRev(*a);
        pushLazy(*a);
        split(t->right, k - getSize(t->left) - 1, &((*a)->right), b);
        pull(*a);
    }
}

Node* newNode(long long bT)
{
    Node* temp = malloc(sizeof(Node));
    temp->priority = rand();
    temp->left = temp->right = NULL;
    temp->size = 1;
    temp->lazy = 0;
    temp->rev = 0;
    temp->bootTime = bT;
    temp->timeSum = bT;
    temp->maxBT = bT;
    temp->secondBT = 0;
    temp->maxCnt = 1;
    return temp;
}

Node* insertNode(Node* root, long long key, long long bT)
{
    Node* lt = 0, * rt = NULL;
    split(root, key, &lt, &rt);
    return merge(merge(lt, newNode(bT)), rt);
}

Node* addNode(Node* root, long long place, long long bT)
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


Node* deleteNode(Node* root, long long key)
{
    Node* temp = 0;
    Node* lt = 0, * rt = 0, *lt2, *rt2;
    split(root, key, &lt, &rt);
    split(lt, key - 1, &lt2, &rt2);
    temp = merge(lt2, rt);
    numMachineN--;
    return temp;
}

Node* reverseNode(Node* root, long long l, long long r)
{
    Node* lt = 0, * rt = 0, * lt2, * rt2;
    split(root, l - 1, &lt, &rt);
    split(rt, r - l + 1, &lt2, &rt2);
    lt2->rev ^= 1;
    return merge(merge(lt, lt2), rt2);
}

long long tmpv;
Node* swapNode(Node* root, long long l, long long r, long long x, long long y)
{
    if (l > x)
    {
        // swap the numbers
        tmpv = l;
        l = x;
        x = tmpv;
        tmpv = r;
        r = y;
        y = tmpv;
    }
    Node* lt = 0, * rt = 0, * lt2, * rt2, * lt3, *rt3, *lt4, *rt4;
    split(root, l - 1, &lt, &rt);
    split(rt, r - l + 1, &lt2, &rt2);
    split(rt2, x - r - 1, &lt3, &rt3);
    split(rt3, y - x + 1, &lt4, &rt4);
    return merge(merge(merge(merge(lt, lt4), lt3), lt2), rt4);
}

Node* upgradeNode(Node* root, long long l, long long r, long long k)
{
    Node* lt, * rt, * lt2, * rt2;
    split(root, l - 1, &lt, &rt);
    split(rt, r - l + 1, &lt2, &rt2);
    upgradeCheck(lt2, k);
    return merge(lt, merge(lt2, rt2));
}


Node* query(Node* root, long long l, long long r, long long* q)
{
    Node *lt, *rt, *lt2, *rt2;
    Node* nuroot;
    split(root, l - 1, &lt, &rt);
    split(rt, r - l + 1, &lt2, &rt2);
    *q = lt2->timeSum;
    nuroot = merge(lt, merge(lt2, rt2));
    return nuroot;
}



int main() {
    srand(7777777);
    long long sr;
    FILE* ptr = 0;
    if (fileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P4\\7.in", "r");
        sr = fscanf(ptr, "%lld %lld", &numMachineN, &numRecordQ);
    }
    else
        sr = scanf("%lld %lld", &numMachineN, &numRecordQ);


    server = NULL;
    long long bT = 0;
    for (long long i = 1; i < numMachineN + 1; i++)
    {
        if (fileFlag)
            sr = fscanf(ptr, "%lld", &bT);
        else
            sr = scanf("%lld", &bT);
        server = insertNode(server, i, bT);
    }
    // high priority at top
    if (debugFlag)
    {
        printf("Initial server: \n");
        printTreap(server, true);
    }


    long long type;
    long long p, k, l, r, x, y;
    long long q;
    for (long long i = 1; i < (numRecordQ + 1); i++)
    {
        if (fileFlag)
            sr = fscanf(ptr, "%lld", &type);
        else
            sr = scanf("%lld", &type);

         switch (type)
        {
        case 1:
            if (fileFlag)
                sr = fscanf(ptr, "%lld %lld", &p, &k);
            else
                sr = scanf("%lld %lld", &p, &k);
            // insert between p, p+1, with bT = k
            server = addNode(server, p, k);
            break;
        case 2:
            if (fileFlag)
                sr = fscanf(ptr, "%lld", &p);
            else
                sr = scanf("%lld", &p);
            server = deleteNode(server, p);
            break;
        case 3:
            if (fileFlag)
                sr = fscanf(ptr, "%lld %lld", &l, &r);
            else
                sr = scanf("%lld %lld", &l, &r);
            server = reverseNode(server, l, r);
            break;
        case 4:
            if (fileFlag)
                sr = fscanf(ptr, "%lld %lld %lld %lld", &l, &r, &x, &y);
            else
                sr = scanf("%lld %lld %lld %lld", &l, &r, &x, &y);
            server = swapNode(server, l, r, x, y);
            break;
        case 5:
            if (fileFlag)
                sr = fscanf(ptr, "%lld %lld %lld", &l, &r, &k);
            else
                sr = scanf("%lld %lld %lld", &l, &r, &k);
            server = upgradeNode(server, l, r, k);
            break;
        case 6:
            if (fileFlag)
                sr = fscanf(ptr, "%lld %lld", &l, &r);
            else
                sr = scanf("%lld %lld", &l, &r);
            server = query(server, l, r, &q);
            printf("%lld\n", q);
            break;
        default:
            break;
        }
        if (debugFlag)
        {
            printf("op: %lld, type: %lld \n", i, type);
            printTreap(server, true);
            printf("--++--++--\n");
        }
    }
    if (fileFlag)
    {
        fclose(ptr);
    }
    return 0;
}
#pragma warning(pop)