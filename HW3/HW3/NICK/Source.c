#include <stdio.h>
#include <stdlib.h> // malloc / free
#include <memory.h> // memset

// upload = 1 => upload judge
int upload = 1;

// k : # of magic/ l : length of magic / flag : answer format
int k, l, flag;
char** input;
// 0x21(33) ~ 0x7E(126)
unsigned long long d = 94;
unsigned long long q = 196241958230952677;

// record key and idx
typedef struct data
{
    unsigned long long key;
    int idx;
} data;

data* RK, * RK_tmp;

// compare func for qsort
int cmpfunc(const void* a, const void* b)
{
    if (((data*)a)->key < ((data*)b)->key)
    {
        return -1;
    }
    else if (((data*)a)->key == ((data*)b)->key)
    {
        return 0;
    }
    else
        return 1;
    // return (((data *)a)->key - ((data *)b)->key);
}

int main()
{
    FILE* fp;
    if (upload)
        scanf("%d %d %d", &k, &l, &flag);
    else
    {
        fp = fopen("D:\\nick\\Homework\\Senior\\DSA\\Homework\\b07501117\\9\\test\\5.in", "r");
        fscanf(fp, "%d %d %d", &k, &l, &flag);
    }

    if (k == 1)
    {
        printf("No\n");
        return 0;
    }

    // input(k * l) : to store all input char
    input = malloc(sizeof(char*) * k);
    RK = malloc(sizeof(data) * k);
    RK_tmp = malloc(sizeof(data) * k);

    for (int i = 0; i < k; i++)
    {
        input[i] = malloc(sizeof(char) * (l + 1));
        if (upload)
            scanf("%s", input[i]);
        else
            fscanf(fp, "%s", input[i]);
    }

    // calculate Rabin-Karp values for every string
    unsigned long long magic;
    for (int i = 0; i < k; i++)
    {
        magic = 0;
        for (int j = 0; j < l; j++)
            magic = (d * magic % q + (unsigned long long)(input[i][j] - 33)) % q;
        RK[i].idx = i;
        RK[i].key = magic;
    }

    unsigned long long count = 1;
    unsigned long long total = 0;

    unsigned long long x = 1;
    for (int j = l - 1; j >= 0; j--)
    {
        // calculate Rabin-Karp values for missing char
        for (int i = 0; i < k; i++)
        {
            RK_tmp[i].idx = RK[i].idx;
            RK_tmp[i].key = ((RK[i].key + q) - (unsigned long long)(input[i][j] - 33) * x % q) % q;
        }
        x = x * 94 % q;

        qsort(RK_tmp, k, sizeof(data), cmpfunc);

        // dealing with output
        if (!flag)
        {
            // run through the sorted array and check for similar pairs
            for (int i = 1; i < k; i++)
            {
                if (RK_tmp[i - 1].key == RK_tmp[i].key)
                {
                    printf("Yes\n");
                    printf("%d %d\n", RK_tmp[i - 1].idx, RK_tmp[i].idx);
                    return 0;
                }
            }
        }
        else
        {
            // flag = 1
            for (int i = 1; i < k; i++)
            {
                if (RK_tmp[i - 1].key == RK_tmp[i].key)
                    count++;
                else
                {
                    if (count > 1)
                    {
                        total += count * (count - 1) / 2;
                        count = 1;
                    }
                }
            }
            if (count > 1)
            {
                total += count * (count - 1) / 2;
                count = 1;
            }
        }
    }

    if (!flag)
        printf("No\n");
    else
    {
        qsort(RK, k, sizeof(data), cmpfunc);
        for (int i = 1; i < k; i++)
        {
            if (RK[i - 1].key == RK[i].key)
                count++;
            else
            {
                if (count > 1)
                {
                    total = total - count * (count - 1) / 2 * (unsigned long long)(l - 1);
                    count = 1;
                }
            }
        }
        if (count > 1)
            total = total - count * (count - 1) / 2 * (unsigned long long)(l - 1);
        if (total)
        {
            printf("Yes\n");
            printf("%llu\n", total);
        }
        else
            printf("No\n");
    }

    if (!upload)
        fclose(fp);

    return 0;
}