
#define _CRT_SECURE_NO_DEPRECATE
#include "price.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


typedef struct bstNode
{
    unsigned long long price;
    struct bstNode *left;
    struct bstNode* right;
} BstNode;



struct bstNode* balanceBST(struct bstNode* root) 
{

}

void InsertAPrice(unsigned long long p, BstNode *stock)
{
    if (stock == 0)
    {
        stock = malloc(sizeof(BstNode));
        stock->price = p;
        stock->left = 0;
        stock->right = 0;
    }
    else
    {
        if (p <= stock->price)
        {
            InsertAPrice(p, stock->left);
        }
        else
        {
            InsertAPrice(p, stock->right);
        }
    }
}

//BstNode* stocks;

int numOfStocks, numQuery, increasePeriod, kSweet, extra, activeNumber;
unsigned int* stockIDs,*activeIDs;
unsigned long long* firstValues;
//unsigned long
char answer[80];
FILE* filePtr;


typedef struct node
{
    unsigned long long value;
    struct node* next;
    struct node* prev;
} Node;

void showList(Node* h)
{
    Node* ptr = h;
    while (ptr)
    {
        printf("%u ", ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}

void CorrectQuickSort(int p, int r)
{
    if (p >= r) return;
    unsigned long long x = firstValues[r]; // last element is the pivot
    // i: left upper bound
    int i = p - 1;
    unsigned int temp;
    unsigned long long tempV;
    for( int j = p; j <= r-1; j++)
        if (firstValues[j] <= x)
        {
            // extend
            i++;
            // move element j to left part
            temp = activeIDs[i];
            activeIDs[i] = activeIDs[j];
            activeIDs[j] = temp;
            tempV = firstValues[i];
            firstValues[i] = firstValues[j];
            firstValues[j] = tempV;
        }
    // extend 
    i++;
    // insert the pivot between
    temp = activeIDs[i];
    activeIDs[i] = activeIDs[r];
    activeIDs[r] = temp;
    tempV = firstValues[i];
    firstValues[i] = firstValues[r];
    firstValues[r] = tempV;

    //for (int i = 0; i < activeNumber; i++)
    //    printf("%d ", firstValues[i]);
    //printf("\n");

    CorrectQuickSort(p, i - 1);
    CorrectQuickSort(i+1, r);
}


void quicksort( int left, int right) // 輸入資料，和從兩邊開始的位置
{
    if (left >= right) return; // 如果左邊大於右邊，就跳出function
    int i = left; // 左邊的代理人
    int j = right; // 右邊的代理人
    unsigned long long key = firstValues[ left]; // 基準點
    int temp;
    unsigned long long tempV;
    while (i!=j)
    {
        while ( i < j && firstValues[j]  > key ) j--;  // 從右邊開始找，找比基準點小的值
        while (i < j && firstValues[i]  <= key) i++; // 從左邊開始找，找比基準點大的值
        if (i < j) // 當左右代理人沒有相遇時，互換值
        {
            temp = activeIDs[i];
            activeIDs[i] = activeIDs[j];
            activeIDs[j] = temp;
            tempV = firstValues[i];
            firstValues[i] = firstValues[j];
            firstValues[j] = tempV;
        }
    }
    // 將基準點歸換至代理人相遇點
    temp = activeIDs[left];
    activeIDs[left] = activeIDs[i];
    activeIDs[i] = temp;
    tempV = firstValues[left];
    firstValues[left] = firstValues[i];
    firstValues[i] = tempV;

    for (int i = 0; i < activeNumber; i++)
        printf("%d ", firstValues[i]);
    printf("\n");

    quicksort(activeIDs, left, i - 1);   // 繼續處理較小部分的子循環
    quicksort(activeIDs, i + 1, right); //  繼續處理較大部分的子循環
}


unsigned long long method2TraverseFromSmallestAndCountTraversed(long long  extras, long long kSmallest)
{
    int count = 0;
    int batchsize = activeNumber * increasePeriod;
    int loop;
    unsigned long long value, nextValue;
    for (loop = 0; 1 ; loop++) // Carry out one batch traverse
    {
        for (int p = 1; p <= increasePeriod; p++) // up 1024
        {
            for (int s = 0; s < activeNumber; s++)
            {
                int pid = loop * increasePeriod + p;
                if (s == 0) value = price(activeIDs[s],pid );
                else  value = nextValue;

                if (s != activeNumber - 1)
                {
                    nextValue = price(activeIDs[s + 1], pid);
                    if (value > nextValue)
                    {
                        // swap id
                        unsigned int temp = activeIDs[s + 1];
                        activeIDs[s + 1] = activeIDs[s];
                        activeIDs[s] = temp;
                        nextValue = value;
                    }
                }
                count++;
                if (count == kSmallest) return value;
                printf("%d = %u \n", count, value);
            }
        }
    } 

    return 0;
}




// The following does not work!
// A stock is divided into N sub-lists. Totally there are (|A|+1)N = M sorted lists
// Repeatedly traverse M lists and update a doubly linked sorted list L of M so-far largest values.
// The traversed element count C is therefore increased by M in each traverse batch.
// C = b * M, after b batch traversals.  
// If the traversed count C >= K, stop the batch traverse and the K-th smallest value
// is the K - (b-1)M element in the list L.
unsigned long long method1DoubleLinkedList(long long  extras, long long kSmallest)
{
    int count = extras == 0 ? numOfStocks : numOfStocks + 1;
    unsigned long long answer = pow(2, 127);
    int numberSortedLists = count * increasePeriod;
    // A  doubly-linked sorted list of length of numberSortedLists is maintained 
    Node* head = 0;
    Node* tail = 0;
    Node* newOne;
    Node* ptr;
    Node* prev;
    unsigned long long value;

    int traversedCount = 0;
    int loop;
    for (loop = 0; traversedCount < kSmallest; loop++ ) // Carry out one batch traverse
    {
        for (int s = 0; s < count; s++)
        {
            for (int p = 0; p < increasePeriod; p++) // up 1024
            {

                int sid = extras == 0 ? stockIDs[s] : extras;
                value = price(sid, p * increasePeriod + loop);


                if (loop==0)
                {
                    // Build the doubly linked list.
                    // start from self flag or head
                    newOne = malloc(sizeof(Node));
                    newOne->value = value;

                    ptr = head;
                    if (ptr == 0)
                    {
                        head = newOne;
                        tail = newOne;
                        newOne->prev = 0;
                        newOne->next = 0;
                    }
                    else
                    {
                        // search for the right place to insert the element
                        while (ptr != 0 && value > ptr->value) ptr = ptr->next;
                        if (ptr == 0) // tail
                        {
                            tail->next = newOne;
                            newOne->prev = tail;
                            newOne->next = 0;
                            tail = newOne;
                        }
                        else // Insert in front of ptr 
                        {
                            prev = ptr->prev;
                            if (prev == 0) // new head
                            {
                                head = newOne;
                                newOne->prev = 0;
                                ptr->prev = newOne;
                                newOne->next = ptr;
                            }
                            else  // inserted in the middle
                            {
                                prev->next = newOne;
                                newOne->prev = prev;
                                ptr->prev = newOne;
                                newOne->next = ptr;
                            }
                        }
                    }
                }
                else
                {
                    printf("Add %d -> ", value);
                    showList(head);

                    // Insert the element to update the list and maintain the length 
                    // by discarding the smallest (head)
                    // search for the right place to insert the element
                    ptr = head;
                    while (ptr != 0 && value > ptr->value) ptr = ptr->next;

                    if (ptr == head)
                    {
                        // smaller or equal to the smallest value
                        continue;
                    }
                   // head is to be discarded, but memory is used to store the new value
                   newOne = head;
                   // Store the new value
                   newOne->value = value;



                   if (ptr == 0) // tail
                   {
                       // discard head and update the pointer
                       if (head->next)
                       {
                           head = head->next;
                           head->prev = 0; // head prev must be 0
                       }
                       // new tail
                       tail->next = newOne;
                       newOne->prev = tail;
                       tail = newOne;
                       tail->next = 0; // tail next must be 0
                   }
                   else // Insert in front of ptr; value smaller than or equal to ptr
                   {
                       prev = ptr->prev; // store previous pointer
                       if (prev == head || prev == 0)
                       {
                           // No head update
                       }
                       else
                       {
                           if (prev)
                           {

                               // discard head and update the pointer
                               if (head->next)
                               {
                                   head = head->next;
                                   head->prev = 0; // head prev must be 0
                               }
                               prev->next = newOne;
                               newOne->prev = prev;
                               ptr->prev = newOne;
                               newOne->next = ptr;
                           }
                       }
                   }

                    showList(head);
                }


            }
        }
        traversedCount += increasePeriod * count;
        //ptr = head;
        //printf("Loop %d :  ", loop);
        //while (ptr)
        //{
        //    printf("%u ", ptr->value);
        //    ptr = ptr->next;
        //}
        //printf("\n");
    }
    int pos = kSmallest - (loop - 1) * (increasePeriod * count);
    int c = 1;
    ptr = head;
    while (c < pos)
    {
        ptr = ptr->next;
        c++;
    }
    return ptr->value;
}


void main()
{

    char fileName1[] = "..\\HWK2 Samples\\p5sample1.txt";
    char fileName2[] = "..\\HWK2 Samples\\p5sample2.txt";
    char fileName3[] = "..\\HWK2 Samples\\p5sample3.txt";
    char fileName4[] = "..\\HWK2 Samples\\p5sample4.txt";

    char answer[80];

  //   filePtr = fopen(fileName1, "r");
    filePtr = fopen(fileName2, "r");
    // filePtr = fopen(fileName4, "r");
    // filePtr = fopen(fileName3, "r");

    fscanf(filePtr, "%d %d %d", &numOfStocks, &numQuery, &increasePeriod);
    stockIDs = malloc((numOfStocks )* sizeof(unsigned int));
    activeIDs = malloc((numOfStocks + 1) * sizeof(unsigned int));
    firstValues = malloc((numOfStocks + 1) * sizeof(unsigned long long));

    for (int i = 0; i < numOfStocks; i++)
    {
        fscanf(filePtr, "%d", &stockIDs[i]);
        firstValues[i] = price(stockIDs[i], 1);
    }

    for (int j = 0; j < numQuery; j++)
    {
        fscanf(filePtr, "%d %d", &extra, &kSweet);
        if (extra == 0)
        {
            activeNumber = numOfStocks;
        }
        else
        {
            activeNumber = numOfStocks + 1;
            stockIDs[numOfStocks] = extra;
            firstValues[numOfStocks] = price(extra, 1);
        }
        for (int i = 0; i < activeNumber; i++)
            activeIDs[i] = stockIDs[i];

        // Sort the stockIDs based on their first values
        for (int i = 0; i < activeNumber; i++)
            printf("%d = %d ", activeIDs[i], firstValues[i]);
        printf("\n");

       // quicksort( 0, activeNumber-1);
        CorrectQuickSort(0, activeNumber - 1);

        for (int i = 0; i < activeNumber; i++)
            printf("%d = %d ", activeIDs[i], firstValues[i]);
        printf("\n");

        //unsigned long long answer = method1DoubleLinkedList(extra, kSweet);
        unsigned long long answer = method2TraverseFromSmallestAndCountTraversed(extra, kSweet);
         printf("Query %d (s=%d,k=%d) => answer =",j,extra, kSweet );
        printf(" %u\n",  answer);



    }
    printf("Done! Check answer ...\n");
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