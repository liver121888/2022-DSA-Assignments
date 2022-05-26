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
    // TODO: Determine fields to use by your method
    char op[10];
    int v1;
    int v2;
} Operate;

typedef struct disjointSet
{
    // TODO: Determine fields to use by your method
    int leaderHash;
    int size;
} DisjointSet;

typedef struct opday
{
    // TODO: Determine fields to use by your method
    Operate operate;
    int day;
    DisjointSet* ds;
    struct opday* child;
} OPDay;

typedef struct graph {
    int nodeNumber;
    bool* visited;
    OPDay** adjlists;
}Graph;


// init set
bool* set;
int FileFlag = 1;
int DebugFlag = 1;
int shopNumberInitial;
int shopNumber;
int* shopNumberHistory;
int daysM;
int dayBoom; 
OPDay** T;
Graph* G;

//void makeset(int i)
//{
//    // TODO: Initialize a set with hash value
//    DisjointSet* n = malloc(sizeof(DisjointSet));
//    if (n != NULL)
//    {
//        n->leaderHash = i;
//        n->size = 1;
//        ds[i] = *n;
//    }
//}
//
//inline void static init(int i)
//{
//    if (!set[i]) 
//    {
//        makeset(i);
//        set[i] = 1;
//    }
//}
//
//int find_set(int day, int i)
//{
//    // TODO: Implement your find algorithm here
//    if (ds[i].leaderHash != i)
//    {
//        ds[i].leaderHash = find_set(day, ds[i].leaderHash);
//    }
//    return ds[i].leaderHash;
//}
//
//void merge(int day, int ra, int rb)
//{
//
//    int a = find_set(day, ra), b = find_set(day, rb);
//    // TODO: Implement your union algorithm here
//    if (a == b)
//        return;
//    else
//    {
//
//        if (ds[a].size <= ds[b].size)
//        {
//            // a <= b, merge a into b
//            ds[a].leaderHash = ds[b].leaderHash;
//            ds[b].size += ds[a].size;
//        }
//        else
//        {
//            // a > b, merge b into a
//            ds[b].leaderHash = ds[a].leaderHash;
//            ds[a].size += ds[b].size;
//        }
//
//    }
//    shopNumber--;
//}

//void printShop(int d)
//{
//    printf("-------------------------\n");
//    printf("Day: %d\n", d);
//    for (int i = 1; i < shopNumberInitial + 1; i++)
//        printf("S_%d = lead: %d /// size: %d, ", i, ds[i].leaderHash, ds[i].size);
//    printf("\n-------------------------\n");
//}

void printShopNumberHistory()
{
    printf("ShopNumberHistory: \n");
    for (int j = 0; j < daysM + 1; j++)
        printf("D%d: %d, ", j, shopNumberHistory[j]);
    printf("\n++++++++\n");
}

void connect(OPDay* day1, OPDay* day2)
{
    // for day1, day2 we need copy by value
    OPDay* no = malloc(sizeof(OPDay));
    no->child = day1->child;
    no->day = day1->day;
    no->operate = day1->operate;

    OPDay* nd = malloc(sizeof(OPDay));
    nd->child = day2->child;
    nd->day = day2->day;
    nd->operate = day2->operate;
    
    no->child = G->adjlists[nd->day];
    G->adjlists[nd->day] = no;

    nd->child = G->adjlists[no->day];
    G->adjlists[no->day] = nd;
}

void printCity()
{
    printf("##########\n");
    for (int i = 0; i < G->nodeNumber; i++) {
        OPDay* tmp = G->adjlists[i];
        printf("Day %d: \n", i);
        while (tmp)
        {
            printf("%d, ", tmp->day);
            tmp = tmp->child;
        }
        printf("\n");
    }
    printf("##########\n");
}

int main()
{
    // TODO: Implement your algorithm here
    int r;
    FILE* ptr = 0;
    if (FileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P3\\1.in", "r");
        r = fscanf(ptr, "%d %d", &shopNumberInitial, &daysM);
    }
    else
        r = scanf("%d %d", &shopNumberInitial, &daysM);
    // we ignore the space at index 0
    //operationDays = malloc(sizeof(OPDay) * (daysM + 1));
    shopNumberHistory = malloc(sizeof(int) * (daysM + 1));
    for (int i = 0; i < daysM + 1; i++)
        shopNumberHistory[i] = -1;

    G = malloc(sizeof(Graph));
    G->nodeNumber = daysM + 1;
    G->adjlists = malloc((daysM + 1) * sizeof(OPDay*));
    G->visited = malloc((daysM + 1) * sizeof(bool));
    for (int i = 0; i < (daysM + 1); i++)
    {
        G->adjlists[i] = NULL;
        G->visited[i] = 0;
    }


    T = malloc(sizeof(OPDay*) * (daysM + 1));

    set = malloc(sizeof(bool) * (shopNumberInitial + 1));
    for (int i = 1; i < shopNumberInitial + 1; i++)
        set[i] = 0;
    shopNumber = shopNumberInitial;
    shopNumberHistory[0] = shopNumber;
    //for (int i = 1; i < shopNumberInitial + 1; i++)
    //    init(i);


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
            // need multiple child
            // TODO:
            connect(T[n->operate.v1], n);
            break;
        default:
            break;
        }
        prevn = n;
        T[i] = n;
    }
    if (DebugFlag)
        printCity();







    // here should be a tmp to store the initial state
    //for (int i = 1; i < daysM + 1; i++)
    //{
    //    switch (previousDay[i].op[0])
    //    {
    //    case('q'):
    //        break;
    //    case('m'):
    //        break;
    //    case('b'):
    //        break;
    //    default:
    //        break;
    //    }
    //}

    if (FileFlag)
        fclose(ptr);
    return 0;
}
#pragma   warning(pop)  