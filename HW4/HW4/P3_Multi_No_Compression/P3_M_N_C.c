#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)

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

typedef struct opday
{
    Operate operate;
    int day;
} OPDay;

typedef struct node
{
    int day;
    struct node* next;
} Node;

typedef struct graph {
    bool* visited;
    Node** adjLists;
}Graph;

int fileFlag = 1;
int debugFlag = 0;
int shopNumberInitial;
int shopNumber;
int* shopNumberHistory;
int daysM;
DisjointSet* ds;
int* dsHistory;
OPDay** T;
Graph* G;

void makeset(int i)
{
    ds[i].leaderHash = i;
    ds[i].size = 1;
}

int find_set(int i)
{
    if (ds[i].leaderHash != i)
        i = find_set(ds[i].leaderHash);
    return i;
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
            dsHistory[day] = ds[a].leaderHash;

            ds[a].leaderHash = ds[b].leaderHash;
            ds[b].size += ds[a].size;
        }
        else
        {
            // a > b, merge b into a
            dsHistory[day] = ds[b].leaderHash;

            ds[b].leaderHash = ds[a].leaderHash;
            ds[a].size += ds[b].size;
        }
        shopNumber--;
    }
}

void demerge(int ra, int rb, int day)
{
    int a = find_set(ra), b = find_set(rb);
    if (a!=b)
        return;
    else
    {
        int tmp = find_set(dsHistory[day]);
        ds[dsHistory[day]].leaderHash = dsHistory[day];
        ds[tmp].size -= ds[dsHistory[day]].size;
        shopNumber++;
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
    int i = 0;
    //Node* tmp = G->adjLists[i];
    //while (tmp)
    //{
    //    printf("Day %d: \n", i);
    //    printf("%d, ", tmp->day);
    //    tmp = tmp->next;
    //    i++;
    //}
    printf("\n");
    printf("##########\n");
}


//// ref: https://www.programiz.com/dsa/graph-dfs
//// DFS algo
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
            while (tmpindex > 0 && !(T[tmpindex]->operate.op[0] == 'b'))
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



void DFSMODI(int vertex) {
    if (G->visited[vertex] == true)
        return;
    if (debugFlag)
        printf("Visit %d \n", vertex);
    G->visited[vertex] = true;
    Node* temp = G->adjLists[vertex];
    while (temp != NULL)
    {
        if (G->adjLists[temp->day])
            DFSMODI(temp->day);
        int tmpindex = vertex + 1;
        while (G->visited[tmpindex] != true && tmpindex <= daysM && T[tmpindex]->operate.op[0] != 'b')
        {
            switch (T[tmpindex]->operate.op[0])
            {
            case('q'):
                shopNumberHistory[tmpindex] = shopNumber;
                break;
            case('m'):
                merge(T[tmpindex]->operate.v1, T[tmpindex]->operate.v2, tmpindex);
                break;
            case('b'):
                break;
            default:
                break;
            }
            G->visited[tmpindex] = true;
            if (debugFlag)
            {
                printf("Visit %d \n", tmpindex);
                printShop(tmpindex);
            }
            //printShopNumberHistory();
            if (G->adjLists[tmpindex])
                DFSMODI(G->adjLists[tmpindex]->day);
            tmpindex++;
        }
        tmpindex--;
        //while (tmpindex>=1 && !(T[tmpindex]->operate.op[0] == 'b'))
        while (tmpindex > 0 && !(T[tmpindex]->operate.op[0] == 'b'))
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
        temp = temp->next;
    }
}
//
//void printT()
//{
//    for (int i = 0; i < daysM + 1; i++)
//    {
//        printf("Day: %d, operate: %c, v1: %d, v2: %d\n", T[i]->day, T[i]->operate.op[0], T[i]->operate.v1, T[i]->operate.v2);
//    }
//}



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
    dsHistory = malloc(sizeof(int) * (daysM + 1));
    for (int i = 0; i < daysM + 1; i++)
    {
        shopNumberHistory[i] = -1;
    }

    G = malloc(sizeof(Graph));
    G->adjLists = malloc((daysM + 1) * sizeof(Node*));
    G->visited = malloc((daysM + 1) * sizeof(bool));
    for (int i = 0; i < (daysM + 1); i++)
    {
        G->adjLists[i] = NULL;
        G->visited[i] = false;
    }

    T = malloc(sizeof(OPDay*) * (daysM + 1));

    shopNumber = shopNumberInitial;
    ds = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
    for (int i = 0; i < shopNumberInitial + 1; i++)
        makeset(i);

    OPDay* n = malloc(sizeof(OPDay));
    n->day = 0;
    n->operate.op[0] = 'y';
    n->operate.op[1] = 0;
    T[0] = n;
    char optmp[10];
    //int v1, v2;
    for (int i = 1; i < daysM + 1; i++)
    {
        n = malloc(sizeof(OPDay));
        n->day = i;
        if (fileFlag)
            r = fscanf(ptr, "%s", optmp);
        else
            r = scanf("%s", optmp);
        n->operate.op[0] = optmp[0];
        n->operate.op[1] = 0;
        switch (optmp[0])
        {
        case('q'):
            break;
        case('m'):
            if (fileFlag)
            {
                r = fscanf(ptr, "%d %d", &(n->operate.v1), &(n->operate.v2));
                //r = fscanf(ptr, "%d %d", &v1, &v2);
                //operateNumber[i][0] = v1;
                //operateNumber[i][1] = v2;
            }
            else
            {
                r = scanf("%d %d", &(n->operate.v1), &(n->operate.v2));
                //r = scanf("%d %d", &v1, &v2);
                //operateNumber[i][0] = v1;
                //operateNumber[i][1] = v2;
            }                
            break;
        case('b'):
            if (fileFlag)
                r = fscanf(ptr, "%d", &(n->operate.v1));
            else
                r = scanf("%d", &(n->operate.v1));
            connect(n->operate.v1, n->day);
            break;
        default:
            break;
        }
        T[i] = n;
    }

    DFSMODI(0);
    for (int i = 1; i < daysM + 1; i++)
    {
        if (!(G->visited[i]))
        {
            switch (T[i]->operate.op[0])
            {
            case('q'):
                // save query, later print
                shopNumberHistory[i] = shopNumber;
                break;
            case('m'):
                merge(T[i]->operate.v1, T[i]->operate.v2, i);
                break;
            case('b'):
                break;
            default:
                break;
            }
        }
        if (debugFlag)
            printShop(i);
        DFSMODI(i);
    }
    //DFS(0);


    for (int i = 0; i < daysM + 1; i++)
    {
        if (T[i]->operate.op[0] == 'q')
            printf("%d\n", shopNumberHistory[i]);
    }

    if (fileFlag)
        r = fclose(ptr);
    return 0;
}
#pragma warning(pop)