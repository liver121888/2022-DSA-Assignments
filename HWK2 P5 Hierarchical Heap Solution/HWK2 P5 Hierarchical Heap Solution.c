
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

unsigned long long price(unsigned long long s, unsigned long long t) {
    unsigned long long p, x = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ULL;
    p = (s - 1) / 1000000ULL + 1ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    unsigned long long la = 0, lb = 0, ra = 0xffffffffffefULL, rb = 0xffffffffffefULL,
        ma, mb, na, nb, y = x, ta = (t > (p >> 1)) ? (t - (p >> 1)) : 0, tb = t + (p >> 1), tr = t / p;

    for (int i = 28; i >= 1; i -= 3) {
        ma = la + (((ra - la) * (x >> 48)) >> 16);
        mb = lb + (((rb - lb) * (y >> 48)) >> 16);
        if ((1ULL << (i + 2)) & ta) la = ma;
        else ra = ma;
        if ((1ULL << (i + 2)) & tb) lb = mb;
        else rb = mb;
        ma = la + (((ra - la) * ((x & 0xffff00000000ULL) >> 32)) >> 16);
        mb = lb + (((rb - lb) * ((y & 0xffff00000000ULL) >> 32)) >> 16);
        if ((1ULL << (i + 1)) & ta) la = ma;
        else ra = ma;
        if ((1ULL << (i + 1)) & tb) lb = mb;
        else rb = mb;
        ma = la + (((ra - la) * ((x & 0xffff0000ULL) >> 16)) >> 16);
        mb = lb + (((rb - lb) * ((y & 0xffff0000ULL) >> 16)) >> 16);
        x = (x >> 21) ^ (ta & (1ULL << i)) ^ (x * 0xc0ffee123456789ULL);
        y = (y >> 21) ^ (tb & (1ULL << i)) ^ (y * 0xc0ffee123456789ULL);
        if ((1ULL << i) & ta) la = ma;
        else ra = ma;
        if ((1ULL << i) & tb) lb = mb;
        else rb = mb;
    }

    ma = la + (((ra - la) * (x >> 48)) >> 16);
    mb = lb + (((rb - lb) * (y >> 48)) >> 16);
    if (1ULL & ta) la = ma;
    else ra = ma;
    if (1ULL & tb) lb = mb;
    else rb = mb;
    y = (y ^ (y >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (y << 13);

    return 	la + tr + (((lb - la) * (x >> 48)) >> 16);
}



typedef struct segmentNode
{
    unsigned long long value;
    unsigned long long seqID;   //sequence index;  unsigned int should be OK to save memory
} SegmentNode;

typedef struct stockNode
{
    unsigned long long stockID; // unsigned int should be OK to save memory
    SegmentNode** segmentHeapArray;
} StockNode;

// Instead of using an object array whose orders are fixed, use pointer array for easier heapify operation
// During the operation, only swap the pointers is required
StockNode** stockHeapArray;

int numOfStocks, numQuery, increasePeriod, activeNumber, arraySize;
unsigned long long kSweet, extra;
unsigned long long* stockIDs, * activeIDs;


char answer[80];
FILE* filePtr;


// Construct the first heap array of StockNode according to their first min values.
// It will be constructed as a min-heap, where the root is the smallest and all children are larger than their parents.
// Once completed the root is the smallest node; yet the tree is not sorted (we don't need a sorted heap)!
void StockHeapArrayFirstSort()
{
    StockNode* temp;
    // Start from last parent backward to do min heapify operation
    for (int parent = activeNumber / 2; parent >= 0; parent--)
    {
        int parentID = parent;
        int childID = parentID * 2 + 1;

        while (childID < activeNumber) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < activeNumber && stockHeapArray[childID + 1]->segmentHeapArray[0]->value < stockHeapArray[childID]->segmentHeapArray[0]->value)
                childID++; // second child is smaller than first child
            if (stockHeapArray[parentID]->segmentHeapArray[0]->value <= stockHeapArray[childID]->segmentHeapArray[0]->value)
                break; // Done! Since the parent is smaller or equal to the smaller child
            else
            {
                // Let the smaller child turn parent, and downgrade the parent to the child
                // Swap child and parent
                temp = stockHeapArray[parentID];
                stockHeapArray[parentID] = stockHeapArray[childID];
                stockHeapArray[childID] = temp;
                // Since parent and child are swapped, we need to traverse down further
                parentID = childID;
                childID = parentID * 2 + 1;
            }
        }
    }
}


void SegmentHeapArrayFirstSort( SegmentNode** segmentHeap )
{
    SegmentNode* temp;
    // Start from last parent backward to do min heapify operation
    for (int parent = increasePeriod / 2; parent >= 0; parent--)
    {
        int parentID = parent;
        int childID = parentID * 2 + 1;

        while (childID < increasePeriod) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < increasePeriod && segmentHeap[childID + 1]->value < segmentHeap[childID]->value)
                childID++; // second child is smaller than first child
            if (segmentHeap[parentID]->value <= segmentHeap[childID]->value)
                break; // Done! Since the parent is smaller or equal to the smaller child
            else
            {
                // Let the smaller child turn parent, and downgrade the parent to the child
                // Swap child and parent
                temp = segmentHeap[parentID];
                segmentHeap[parentID] = segmentHeap[childID];
                segmentHeap[childID] = temp;
                // Since parent and child are swapped, we need to traverse down further
                parentID = childID;
                childID = parentID * 2 + 1;
            }
        }
    }
}



// For debug using. Check whether the heap array is a valid min heap
int CheckHeapArrayValidity(int printFlag)
{
    int cid;
    for (int pid = 0; pid <= arraySize / 2; pid++)
    {
        cid = pid * 2 + 1;
        if (cid < arraySize && stockHeapArray[pid]->segmentHeapArray[0]->value > stockHeapArray[cid]->segmentHeapArray[0]->value)
        {
            if (printFlag)  printf("\nERROR! parent H[%d] = %llu  > C[%d] = %llu\n\n", pid, stockHeapArray[pid]->segmentHeapArray[0]->value, cid, stockHeapArray[cid]->segmentHeapArray[0]->value);
            return 0;
        }
        cid++;
        if (cid < arraySize && stockHeapArray[pid]->segmentHeapArray[0]->value > stockHeapArray[cid]->segmentHeapArray[0]->value)
        {
            if (printFlag)  printf("\nERROR! parent H[%d] = %llu  > C[%d] = %llu\n\n", pid, stockHeapArray[pid]->segmentHeapArray[0]->value, cid, stockHeapArray[cid]->segmentHeapArray[0]->value);
            return 0;
        }
    }
    if (printFlag)   printf("\nHeap array is OK!!\n");
    return 1;
}


unsigned long long SequentialPoolFiltering(unsigned long long kSweet)
{
    SegmentNode* segmentTemp;
    StockNode* stockTemp;
    unsigned long long k = 1;

    while (k != kSweet)
    {
        // The smallest Stock node upgrades its heap to the next value
        stockHeapArray[0]->segmentHeapArray[0]->seqID += increasePeriod; // day id jump to next increased day
        unsigned long long v = price(stockHeapArray[0]->stockID, stockHeapArray[0]->segmentHeapArray[0]->seqID); // get upgraded value
        stockHeapArray[0]->segmentHeapArray[0]->value = v; // update value

        // Top-down min heapify the nested min-heap of the stock node
        int parentID = 0; // The root is upgraded
        int childID = 1;
        while (childID < increasePeriod) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < increasePeriod && stockHeapArray[0]->segmentHeapArray[childID + 1]->value < stockHeapArray[0]->segmentHeapArray[childID]->value)
                childID++; // second child is smaller than first child
            if (stockHeapArray[0]->segmentHeapArray[parentID]->value <= stockHeapArray[0]->segmentHeapArray[childID]->value)
            {
                break; // Done! Since the parent is smaller or equal to the smaller child
            }
            else
            {
                // Let the smaller child turn parent, and downgrade the parent to the child
                // Swap child and parent
                segmentTemp = stockHeapArray[0]->segmentHeapArray[parentID];
                stockHeapArray[0]->segmentHeapArray[parentID] = stockHeapArray[0]->segmentHeapArray[childID];
                stockHeapArray[0]->segmentHeapArray[childID] = segmentTemp;
                // Since parent and child are swapped, we need to traverse down further
                parentID = childID;
                childID = parentID * 2 + 1;
            }
        }

        // The root stock has upgraded its heap, now hepify the outer stock node heap
        parentID = 0; // The root stock has upgraded
        childID = 1;
        // Top-down min heapify the outer stock heap
        while (childID < activeNumber) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < activeNumber && stockHeapArray[childID + 1]->segmentHeapArray[0]->value < stockHeapArray[childID]->segmentHeapArray[0]->value)
                childID++; // second child is smaller than first child
            if (stockHeapArray[parentID]->segmentHeapArray[0]->value <= stockHeapArray[childID]->segmentHeapArray[0]->value)
            {
                break; // Done! Since the parent is smaller or equal to the smaller child
            }
            else
            {
                // Let the smaller child turn parent, and downgrade the parent to the child
                // Swap child and parent
                stockTemp = stockHeapArray[parentID];
                stockHeapArray[parentID] = stockHeapArray[childID];
                stockHeapArray[childID] = stockTemp;
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


    printf("  My answer => (%llu,%llu) = %llu \n", stockHeapArray[0]->stockID, stockHeapArray[0]->segmentHeapArray[0]->seqID, stockHeapArray[0]->segmentHeapArray[0]->value);

    return stockHeapArray[0]->segmentHeapArray[0]->value;
}



void main()
{

    printf("  ******************* Hierarchical HEAPs with Stock Nodes and Nested SegmentNodes ******************\n");

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

        // Allocate heap array that stores the pointers of outer stock nodes
        stockHeapArray = malloc((numOfStocks + 1) * sizeof(StockNode*));

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

            // Create Stock Node array
            for (int s = 0; s < activeNumber; s++)
            {
                stockHeapArray[s] = malloc(sizeof(StockNode));
                stockHeapArray[s]->stockID = activeIDs[s]; // The stock node stores its id
                // Create the heap array of its segments
                stockHeapArray[s]->segmentHeapArray = malloc( increasePeriod * sizeof(SegmentNode*));
                for (int p = 0; p < increasePeriod; p++)
                {
                    // Allocate memory for each segment 
                    stockHeapArray[s]->segmentHeapArray[p] = malloc(sizeof(SegmentNode));
                    stockHeapArray[s]->segmentHeapArray[p]->seqID = p + 1; // 1 + p * increasePeriod;
                    stockHeapArray[s]->segmentHeapArray[p]->value = price(stockHeapArray[s]->stockID, stockHeapArray[s]->segmentHeapArray[p]->seqID);
                }
                SegmentHeapArrayFirstSort(stockHeapArray[s]->segmentHeapArray);
            }

            // Sort the firstly constructed heap array to get the first round of values.
            StockHeapArrayFirstSort();
            // Debug            CheckHeapArrayValidity( 1 );

            printf("Query %d  s=%llu, k=%llu  => ", j, extra, kSweet);
            unsigned long long answer = SequentialPoolFiltering(kSweet);

            // Debug           printf("Query %d  s=%llu, k=%llu  => answer = %llu \n", j, extra, kSweet, answer);

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