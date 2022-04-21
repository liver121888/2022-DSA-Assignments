
#define _CRT_SECURE_NO_DEPRECATE
#include "price.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


typedef struct pollNode
{
    unsigned long long value;
    unsigned long long stockID; // unsigned int should be OK to save memory
    unsigned long long seqID;   //sequence index;  unsigned int should be OK to save memory
} PoolNode;


// Instead of using an object array whose orders are fixed, use pointer array for easier sorting (altering their orders)
PoolNode** heapArray;

int numOfStocks, numQuery, increasePeriod, activeNumber;
unsigned long long kSweet, extra;
unsigned long long* stockIDs,*activeIDs;



char answer[80];
FILE* filePtr;


void NodeQuickSort(int left, int right)
{
    if (left >= right) return ;
    unsigned long long key = heapArray[right]->value; // last element is the pivot
    // i: left upper bound
    int i = left - 1;
    PoolNode* temp;
    for (int j = left; j <= right - 1; j++)
        if (heapArray[j]->value <= key)
        {
            // extend
            i++;
            // move element j to left part
            temp = heapArray[i];
            heapArray[i] = heapArray[j];
            heapArray[j] = temp;
        }
    // extend 
    i++;
    // insert the pivot between
    temp = heapArray[i];
    heapArray[i] = heapArray[right];
    heapArray[right] = temp;

    NodeQuickSort(left, i - 1);
    NodeQuickSort(i + 1, right);
}



unsigned long long SequentialPoolFiltering( unsigned long long kSweet)
{
    PoolNode* head;
    unsigned long long k = 1;

    while (k != kSweet)
    {
        heapArray[0]->seqID += increasePeriod;
        unsigned long long v = price(heapArray[0]->stockID, heapArray[0]->seqID);
        heapArray[0]->value = v;
        head = heapArray[0];
        int done = 0;
        for (int i = 1; i < increasePeriod * activeNumber; i++)
        {
            if (heapArray[i]->value < v)
            {
                heapArray[i - 1] = heapArray[i];
            }
            else
            {
                heapArray[i - 1] = head;
                done = 1;
                break;
            }
        }
        if (!done) heapArray[increasePeriod * activeNumber - 1] = head;
        k++;

    }
 
     printf("  My answer => (%llu,%llu) = %llu \n", heapArray[0]->stockID, heapArray[0]->seqID, heapArray[0]->value);

     return heapArray[0]->value;
}

void main()
{
    printf("  ******************* Naive ORDERED and SORTED Array & Serial INSERT METHOD ******************\n");

    char fileName1[] = "..\\HWK2 Samples\\p5sample1.txt";
    char fileName2[] = "..\\HWK2 Samples\\p5sample2.txt";
    char fileName3[] = "..\\HWK2 Samples\\p5sample3.txt";
    char fileName4[] = "..\\HWK2 Samples\\p5sample4.txt";

    char answer[80];

   // filePtr = fopen(fileName1, "r");
 //  filePtr = fopen(fileName2, "r");
 //     filePtr = fopen(fileName3, "r");
 
    for (int f = 1; f <= 4; f++)
    {
        sprintf(answer, "..\\HWK2 Samples\\p5sample%d.txt", f);

        clock_t startTime = clock();

        filePtr = fopen(answer, "r");
        printf("\nBenchmark file: %s\n", answer);

        fscanf(filePtr, "%d %d %d", &numOfStocks, &numQuery, &increasePeriod);
        stockIDs = malloc((numOfStocks) * sizeof(unsigned long long));
        activeIDs = malloc((numOfStocks + 1) * sizeof(unsigned long long));
        heapArray = malloc((numOfStocks + 1) * increasePeriod * sizeof(PoolNode*));

        unsigned long long id = 0;
        for (int i = 0; i < numOfStocks; i++)
        {
            fscanf(filePtr, "%llu", &id); // or %I64u
            stockIDs[i] = id;
            activeIDs[i] = stockIDs[i];
        }

        unsigned long long ee = 0, kk = 0;

        for (int j = 0; j < numQuery; j++)
        {
            fscanf(filePtr, "%llu %llu", &ee, &kk);
            kSweet = kk;
            extra = ee;
            if (extra == 0)
            {
                activeNumber = numOfStocks;
            }
            else
            {
                activeNumber = numOfStocks + 1;
                activeIDs[numOfStocks] = extra;
            }

            // Create Node array
            int c = 0;
            for (int s = 0; s < activeNumber; s++)
                for (int p = 0; p < increasePeriod; p++)
                {
                    heapArray[c] = malloc(sizeof(PoolNode));
                    heapArray[c]->stockID = activeIDs[s];
                    heapArray[c]->seqID = p + 1; // 1 + p * increasePeriod;
                    heapArray[c]->value = price(heapArray[c]->stockID, heapArray[c]->seqID);
                    c++;
                }

            //printf("k = 0 " );
            //for (int i = 0; i < activeNumber * increasePeriod; i++)
            //    printf("(%llu,%llu)=%llu ", sortedNodes[i]->stockID, sortedNodes[i]->seqID, sortedNodes[i]->value);
            //printf("\n");

            NodeQuickSort(0, activeNumber * increasePeriod - 1);

            //printf("k = 1 " );
            //for (int i = 0; i < activeNumber * increasePeriod; i++)
            //    printf("(%llu,%llu)=%llu ", sortedNodes[i]->stockID, sortedNodes[i]->seqID, sortedNodes[i]->value);
            //printf("\n");


            printf("Query %d  s=%llu, k=%llu  => ", j, extra, kSweet);
            unsigned long long answer = SequentialPoolFiltering(kSweet);


        }

        clock_t endTime = clock();
        double seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        printf("Done! Time Used: %f secs.  Check with the correct answers ...\n", seconds);
        fscanf(filePtr, "%s", answer);
        printf("%s\n", answer);

        for (int j = 0; j < numQuery; j++)
        {
            fscanf(filePtr, "%s", answer);
            printf("%s\n", answer);
        }
        fclose(filePtr);
    }

}