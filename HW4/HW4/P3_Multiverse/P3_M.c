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

typedef struct operate
{
    char op[6];
    int v1;
    int v2;
} Operate;

typedef struct disjointSet
{
    int leaderHash;
    int size;
} DisjointSet;

typedef struct opday
{
    // TODO: Determine fields to use by your method
    Operate operate;
    int day;
    struct opday* next;
    int childNumber;
} OPDay;

typedef struct graph {
    int nodeNumber;
    bool* visited;
    OPDay** adjLists;
    int* nextCnt;
}Graph;

typedef struct q {
    bool isquery;
    int cnt;
}Q;

// init set
//bool* set;
int FileFlag = 1;
int DebugFlag = 0;
int shopNumberInitial;
int shopNumber;
Q* shopNumberHistory;
int daysM;
int dayBoom; 
bool* isBoom;
DisjointSet* ds;
DisjointSet** dsHistory;
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

//inline void static init(int i)
//{
//    if (!set[i]) 
//    {
//        makeset(i);
//        set[i] = 1;
//    }
//}

int find_set(int i)
{
    // TODO: Implement your find algorithm here
    if (ds[i].leaderHash != i)
    {
        ds[i].leaderHash = find_set(ds[i].leaderHash);
    }
    return ds[i].leaderHash;
}

void merge(int ra, int rb)
{

    int a = find_set(ra), b = find_set(rb);
    // TODO: Implement your union algorithm here
    if (a == b)
        return;
    else
    {

        if (ds[a].size <= ds[b].size)
        {
            // a <= b, merge a into b
            ds[a].leaderHash = ds[b].leaderHash;
            ds[b].size += ds[a].size;
        }
        else
        {
            // a > b, merge b into a
            ds[b].leaderHash = ds[a].leaderHash;
            ds[a].size += ds[b].size;
        }
        shopNumber--;
    }
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
        printf("D%d: %d, ", j, shopNumberHistory[j].cnt);
    printf("\n++++++++\n");
}

void saveState(int j)
{    
    for (int i = 0; i < (shopNumberInitial + 1); i++)
    {
        dsHistory[j][i].leaderHash =  ds[i].leaderHash;
        dsHistory[j][i].size = ds[i].size;
    }
    shopNumberHistory[j].cnt = shopNumber;
}

void readState(int j)
{
    for (int i = 0; i < (shopNumberInitial + 1); i++)
    {
        ds[i].leaderHash = dsHistory[j][i].leaderHash;
        ds[i].size = dsHistory[j][i].size;
    }
    shopNumber = shopNumberHistory[j].cnt;
}


void connect(OPDay* day1, OPDay* day2)
{
    G->nextCnt[day1->day]++;
    G->nextCnt[day2->day]++;
    // for day1, day2 we need copy by value
    OPDay* no = malloc(sizeof(OPDay));
    no->next = day1->next;
    no->day = day1->day;
    no->operate = day1->operate;

    OPDay* nd = malloc(sizeof(OPDay));
    nd->next = day2->next;
    nd->day = day2->day;
    nd->operate = day2->operate;
    
    no->next = G->adjLists[nd->day];
    G->adjLists[nd->day] = no;

    nd->next = G->adjLists[no->day];
    G->adjLists[no->day] = nd;
}

void printCity()
{
    printf("##########\n");
    for (int i = 0; i < G->nodeNumber; i++) {
        OPDay* tmp = G->adjLists[i];
        printf("Day %d: cN: %d\n", i, G->nextCnt[i]);
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
bool hitButtomFlag = false;
int boomi = 0;
void DFS(int vertex) {
    OPDay* adjList = G->adjLists[vertex];
    OPDay* temp = adjList;
    G->visited[vertex] = 1;
    if (DebugFlag)
        printf("Visited %d \n", vertex);
    //G->nextCnt[vertex] > 2
    if (isBoom[vertex] == true)
    {
        boomi = vertex;
        saveState(vertex);
    }

    while (temp != NULL) 
    {
        int connectedVertex = temp->day;
        if (G->visited[connectedVertex] == 0) 
        {
            switch (temp->operate.op[0])
            {
            case('q'):
                // save query, later print
                shopNumberHistory[connectedVertex].cnt = shopNumber;
                shopNumberHistory[connectedVertex].isquery = true;
                break;
            case('m'):
                merge(temp->operate.v1, temp->operate.v2);
                break;
            case('b'):
                readState(vertex);
                break;
            default:
                break;
            }
            if (DebugFlag)
                printShop(vertex);
            DFS(connectedVertex);
        }
        if (hitButtomFlag && boomi == vertex)
        {
            readState(vertex);
            hitButtomFlag = false;
        }
        temp = temp->next;
    }
    if (G->nextCnt[vertex] == 1)
    {
        hitButtomFlag = true;
    }
}

int main()
{
    // TODO: Implement your algorithm here
    int r;
    FILE* ptr = 0;
    if (FileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P3\\3.in", "r");
        r = fscanf(ptr, "%d %d", &shopNumberInitial, &daysM);
    }
    else
        r = scanf("%d %d", &shopNumberInitial, &daysM);
    // we ignore the space at index 0
    shopNumberHistory = malloc(sizeof(Q) * (daysM + 1));
    dsHistory = malloc(sizeof(DisjointSet*) * (daysM + 1));
    for (int i = 0; i < daysM + 1; i++)
    {
        shopNumberHistory[i].cnt = -1;
        dsHistory[i] = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
    }

    G = malloc(sizeof(Graph));
    G->nodeNumber = daysM + 1;
    G->adjLists = malloc((daysM + 1) * sizeof(OPDay*));
    G->visited = malloc((daysM + 1) * sizeof(bool));
    G->nextCnt = malloc((daysM + 1) * sizeof(int));
    for (int i = 0; i < (daysM + 1); i++)
    {
        G->adjLists[i] = NULL;
        G->visited[i] = 0;
        G->nextCnt[i] = 0;
    }

    T = malloc(sizeof(OPDay*) * (daysM + 1));

    //set = malloc(sizeof(bool) * (shopNumberInitial + 1));
    //for (int i = 1; i < shopNumberInitial + 1; i++)
    //    set[i] = 0;
    shopNumber = shopNumberInitial;
    //shopNumberHistory[0] = shopNumber;
    ds = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
    for (int i = 1; i < shopNumberInitial + 1; i++)
        makeset(i);
    isBoom = malloc(sizeof(bool) * (shopNumberInitial + 1));

    OPDay* prevn = malloc(sizeof(OPDay));
    prevn->day = 0;
    T[0] = prevn;
    for (int i = 1; i < daysM + 1; i++)
    {
        OPDay* n = malloc(sizeof(OPDay));
        n->day = i;
        if (FileFlag)
            r = fscanf(ptr, "%s", &n->operate.op);
        else
            r = scanf("%s", &n->operate.op);
        // store op in operationDays
        switch (n->operate.op[0])
        {
        case('q'):
            // do nothing
            connect(prevn, n);
            break;
        case('m'):
            if (FileFlag)
                r = fscanf(ptr, "%d %d", &n->operate.v1, &n->operate.v2);
            else
                r = scanf("%d %d", &n->operate.v1, &n->operate.v2);
            connect(prevn, n);
            break;
        case('b'):
            if (FileFlag)
                r = fscanf(ptr, "%d", &n->operate.v1);
            else
                r = scanf("%d", &n->operate.v1);
            // switch to different branch
            // need multiple next
            // TODO:
            connect(T[n->operate.v1], n);
            isBoom[T[n->operate.v1]->day] = true;
            break;
        default:
            break;
        }
        prevn = n;
        T[i] = n;
    }
    // at the start of every list, add itself
    for (int k = 0; k < (daysM + 1); k++)
    {
        OPDay* nself = malloc(sizeof(OPDay));
        nself->day = k;
        nself->next = G->adjLists[k];
        G->adjLists[k] = nself;
    }
    // we add one to be lazy in checking whether boom or not
    G->nextCnt[0]++;
    if (DebugFlag)
        printCity();
    DFS(0);
    for (int i = 0; i < daysM + 1; i++)
        if (shopNumberHistory[i].isquery == true)
            printf("%d\n", shopNumberHistory[i].cnt);
    if (FileFlag)
        fclose(ptr);
    return 0;
}
#pragma   warning(pop)  