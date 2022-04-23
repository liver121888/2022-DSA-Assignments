
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


typedef struct query
{
    unsigned int stockFlag;
    unsigned int sweetK;
} Query;


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
SegmentNode** extraHeapArray;

int numOfStocks, numQuery, increasePeriod;
unsigned long long kSweet, extra;
unsigned long long* stockIDs;
Query* queryArray;

unsigned long long* dataBank;
unsigned int bankLength;


char answer[80];
FILE* filePtr;


// Construct the first heap array of StockNode according to their first min values.
// It will be constructed as a min-heap, where the root is the smallest and all children are larger than their parents.
// Once completed the root is the smallest node; yet the tree is not sorted (we don't need a sorted heap)!
void ConstrucStockHeapArrayFirstMinHeapifiedAndSetDataBank()
{
    StockNode* stockTemp;
    // Start from last parent backward to do min heapify operation
    for (int parent = numOfStocks / 2; parent >= 0; parent--)
    {
        int parentID = parent;
        int childID = parentID * 2 + 1;

        while (childID < numOfStocks) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < numOfStocks && stockHeapArray[childID + 1]->segmentHeapArray[0]->value < stockHeapArray[childID]->segmentHeapArray[0]->value)
                childID++; // second child is smaller than first child
            if (stockHeapArray[parentID]->segmentHeapArray[0]->value <= stockHeapArray[childID]->segmentHeapArray[0]->value)
                break; // Done! Since the parent is smaller or equal to the smaller child
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
    }

    // Construct the data bank 
    SegmentNode* segTemp;
    dataBank[0] = stockHeapArray[0]->segmentHeapArray[0]->value;
    for (int k = 1; k < bankLength; k++)
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
                segTemp = stockHeapArray[0]->segmentHeapArray[parentID];
                stockHeapArray[0]->segmentHeapArray[parentID] = stockHeapArray[0]->segmentHeapArray[childID];
                stockHeapArray[0]->segmentHeapArray[childID] = segTemp;
                // Since parent and child are swapped, we need to traverse down further
                parentID = childID;
                childID = parentID * 2 + 1;
            }
        }

        // The root stock has upgraded its heap, now hepify the outer stock node heap
        parentID = 0; // The root stock has upgraded
        childID = 1;
        // Top-down min heapify the outer stock heap
        while (childID < numOfStocks) // child is traversed one by one
        {
            // Select the child with the smaller value
            if (childID + 1 < numOfStocks && stockHeapArray[childID + 1]->segmentHeapArray[0]->value < stockHeapArray[childID]->segmentHeapArray[0]->value)
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
        dataBank[k] = stockHeapArray[0]->segmentHeapArray[0]->value;
    }
}


void SegmentHeapArrayFirstMinHeapified(SegmentNode** segmentHeap)
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



void main()
{

    printf("  ******************* Hierarchical HEAPs with Standard Stock Nodes for Data Sets and Extra Heap Insertion ******************\n");

    char fileName1[] = "..\\HWK2 Samples\\p5sample1.txt";
    char fileName2[] = "..\\HWK2 Samples\\p5sample2.txt";
    char fileName3[] = "..\\HWK2 Samples\\p5sample3.txt";
    char fileName4[] = "..\\HWK2 Samples\\p5sample4.txt";

    char answer[80];


    for (int f = 1; f <= 4; f++)
    {
        sprintf(answer, "..\\HWK2 Samples\\p5sample%d.txt", f);

        clock_t startTime = clock();

        filePtr = fopen(answer, "r");
        printf("\nBenchmark file: %s\n", answer);

        fscanf(filePtr, "%d %d %d", &numOfStocks, &numQuery, &increasePeriod);

        // Store stock ids in arrays
        stockIDs = malloc((numOfStocks) * sizeof(unsigned long long));

        // Allocate heap array that stores the pointers of outer stock nodes
        stockHeapArray = malloc(numOfStocks * sizeof(StockNode*));

        unsigned long long id = 0;
        for (int i = 0; i < numOfStocks; i++)
        {
            fscanf(filePtr, "%llu", &id); // or %I64u
            stockIDs[i] = id;
        }

        queryArray = malloc(sizeof(Query) * numQuery);
        bankLength = 0;

        for (int j = 0; j < numQuery; j++)
        {
            fscanf(filePtr, "%llu %llu", &queryArray[j].stockFlag, &queryArray[j].sweetK);
            if (bankLength < queryArray[j].sweetK) bankLength = queryArray[j].sweetK;
        }

        // Prepare data bank
        dataBank = malloc(bankLength * sizeof(unsigned long long));

        // Create Stock Node array
        for (int s = 0; s < numOfStocks; s++)
        {
            stockHeapArray[s] = malloc(sizeof(StockNode));
            stockHeapArray[s]->stockID = stockIDs[s]; // The stock node stores its id
            // Create the heap array of its segments
            stockHeapArray[s]->segmentHeapArray = malloc(increasePeriod * sizeof(SegmentNode*));
            for (int p = 0; p < increasePeriod; p++)
            {
                // Allocate memory for each segment 
                stockHeapArray[s]->segmentHeapArray[p] = malloc(sizeof(SegmentNode));
                stockHeapArray[s]->segmentHeapArray[p]->seqID = p + 1; // 1 + p * increasePeriod;
                stockHeapArray[s]->segmentHeapArray[p]->value = price(stockHeapArray[s]->stockID, stockHeapArray[s]->segmentHeapArray[p]->seqID);
            }
            SegmentHeapArrayFirstMinHeapified(stockHeapArray[s]->segmentHeapArray);
        }

        // Allocate extra memory for the extra stock
        extraHeapArray = malloc(increasePeriod * sizeof(SegmentNode*));
        for (int i = 0; i < increasePeriod; i++)
        {
            extraHeapArray[i] = malloc(sizeof(SegmentNode));
        }

        ConstrucStockHeapArrayFirstMinHeapifiedAndSetDataBank();
        // Sort the firstly constructed heap array to get the first round of values.


    // Answer the queries one by one

        for (int i = 0; i < numQuery; i++)
        {
            if (queryArray[i].stockFlag == 0)
            {
                printf("Query %i  (s=%d k=%d) My answer => %llu \n", i, queryArray[i].stockFlag, queryArray[i].sweetK, dataBank[queryArray[i].sweetK - 1]);
            }
            else
            {
                // Construct the initial heap array
                // Set nodes first
                for (int p = 0; p < increasePeriod; p++)
                {
                    // first non-sorted heap array 
                    extraHeapArray[p]->seqID = p + 1; // 1 + p * increasePeriod;
                    extraHeapArray[p]->value = price(queryArray[i].stockFlag, extraHeapArray[p]->seqID);
                }
                // Complete the min-heap
                SegmentNode* temp;
                // Start from last parent backward to do min heapify operation
                for (int parent = increasePeriod / 2; parent >= 0; parent--)
                {
                    int parentID = parent;
                    int childID = parentID * 2 + 1;

                    while (childID < increasePeriod) // child is traversed one by one
                    {
                        // Select the child with the smaller value
                        if (childID + 1 < increasePeriod && extraHeapArray[childID + 1]->value < extraHeapArray[childID]->value)
                            childID++; // second child is smaller than first child
                        if (extraHeapArray[parentID]->value <= extraHeapArray[childID]->value)
                            break; // Done! Since the parent is smaller or equal to the smaller child
                        else
                        {
                            // Let the smaller child turn parent, and downgrade the parent to the child
                            // Swap child and parent
                            temp = extraHeapArray[parentID];
                            extraHeapArray[parentID] = extraHeapArray[childID];
                            extraHeapArray[childID] = temp;
                            // Since parent and child are swapped, we need to traverse down further
                            parentID = childID;
                            childID = parentID * 2 + 1;
                        }
                    }
                }

                // 
                int bankIdx = queryArray[i].sweetK - 1;
                unsigned long long value = dataBank[bankIdx];
                unsigned long long nextValue = dataBank[--bankIdx];
                unsigned long long extraValue = extraHeapArray[0]->value;
                unsigned long long answer;

                while (1)
                {
                    if (extraValue > value)
                    {
                        answer = value;
                        break;
                    }
                    else if (extraValue > nextValue && extraValue < value)
                    {
                        answer = extraValue;
                        break;
                    }
                    else
                    {
                        // Lower down data bank one item
                        value = nextValue;
                        nextValue = dataBank[--bankIdx];

                        // extra heap root upgrade 
                        // The smallest node upgrades to next value
                        extraHeapArray[0]->seqID += increasePeriod; // day id jump to next increased day
                        unsigned long long v = price(queryArray[i].stockFlag, extraHeapArray[0]->seqID); // get upgraded value
                        extraHeapArray[0]->value = v; // update value

                        int parentID = 0; // The root is upgraded
                        int childID = 1;
                        // Top-down min heapify??
                        while (childID < increasePeriod) // child is traversed one by one
                        {
                            // Select the child with the smaller value
                            if (childID + 1 < increasePeriod && extraHeapArray[childID + 1]->value < extraHeapArray[childID]->value)
                                childID++; // second child is smaller than first child
                            if (extraHeapArray[parentID]->value <= extraHeapArray[childID]->value)
                            {
                                break; // Done! Since the parent is smaller or equal to the smaller child
                            }
                            else
                            {
                                // Let the smaller child turn parent, and downgrade the parent to the child
                                // Swap child and parent
                                temp = extraHeapArray[parentID];
                                extraHeapArray[parentID] = extraHeapArray[childID];
                                extraHeapArray[childID] = temp;
                                // Since parent and child are swapped, we need to traverse down further
                                parentID = childID;
                                childID = parentID * 2 + 1;
                            }
                        }
                        extraValue = extraHeapArray[0]->value;
                    }
                }

                // 
                printf("Query %i  (s=%d k=%d) My answer => %llu \n", i, queryArray[i].stockFlag, queryArray[i].sweetK, answer);

            }
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