
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

int numOfStocks, numQuery, increasePeriod, kSweet, extra;
int **stockIDs;
char answer[80];
FILE* filePtr;


typedef struct node
{
    unsigned long long value;
    struct node* next;
    struct node* prev;
} Node;

unsigned long long method1DoubleLinkedList(long long  extras, long long kSmallest)
{
    // A  doubly-linked sorted list of length of k is maintained 
    int count = extras == 0 ? numOfStocks : numOfStocks + 1;
    unsigned long long answer = pow(2, 127);
    Node* head = 0;
    Node* tail = 0;
    unsigned long long value;
    int loop = ceil(pow(10, 9) / increasePeriod);
    Node** flagPtrs;
    int numberSortedLists = count * increasePeriod;

    // A stock is divided into loop sub-lists, which are sorted lists
    // So totally there are loop * count sorted list
    // Each sublist contains a node flag, if flag value < 0 in initial state; 
    // otherwise, flag value = 0 means the rest of sublist consisting values larger than answer
    // otherwise, the pointer is the last smallest node 

    // malloc 記憶體長度無法開 10億個，放棄 flag
    //Node* dummy = malloc(sizeof(Node));

    //flagPtrs = malloc(sizeof(Node*) * all);
    //for (int i = 0; i < all; i++)flagPtrs[i] = dummy;
    
    int refreshCount = 0;
    int length = 0;
    int done = 0;
    for (int i = 0; i < loop; i++) // up 10^6
    {
        // each step read in the i-th element of each sub-stock line;
        // simply discard the element (value > answer) and set stop flag of that line; 
        // or insert into the list and discard the last element (tail) and update answer
        //if (length >= kSmallest) break;
        //refreshCount = 0;

            //if (refreshCount >= length) break;
        for (int s = 0; s < count; s++)
        {
            for (int p = 0; p < increasePeriod; p++) // up 1024
            {
                if (refreshCount >= numberSortedLists)
                {
                    // Looping through all lists; no further insertion;
                    // The current stored are the smallest k values
                    return tail->value;
                    break;
                }

                int sid = extras == 0 ? stockIDs[s] : extras;
                value = price(sid, p * increasePeriod + i);
                if (length <= kSmallest)
                {
                    // the stored list is not completed, insert the element update length
                    // start from self flag or head
                    Node* newOne = malloc(sizeof(Node));
                    newOne->value = value;

                    Node *ptr = head;
                    if (ptr == 0)
                    {
                        head = newOne;
                        tail = newOne;
                        newOne->prev = 0;
                        newOne->next = 0;
                    }
                    else
                    {
                        // search to the right sorted place
                        while (ptr != 0 && value > ptr->value) ptr = ptr->next;
                        if (ptr == 0)
                        {
                            // tail
                            tail->next = newOne;
                            newOne->prev = tail;
                            newOne->next = 0;
                            tail = newOne;
                        }
                        else
                        {
                            // Insert in front of ptr middle
                            Node* prev = ptr->prev;
                            if (prev == 0)
                            {
                                // new head
                                head = newOne;
                                newOne->prev = 0;
                                ptr->prev = newOne;
                                newOne->next = ptr;
                            }
                            else
                            {
                                // middle
                                prev->next = newOne;
                                newOne->prev = prev;
                                ptr->prev = newOne;
                                newOne->next = ptr;
                            }
                        }
                    }
                    length++;
                }
                else
                {
                    // the k-smallest stored list is full
                    // check whether the value can be inserted into the stored list
                    if (value >= tail->value)
                    {
                        // large than the largest vlaue of the stored list; discard this
                        // value add count traverse once
                        refreshCount++;
                    }
                    else
                    {
                        // Add this value to the stored list and expel the largest node
                        Node* ptr = head;                     // search to the right sorted place
                        while (ptr != 0 && value > ptr->value) ptr = ptr->next;
                        if (ptr == 0) // tail
                        {
                            // tail; simply replace value; no any pointer redirection
                            ptr->value = value;
                        }
                        else // head or middle
                        {
                            // Since tail is to be discarded;
                           // We store the value to tail
                            Node* newOne = tail;
                            tail = tail->prev; // discard tail
                            tail->next = 0;
                            newOne->value = value;
                            // Insert in front of ptr middle
                            Node* prev = ptr->prev;
                           if (prev == 0) // head
                            {
                                // new head
                                head = newOne;
                                newOne->prev = 0;
                                ptr->prev = newOne;
                                newOne->next = ptr;
                            }
                            else // middle
                            {
                                prev->next = newOne;
                                newOne->prev = prev;
                                ptr->prev = newOne;
                                newOne->next = ptr;
                            }

                        }
                        // Since a new small values is stored, reset the refresh counter to try another traverse.
                        if(i != loop-1) refreshCount = 1;
                    }                
                }
            }

        }
    }
    return tail->value;

}

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
    stockIDs = malloc(numOfStocks * sizeof(unsigned int));
    for (int i = 0; i < numOfStocks; i++)
    {
        fscanf(filePtr, "%d", &stockIDs[i]);
    }

    for (int j = 0; j < numQuery; j++)
    {
        fscanf(filePtr, "%d %d", &extra, &kSweet);

         printf("Query %d (s=%d,k=%d) => answer =",j,extra, kSweet );
        unsigned long long answer = method1DoubleLinkedList(extra, kSweet);
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