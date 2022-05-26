#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct disjointSet {
    // TODO: Determine fields to use by your method
    int leaderHash;
    int size;
} DisjointSet;

DisjointSet ds[1 << 24];
// init set
bool set[1 << 24] = { 0 };
int FileFlag = 1;
int DebugFlag = 1;

int c2i(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {
    int ret = 0;
    int mask = (1 << 24) - 1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char* s) {
    // TODO: Initialize a set with hash value
    int i = hash(s);
    DisjointSet* n = malloc(sizeof(DisjointSet));
    if (n != NULL)
    {
        n->leaderHash = i;
        n->size = 1;
        ds[i] = *n;
    }
}

inline void static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
}

int find_set(int i) {
    // TODO: Implement your find algorithm here
    if (ds[i].leaderHash != i)
        ds[i].leaderHash = find_set(ds[i].leaderHash);
    return ds[i].leaderHash /* something */;
}

void group(const char* ra, const char* rb) {

    init(ra);
    int i = hash(ra);
    init(rb);
    int i2 = hash(rb);

    int a = find_set(i), b = find_set(i2);
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
}

bool same_set(const char* a, const char* b) {
    // TODO: Implement your algorithm here
    init(a);
    int i = hash(a);
    init(b);
    int i2 = hash(b);
    int ah = find_set(i), bh = find_set(i2);
    if (ds[ah].leaderHash == ds[bh].leaderHash)
    {
        return 1;
    }
    else
        return 0;
}

int main() {
    // TODO: Implement your algorithm here
    int N;
    int r;
    char operation[10];
    char name1[20];
    char name2[20];
    FILE* ptr = 0;
    if (FileFlag)
    {
        ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW4\\HW4\\hw4_testdata\\P4", "r");
        r = fscanf(ptr, "%d", &N);
    }
    else
        r = scanf("%d", &N);

    for (size_t i = 0; i < N; i++)
    {
        if (FileFlag)
        {
            r = fscanf(ptr, "%s %s %s", &operation, &name1, &name2);
        }
        else
            r = scanf("%s %s %s", &operation, &name1, &name2);

        if (operation[0] == 'g')
        {
            // group
            init(name1);
            init(name2);
            // we will check whether they are same group
            group(name1, name2);
        }
        else
        {
            // test
            if (same_set(name1, name2))
                printf("Positive\n");
            else
                printf("Negative\n");
        }
        if (DebugFlag)
        {
        }
    }

    if (FileFlag)
    {
        fclose(ptr);
    }
    return 0;
}