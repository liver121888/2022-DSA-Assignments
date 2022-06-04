#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct operate
{
    char op[3];
    int v1;
    int v2;
} Operate;

typedef struct disjointSet
{
    int leaderHash;
    int size;
} DisjointSet;

typedef struct dsh
{
    int originalLeader;
    int modiIndex1;
    int modiIndex2;
    int originalSize;
} DSH;

typedef struct opday
{
    Operate operate;
    int day;
    int childNumber;
} OPDay;

typedef struct node
{
    int day;
    struct node* next;
} Node;

typedef struct graph {
    int nodeNumber;
    bool* visited;
    Node** adjLists;
}Graph;

int fileFlag = 1;
int debugFlag = 1;
int shopNumberInitial;
int shopNumber;
int* shopNumberHistory;
int daysM;
DisjointSet* ds;
DSH* dsHistory;
OPDay** T;
Graph* G;

void makeset(int i)
{
    // TODO: Initialize a set with hash value
    DisjointSet* n = malloc(sizeof(DisjointSet));
    if (n != NULL)
    {
        n->leaderHash = i;
        n->size = 1;
        ds[i] = *n;
    }
}

int find_set(int i)
{
    // no path compression
    if (ds[i].leaderHash != i)
        find_set(ds[i].leaderHash);
    return ds[i].leaderHash;
}

void merge(int ra, int rb, int day)
{
    // union by rank
    int a = find_set(ra), b = find_set(rb);
    if (a == b)
        return;
    else
    {
        // here we use same struct but save the original and the modified
        if (ds[a].size <= ds[b].size)
        {
            // a <= b, merge a into b
            dsHistory[day].originalLeader = ds[a].leaderHash;
            dsHistory[day].modiIndex1 = a;
            dsHistory[day].modiIndex2 = b;
            dsHistory[day].originalSize = ds[b].size;


            ds[a].leaderHash = ds[b].leaderHash;
            ds[b].size += ds[a].size;
        }
        else
        {
            // a > b, merge b into a
            dsHistory[day].originalLeader = ds[b].leaderHash;
            dsHistory[day].modiIndex1 = b;
            dsHistory[day].modiIndex2 = a;
            dsHistory[day].originalSize = ds[a].size;

            ds[b].leaderHash = ds[a].leaderHash;
            ds[a].size += ds[b].size;
        }
        shopNumber--;
    }
}

void demerge(int ra, int rb, int day)
{
    // union by rank
    int a = find_set(ra), b = find_set(rb);
    if (a != b)
        return;
    ds[dsHistory[day].modiIndex1].leaderHash = dsHistory[day].originalLeader;
    ds[dsHistory[day].modiIndex2].size = dsHistory[day].originalSize;
    shopNumber++;
}

void printShop(int d)
{
    printf("-------------------------\n");
    printf("Day: %d\n", d);
    for (int i = 1; i < shopNumberInitial + 1; i++)
        printf("S_%d = lead: %d /// size: %d, ", i, ds[i].leaderHash, ds[i].size);
    printf("\n-------------------------\n");
}

void printShopNumberHistory()
{
    printf("ShopNumberHistory: \n");
    for (int j = 0; j < daysM + 1; j++)
        if (shopNumberHistory[j] > 0)
            printf("D%d: %d, ", j, shopNumberHistory[j]);
    printf("\n++++++++\n");
}

void connect(int origin, int destination)
{
    Node* no = malloc(sizeof(Node));
    no->day = origin;
    Node* nd = malloc(sizeof(Node));
    nd->day = destination;

    no->next = G->adjLists[destination];
    G->adjLists[destination] = no;

    nd->next = G->adjLists[origin];
    G->adjLists[origin] = nd;
}

void printG()
{
    printf("##########\n");
    for (int i = 0; i < G->nodeNumber; i++) {
        Node* tmp = G->adjLists[i];
        printf("Day %d: \n", i);
        while (tmp)
        {
            printf("%d, ", tmp->day);
            tmp = tmp->next;
        }
        printf("\n");
    }
    printf("##########\n");
}


// ref: https://www.programiz.com/dsa/graph-dfs
// DFS algo
OPDay* test;
bool mode = 0;
void DFS(int vertex) {
    if (G->visited[vertex] == 1)
        return;
    G->visited[vertex] = 1;
    if (debugFlag)
        printf("Visit %d \n", vertex);
    switch (T[vertex]->operate.op[0])
    {
    case('q'):
        // save query, later print
        shopNumberHistory[vertex] = shopNumber;
        break;
    case('m'):
        merge(T[vertex]->operate.v1, T[vertex]->operate.v2, vertex);
        break;
    case('b'):
        break;
    default:
        break;
    }
    Node* temp = G->adjLists[vertex];
    while (temp != NULL)
    {
        DFS(temp->day);
        temp = temp->next;
    }
    int tmpindex = vertex + 1;
    if (tmpindex <= daysM)
        if (!(T[tmpindex]->operate.op[0] == 'b'))
            DFS(tmpindex);
    else
    {
        tmpindex = vertex;
        //while (tmpindex>=1 && !(T[tmpindex]->operate.op[0] == 'b'))
        while (tmpindex >0 && !(T[tmpindex]->operate.op[0] == 'b'))
        {
            switch (T[tmpindex]->operate.op[0])
            {
            case('q'):
                break;
            case('m'):
                demerge(T[tmpindex]->operate.v1, T[tmpindex]->operate.v2, tmpindex);
                break;
            default:
                break;
            }
            tmpindex--;
        }
    }
}

int main()
{
    int r;
    FILE* ptr = 0;
    if (fileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P3\\6.in", "r");
        r = fscanf(ptr, "%d %d", &shopNumberInitial, &daysM);
    }
    else
        r = scanf("%d %d", &shopNumberInitial, &daysM);
    // we ignore the space at index 0
    shopNumberHistory = malloc(sizeof(int) * (daysM + 1));
    dsHistory = malloc(sizeof(DSH) * (daysM + 1));
    for (int i = 0; i < daysM + 1; i++)
        shopNumberHistory[i] = -1;

    G = malloc(sizeof(Graph));
    G->nodeNumber = daysM + 1;
    G->adjLists = malloc((daysM + 1) * sizeof(Node*));
    G->visited = malloc((daysM + 1) * sizeof(bool));
    for (int i = 0; i < (daysM + 1); i++)
    {
        G->adjLists[i] = NULL;
        G->visited[i] = 0;
    }

    T = malloc(sizeof(OPDay*) * (daysM + 1));

    shopNumber = shopNumberInitial;
    ds = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
    for (int i = 1; i < shopNumberInitial + 1; i++)
        makeset(i);

    OPDay* n = malloc(sizeof(OPDay));
    n->day = 0;
    T[0] = n;
    char optmp[6];
    for (int i = 1; i < daysM + 1; i++)
    {
        n = malloc(sizeof(OPDay));
        n->day = i;
        if (fileFlag)
            r = fscanf(ptr, "%s", optmp);
        else
            r = scanf("%s", optmp);
        // store op in operationDays
        n->operate.op[0] = optmp[0];
        n->operate.op[1] = 0;
        switch (optmp[0])
        {
        case('q'):
            break;
        case('m'):
            if (fileFlag)
                r = fscanf(ptr, "%d %d", &n->operate.v1, &n->operate.v2);
            else
                r = scanf("%d %d", &n->operate.v1, &n->operate.v2);
            break;
        case('b'):
            if (fileFlag)
                r = fscanf(ptr, "%d", &n->operate.v1);
            else
                r = scanf("%d", &n->operate.v1);
            connect(n->operate.v1, n->day);
            break;
        default:
            break;
        }
        T[i] = n;
    }
    if (debugFlag)
        printG();
    DFS(0);
    for (int i = 0; i < daysM + 1; i++)
    {
        if (shopNumberHistory[i] > 0)
            printf("%d\n", shopNumberHistory[i]);
    }

    if (fileFlag)
        r = fclose(ptr);

    free(shopNumberHistory);
    free(ds);
    free(dsHistory);
    free(T);
    free(G);
    return 0;
}