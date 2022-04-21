
#define _CRT_SECURE_NO_DEPRECATE
#include "price.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>




typedef struct poolNode
{
    unsigned long long value;
    unsigned long long stockID; // unsigned int should be OK to save memory
    unsigned long long seqID;   //sequence index;  unsigned int should be OK to save memory
} PoolNode;


// Instead of using an object array whose orders are fixed, use pointer array for easier heapify operation
// During the operation, only swap the pointers is required
PoolNode** heapArray;

int numOfStocks, numQuery, increasePeriod, activeNumber, arraySize;
unsigned long long kSweet, extra;
unsigned long long* stockIDs, * activeIDs;


char answer[80];
FILE* filePtr;


// Construct the first heap array of PollNode according to their values.
// It will be min heap, where the root is the smallest and all children are larger than their parents.
// Once completed the root is the smallest node; yet the tree is not sorted!
void HeapArrayFirstSort()
{
    PoolNode* temp;
    // Start from last parent backward to do min heapify operation
    for (int parent = arraySize / 2; parent >= 0; parent--)
    {
        int parentID = parent;
        int childID = parentID * 2 + 1;

        while (childID < arraySize) // child is traversed one by one
        { 
            // Select the child with the smaller value
            if (childID + 1 < arraySize && heapArray[childID+1]->value < heapArray[childID]->value)
                childID++; // second child is smaller than first child
            if (heapArray[parentID]->value <= heapArray[childID]->value)  
                break; // Done! Since the parent is smaller or equal to the smaller child
            else 
            { 
                // Let the smaller child turn parent, and downgrade the parent to the child
                // Swap child and parent
                temp = heapArray[parentID];
                heapArray[parentID] = heapArray[childID];
                heapArray[childID] = temp;
                // Since parent and child are swapped, we need to traverse down further
                parentID = childID;
                childID = parentID * 2 + 1;
            }
        }
    }
}


// For debug using. Check whether the heap array is a valid min heap
int CheckHeapArrayValidity( int printFlag )
{
    int cid;
    for (int pid = 0; pid <= arraySize / 2; pid++)
    {
        cid = pid * 2 + 1;
        if ( cid < arraySize &&  heapArray[pid]->value > heapArray[cid]->value)
        {
            if(printFlag)  printf("\nERROR! parent H[%d] = %llu  > C[%d] = %llu\n\n", pid, heapArray[pid]->value, cid, heapArray[cid]->value);
            return 0;
        }
        cid++;
        if (cid < arraySize && heapArray[pid]->value > heapArray[cid]->value)
        {
            if (printFlag)  printf("\nERROR! parent H[%d] = %llu  > C[%d] = %llu\n\n", pid, heapArray[pid]->value, cid, heapArray[cid]->value);
            return 0;
        }
    }
    if (printFlag)   printf("\nHeap array is OK!!\n");
    return 1;
}


unsigned long long SequentialPoolFiltering(unsigned long long kSweet)
{
    PoolNode* temp;
    unsigned long long k = 1;

    while (k != kSweet)
    {
        // The smallest node upgrades to next value
        heapArray[0]->seqID += increasePeriod; // day id jump to next increased day
        unsigned long long v = price(heapArray[0]->stockID, heapArray[0]->seqID); // get upgraded value
        heapArray[0]->value = v; // update value

        int parentID = 0; // The root is upgraded
        int childID = 1;
        // Top-down min heapify??
        while (childID < arraySize) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < arraySize && heapArray[childID + 1]->value < heapArray[childID]->value )
                childID++; // second child is smaller than first child
            if (heapArray[parentID]->value <= heapArray[childID]->value )
            {
                break; // Done! Since the parent is smaller or equal to the smaller child
            }
            else
            {
                // Let the smaller child turn parent, and downgrade the parent to the child
                // Swap child and parent
                temp = heapArray[parentID];
                heapArray[parentID] = heapArray[childID];
                heapArray[childID] = temp;
                // Since parent and child are swapped, we need to traverse down further
                parentID = childID;
                childID = parentID * 2 + 1;
            }
        }
        k++;
        // Debug
        //int result = CheckHeapArrayValidity(0);
        //if(result == 0 )
        //    printf(" *** Validity Failed at k = %d  \n", k);

    }


    printf("  My answer => (%llu,%llu) = %llu \n", heapArray[0]->stockID, heapArray[0]->seqID, heapArray[0]->value);

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

    printf("  ******************* SMART HEAP SORTING & Heap INSERT METHOD ******************\n");

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
            arraySize = activeNumber * increasePeriod;

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
// Debug            CheckHeapArrayValidity( 1 );

            printf("Query %d  s=%llu, k=%llu  => ", j, extra, kSweet);
            unsigned long long answer = SequentialPoolFiltering(kSweet);

 // Debug           printf("Query %d  s=%llu, k=%llu  => answer = %llu \n", j, extra, kSweet, answer);

        }


        clock_t endTime = clock();
        double seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        printf("Done! Time Used: %f secs.  Check with the correct answers ...\n", seconds );
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