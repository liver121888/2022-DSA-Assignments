
#define _CRT_SECURE_NO_DEPRECATE
#include "price.h"

#include<stdio.h>
#include<stdlib.h>

int numOfStocks, numQuery, increasePeriod, kSweet, extra;
unsigned int *stocks;
char answer[80];
FILE* filePtr;

void main()
{

    char fileName1[] = "..\\HWK2 Samples\\p5sample1.txt";
    char fileName2[] = "..\\HWK2 Samples\\p5sample2.txt";
    char fileName3[] = "..\\HWK2 Samples\\p5sample3.txt";
    char fileName4[] = "..\\HWK2 Samples\\p5sample4.txt";

    char answer[80];

    // filePtr = fopen(fileName1, "r");
    filePtr = fopen(fileName2, "r");
    // filePtr = fopen(fileName4, "r");
    // filePtr = fopen(fileName3, "r");

    fscanf(filePtr, "%d %d %d", &numOfStocks, &numQuery, &increasePeriod);
    stocks = malloc(numOfStocks * sizeof(unsigned int));
    for (int i = 0; i < numOfStocks; i++)
    {
        fscanf(filePtr, "%d", &stocks[i]);
    }

    for (int j = 0; j < numQuery; j++)
    {
        fscanf(filePtr, "%d %d", &extra, &kSweet);

        unsigned long long d = pow(2, sizeof(long long)) - 1;

        if( extra != 0 ) d = price(extra, kSweet);
 
        for (int i = 0; i < numOfStocks; i++)
        {
            unsigned long long v = price(stocks[i], kSweet);
            if (v < d) d = v;
        }
        printf("%u\n", d);
    }

    fscanf(filePtr, "%s", answer);
    printf( "%s\n", answer);
    unsigned long long g;
    for (int j = 0; j < numQuery; j++)
    {
        fscanf(filePtr, "%u", &g );
 
        printf("%u\n", g );    
    }
    fclose(filePtr);

}