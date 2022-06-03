#define _CRT_SECURE_NO_DEPRECATE
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
    int serialNum, bootTime, size, priority, timeSum;
    struct node* left, *right;
} Node;

int fileFlag = 1;
int debugFlag = 1;
int numMachineN, numRecordQ;
Node* server;

// ref: https://www.geeksforgeeks.org/treap-set-2-implementation-of-search-insert-and-delete/

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
Node* rightRotate(Node* y)
{
    Node* x = y->left, * T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
Node* leftRotate(Node* x)
{
    Node* y = x->right, * T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Return new root
    return y;
}

Node* merge(Node* a, Node* b)
{
    if (a == NULL || b == NULL) return a ? a : b;
    if (a->priority > b->priority)
    {
        a = merge(a->right, b);
        return a;
    }
    else
    {
        b = merge(a, b->left);
        return b;
    }
}

//Node*& a, Node*& b
void split(Node* t, int k, Node** a, Node** b)
{

}

/* Utility function to add a new key */
Node* newNode(int key, int bT)
{
    Node* temp = malloc(sizeof(Node));
    temp->serialNum = key;
    temp->bootTime = bT;
    temp->priority = rand();
    temp->left = temp->right = NULL;
    return temp;
}

Node* insertNode(Node* root, int key, int bT)
{
    // If root is NULL, create a new node and return it
    if (!root)
        return newNode(key, bT);

    // If key is smaller than root
    if (key <= root->serialNum)
    {
        // Insert in left subtree
        root->left = insertNode(root->left, key, bT);

        // Fix Heap property if it is violated
        if (root->left->priority > root->priority)
            root = rightRotate(root);
    }
    else  // If key is greater
    {
        // Insert in right subtree
        root->right = insertNode(root->right, key, bT);

        // Fix Heap property if it is violated
        if (root->right->priority > root->priority)
            root = leftRotate(root);
    }
    return root;
}

Node* addNode(Node* root, int place, int bT)
{

}



Node* deleteNode(Node* root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->serialNum)
        root->left = deleteNode(root->left, key);
    else if (key > root->serialNum)
        root->right = deleteNode(root->right, key);

    // IF KEY IS AT ROOT

    // If left is NULL
    else if (root->left == NULL)
    {
        Node* temp = root->right;
        free(root);
        root = temp;  // Make right child as root
    }

    // If Right is NULL
    else if (root->right == NULL)
    {
        Node* temp = root->left;
        free(root);
        root = temp;  // Make left child as root
    }

    // If key is at root and both left and right are not NULL
    else if (root->left->priority < root->right->priority)
    {
        root = leftRotate(root);
        root->left = deleteNode(root->left, key);
    }
    else
    {
        root = rightRotate(root);
        root->right = deleteNode(root->right, key);
    }

    return root;
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

void query()
{

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


// A utility function to print tree
void printTreap(Node* root)
{
    if (root)
    {
        printTreap(root->left);
        printf("SN: %d | bT: %d | pri: %d ", root->serialNum, root->bootTime, root->priority);
        if (root->left)
            printf("| left: %d ", root->left->bootTime);
        if (root->right)
            printf("| right: %d ", root->right->bootTime);
        printf("\n");
        printTreap(root->right);
    }
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
    for (size_t i = 1; i < numMachineN + 1; i++)
    {
        if (fileFlag)
            sr = fscanf(ptr, "%d", &bT);
        else
            sr = scanf("%d", &bT);
        server = insertNode(server, i, bT);
    }
    // high priority at top
    if (debugFlag)
        printTreap(server);

    int type;
    int p, k, l, r, x, y;
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
            addNode(server, p, k);
            break;
        case 2:
            if (fileFlag)
                sr = fscanf(ptr, "%d", &p);
            else
                sr = scanf("%d", &p);
            // p: is the serial number
            deleteNode(server, p);
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
            query();
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