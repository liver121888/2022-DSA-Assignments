#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)

typedef struct disjointSet {
    int leaderHash;
    int size;
} DisjointSet;

int fileFlag = 1;
int debugFlag = 0;

DisjointSet* ds;
int daysM;
int shopNumberInitial;
int shopNumber;
int** operateNumber;
char* operate;

void makeset(int i) 
{
    ds[i].leaderHash = i;
    ds[i].size = 1;
}

int k;
int find_set(int i) 
{
    //if (ds[i].leaderHash != i)
    //{
    //    k = find_set(ds[i].leaderHash);
    //    return k;
    //}
    //return i;
    if (ds[i].leaderHash != i)
        i = find_set(ds[i].leaderHash);
    return i;
}

void group(int ra, int rb, int day) 
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
    }
    shopNumber--;
}

int main() 
{
    int r;
    FILE* ptr = 0;
    if (fileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P3\\0.in", "r");
        r = fscanf(ptr, "%d %d", &shopNumberInitial, &daysM);
    }
    else
        r = scanf("%d %d", &shopNumberInitial, &daysM);

    operateNumber = malloc(sizeof(int*) * (daysM + 1));
    operate = malloc(sizeof(char) * (daysM + 1));
    for (int i = 0; i < daysM + 1; i++)
        operateNumber[i] = malloc(sizeof(int) * 2);


    ds = malloc(sizeof(DisjointSet) * (shopNumberInitial + 1));
    for (int i = 0; i < shopNumberInitial + 1; i++)
        makeset(i);

    shopNumber = shopNumberInitial;

    char optmp[10];
    int v1, v2;
    for (int i = 1; i < daysM + 1; i++)
    {
        if (fileFlag)
            r = fscanf(ptr, "%s", optmp);
        else
            r = scanf("%s", optmp);
        operate[i] = optmp[0];
        switch (optmp[0])
        {
        case('q'):
            break;
        case('m'):
            if (fileFlag)
            {
                r = fscanf(ptr, "%d %d", &v1, &v2);
                operateNumber[i][0] = v1;
                operateNumber[i][1] = v2;
            }
            else
            {
                r = scanf("%d %d", &v1, &v2);
                operateNumber[i][0] = v1;
                operateNumber[i][1] = v2;
            }
            break;
        case('b'):
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < daysM + 1; i++)
    {
        switch (operate[i])
        {
        case('q'):
            // save query, later print
            operateNumber[i][0] = shopNumber;
            break;
        case('m'):
            //merge(T[i]->operate.v1, T[i]->operate.v2, i);
            group(operateNumber[i][0], operateNumber[i][1], i);
            break;
        case('b'):
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < daysM + 1; i++)
    {
        if (operate[i] == 'q')
            printf("%d\n", operateNumber[i][0]);
    }

    if (fileFlag)
    {
        fclose(ptr);
    }
    return 0;
}
#pragma warning(pop)
