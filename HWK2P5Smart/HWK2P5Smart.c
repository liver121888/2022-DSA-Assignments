
#define _CRT_SECURE_NO_DEPRECATE
#include "price.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>



typedef struct poolNode
{
    unsigned long long value;
    unsigned long long stockID; // unsigned int should be OK to save memory
    unsigned long long seqID;   //sequence index;  unsigned int should be OK to save memory
} PoolNode;


// Instead of using an object array whose orders are fixed, use pointer array for easier heapify operation
// During the operation, only swap the pointers is required
PoolNode** heapArray;

int numOfStocks, numQuery, increasePeriod, activeNumber;
unsigned long long kSweet, extra;
unsigned long long* stockIDs, * activeIDs;


char answer[80];
FILE* filePtr;


// Construct the first heap array of PollNode according to their values.
void HeapArrayFirstSort()
{

}




unsigned long long SequentialPoolFiltering(unsigned long long kSweet)
{
    PoolNode* root;
    unsigned long long k = 1;

    while (k != kSweet)
    {
        // The smallest node upgrades to next value
        heapArray[0]->seqID += increasePeriod; // day id jump to next increased day
        unsigned long long v = price(heapArray[0]->stockID, heapArray[0]->seqID); // get upgraded value
        heapArray[0]->value = v; // update value
        root = heapArray[0];
        int done = 0;
        // The root value is upgraded, re-heapify the heap array (b-tree) to have root updated with the smallest value
        for (int i = 1; i < increasePeriod * activeNumber; i++)
        {
            if (heapArray[i]->value < v)
            {
                heapArray[i - 1] = heapArray[i];
            }
            else
            {
                heapArray[i - 1] = root;
                done = 1;
                break;
            }
        }
        if (!done) heapArray[increasePeriod * activeNumber - 1] = root;
        k++;

    }

    unsigned long long sid = heapArray[0]->stockID;
    unsigned long long qid = heapArray[0]->seqID;
    unsigned long long vvv = heapArray[0]->value;
    printf("(%llu,%llu)=%llu \n", sid, qid, vvv);

    return heapArray[0]->value;
}

void PrintHeapArray()
{
    for (int i = 0; i < activeNumber * increasePeriod; i++)
        printf("(%llu,%llu)=%llu ", heapArray[i]->stockID, heapArray[i]->seqID, heapArray[i]->value);
    printf("\n");
}

void main()
{
    printf("Size of int = %d\n", sizeof(int));
    printf("Size of unsigned int = %d\n", sizeof(unsigned int));
    printf("Size of long = %d\n", sizeof(long));
    printf("Size of long int = %d\n", sizeof(long int));
    printf("Size of long long = %d\n\n\n", sizeof(long long));

    printf("  ******************* SMART HEAP SORTING & INSERT METHOD ******************");

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
        filePtr = fopen(answer, "r");
        printf("\nBenchmark file: %s\n", answer);

        fscanf(filePtr, "%d %d %d", &numOfStocks, &numQuery, &increasePeriod);

        // Store stock ids in arrays
        stockIDs = malloc((numOfStocks) * sizeof(unsigned long long));
        activeIDs = malloc((numOfStocks + 1) * sizeof(unsigned long long));

        // Allocate heap array that stores the pointers of nodes
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
                    // Allocate memory for each node and complete the
                    // first non-sorted heap array 
                    heapArray[c] = malloc(sizeof(PoolNode));
                    heapArray[c]->stockID = activeIDs[s];
                    heapArray[c]->seqID = p + 1; // 1 + p * increasePeriod;
                    heapArray[c]->value = price(heapArray[c]->stockID, heapArray[c]->seqID);
                    c++;
                }


            // Sort the firstly constructed heap array to get the first round of values.
            HeapArrayFirstSort();


            unsigned long long answer = SequentialPoolFiltering(kSweet);

            printf("Query %d  s=%llu, k=%llu  => answer = %llu \n", j, extra, kSweet, answer);

        }

        printf("Done! Check answer ...\n");
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