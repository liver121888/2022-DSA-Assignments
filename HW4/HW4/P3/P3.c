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

typedef struct disjointSet 
{
    // TODO: Determine fields to use by your method
    int leaderHash;
    int recordedTag;
    int size;
} DisjointSet;


typedef struct shopPair
{
    // TODO: Determine fields to use by your method
    int s1;
    int s2;
} ShopPair;

DisjointSet *ds;
DisjointSet **dsHistory;

ShopPair* opShopsOrDays;
// init set
bool *set;
int FileFlag = 1;
int DebugFlag = 0;
int shopNumberInitial;
int shopNumber;
int* shopNumberHistory;
int daysM;
int dayBoom;
char* operations;

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

inline void static init(int i) 
{
    if (!set[i]) {
        makeset(i);
        set[i] = 1;
    }
}

int find_set(int day, int i) 
{
    int tmp;
    // TODO: Implement your find algorithm here
    if (ds[i].leaderHash != i)
    {
        tmp = ds[i].leaderHash;
        ds[i].leaderHash = find_set(day, ds[i].leaderHash);
        dsHistory[day][i].leaderHash = tmp;
    }
    return ds[i].leaderHash;
}

int find_set_get(int i)
{
    // TODO: Implement your find algorithm here
    if (ds[i].leaderHash != i)
        find_set_get(ds[i].leaderHash);
    return ds[i].leaderHash;
}

void merge(int day, int ra, int rb) 
{

    int a = find_set(day, ra), b = find_set(day, rb);
    // TODO: Implement your union algorithm here
    if (a == b)
        return;
    else
    {
        dsHistory[day][a].size = ds[a].size;
        dsHistory[day][a].leaderHash = ds[a].leaderHash;
        dsHistory[day][b].leaderHash = ds[b].leaderHash;
        dsHistory[day][b].size = ds[b].size;

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

    }
    shopNumber--;
}

void reverse(int day, bool isdemerge)
{
    for (int i = 1; i < shopNumberInitial + 1; i++)
    {
        if (dsHistory[day][i].size > 0)
        {
            ds[i].size = dsHistory[day][i].size;
            ds[i].leaderHash = dsHistory[day][i].leaderHash;
        }
    }
    if (isdemerge)  
        shopNumber++;
    else
    {
        shopNumber = shopNumberHistory[day];
        shopNumber -= opShopsOrDays[day].s2;
    }
}

void saveState(int theDayBoomHappened, int rollbackDay)
{
    for (int i = 1; i < shopNumberInitial + 1; i++)
    {
        if (dsHistory[rollbackDay][i].size > 0)
            if (dsHistory[theDayBoomHappened][i].recordedTag == 0)
            {
                dsHistory[theDayBoomHappened][i].leaderHash = ds[i].leaderHash;
                dsHistory[theDayBoomHappened][i].size = ds[i].size;
                dsHistory[theDayBoomHappened][i].recordedTag = 1;
            }
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

void printHistory(int d)
{
    printf("History: \n");
    for (int j = 1; j < d + 1; j++)
    {
        printf("*****\n");
        printf("H_Day: %d\n", j);
        for (int i = 1; i < shopNumberInitial + 1; i++)
        {
            if (dsHistory[j][i].size > 0)
                printf("S_%d = lead: %d /// size: %d, ", i, dsHistory[j][i].leaderHash, dsHistory[j][i].size);
            else
                printf("S_%d = lead: NA /// size: NA, ", i);
        }
        printf("\n");
    }
    printf("*****\n");
}

void printShopNumberHistory()
{
    printf("ShopNumberHistory: \n");
    for (int j = 0; j < daysM + 1; j++)
        printf("D%d: %d, ", j, shopNumberHistory[j]);
    printf("\n++++++++\n");
}

int main() 
{
    // TODO: Implement your algorithm here
    int r;
    int firstShop, secondShop;
    char op[10];
    FILE* ptr = 0;
    if (FileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P3\\3.in", "r");
        r = fscanf(ptr, "%d %d", &shopNumberInitial, &daysM);
    }
    else
        r = scanf("%d %d", &shopNumberInitial, &daysM);
    // we ignore the space at index 0
    ds = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
    dsHistory = malloc(sizeof(DisjointSet*) * (daysM + 1));
    shopNumberHistory = malloc(sizeof(int) * (daysM + 1));
    for (int i = 0; i < daysM + 1; i++)
    {
        shopNumberHistory[i] = -1;
        dsHistory[i] = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
        for (int j = 1; j < shopNumberInitial + 1; j++)
        {
            dsHistory[i][j].recordedTag = 0;
            dsHistory[i][j].size = -1;
        }
    }

    set = malloc(sizeof(bool) * (shopNumberInitial + 1));
    operations = malloc(sizeof(char) * (daysM + 1));
    opShopsOrDays = malloc(sizeof(ShopPair) * (daysM + 1));
    for (int i = 1; i < shopNumberInitial + 1; i++)
        set[i] = 0;
    shopNumber = shopNumberInitial;
    shopNumberHistory[0] = shopNumber;
    for (int i = 1; i < shopNumberInitial + 1; i++)
        init(i);

    if (DebugFlag)
        printShop(0);
    for (int i = 1; i < daysM + 1; i++)
    {
        if (FileFlag)
            r = fscanf(ptr, "%s", &op);
        else
            r = scanf("%s", &op);
        // store op in operations
        operations[i] = op[0];
        switch (op[0])
        {
        case('q'):
            printf("%d\n", shopNumber);
            // do nothing on opShops
            break;
        case('m'):
            if (FileFlag)
                r = fscanf(ptr, "%d %d", &firstShop, &secondShop);
            else
                r = scanf("%d %d", &firstShop, &secondShop);
            merge(i, firstShop, secondShop);
            opShopsOrDays[i].s1 = firstShop;
            opShopsOrDays[i].s2 = secondShop;
            break;
        case('b'):
            if (DebugFlag)
                printHistory(i);
            if (FileFlag)
                r = fscanf(ptr, "%d", &dayBoom);
            else
                r = scanf("%d", &dayBoom);
            int tmp = shopNumber;
            opShopsOrDays[i].s1 = dayBoom;
            for (int j = i - 1; j >= dayBoom + 1; j--)
            {
                switch (operations[j])
                {
                case('m'):
                    saveState(i, j);
                    reverse(j, 1);
                    break;
                case('b'):
                    saveState(i, j);
                    reverse(j, 0);
                    break;
                case('q'):
                default:
                    break;
                }
            }
            int delta = shopNumber - tmp;
            opShopsOrDays[i].s2 = delta;
            break;
        default:
            break;
        }
        shopNumberHistory[i] = shopNumber;
        if (DebugFlag)
            printShop(i);
    }
    if (DebugFlag)
        printShopNumberHistory();

    if (FileFlag)
    {
        fclose(ptr);
    }
    return 0;
}
#pragma   warning(pop)  