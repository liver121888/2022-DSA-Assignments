
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

typedef struct dlitem
{
    int id, grpIdx;
    struct dlitem* prev, * next;
} DLItem;

// Global Data
int QueueNumber; // M
int EventNumber; // N
int GroupNumber; // K
FILE* filePtr;
char* Closed;
DLItem** Heads;
DLItem** Tails;

DLItem** GroupTails; // used in Dismiss functions

void TailLeave(int queue)
{
    if (Tails[queue])
    {
        if (Heads[queue] == Tails[queue]) Heads[queue] = Tails[queue]->prev;
        Tails[queue] = Tails[queue]->prev;
        if( Tails[queue] ) Tails[queue]->next = 0;
    }
}

void GoDequeu(int queue)
{
    if (Heads[queue])
    {
        if (Tails[queue] == Heads[queue]) Tails[queue] = Heads[queue]->next;
        Heads[queue] = Heads[queue]->next;
        if( Heads[queue] ) Heads[queue]->prev = 0;

    }
}

void Enter(int group, int id, int queue)
{
    DLItem* ptr = Tails[queue];
    while (ptr)
    {
        if (ptr->grpIdx == group)
        {
            // Create an Item to insert before current ptr
            DLItem* newOne = malloc(sizeof(DLItem));
            newOne->id = id;
            newOne->grpIdx = group;

            newOne->prev = ptr;
            newOne->next = ptr->next;
            newOne->prev->next = newOne;
            if (newOne->next) newOne->next->prev = newOne;
            if (ptr == Tails[queue])Tails[queue] = newOne;
            return;
        }
        ptr = ptr->prev;
    }
    // Added to tail
    DLItem* newOne = malloc(sizeof(DLItem));
    newOne->id = id;
    newOne->grpIdx = group;
    if (Tails[queue])
    {
        Tails[queue]->next = newOne;
        newOne->prev = Tails[queue];
        newOne->next = 0;
        Tails[queue] = newOne;
    }
    else
    {
        newOne->next = 0;
        newOne->prev = 0;
        Heads[queue] = newOne;
        Tails[queue] = newOne;
    }
}

void QueueDismiss(int queue)
{
    int newQueue = queue;
    do
    {
        newQueue --;
        if (newQueue < 0) newQueue += QueueNumber;
    }while (Closed[newQueue] == 'y');

    DLItem* target, * frontOne;
    target = Tails[queue];
    while (target)
    {
        frontOne = target->prev;
        Enter(target->grpIdx, target->id, newQueue);
        target = frontOne;
    }
    Heads[queue] = 0;
    Tails[queue] = 0;
    Closed[queue] = 'y';
}


void QueueDismissViaReverse(int queue)
{
    int newQueue = queue;
    do
    {
        newQueue--;
        if (newQueue < 0) newQueue += QueueNumber;
    } while (Closed[newQueue] == 'y');

    DLItem* ptr;
    ptr = Tails[newQueue];
    //while (target)
    //{
    //    frontOne = target->prev;
    //    Enter(target->grpIdx, target->id, newQueue);
    //    target = frontOne;
    //}

    // Find the tailer element of each group in destination queue 
    for (int i = 0; i < GroupNumber; i++) GroupTails[i] = 0;
    ptr = Tails[newQueue];
    int gid = -1;
    while (ptr)
    {
        if (ptr->grpIdx != gid)
        {
            gid = ptr->grpIdx;
            GroupTails[gid] = ptr; // get tail of group gid at queue newQueue
        }
        ptr = ptr->prev;
    }

    // Backward traverse the dismissed queue
    ptr = Tails[queue];
    gid = -1;


    DLItem* groupNext = 0;
    DLItem* last = 0, *previous = 0;
    // Backward traverse all elements
    while (ptr)
    {
        if (ptr->grpIdx != gid)
        {
            previous = ptr->prev;

            // A new group encountered
            gid = ptr->grpIdx;
            if (GroupTails[gid])
            {
                // The group exists at the target queue, reconnect 
                // traverse 
                groupNext = GroupTails[gid]->next;

                GroupTails[gid]->next = ptr;
                ptr->next = ptr->prev;
                ptr->prev = GroupTails[gid];            
            }
            else
            {
                // No such group in newQueue 
                groupNext = 0;

                if (Tails[newQueue])
                {
                    Tails[newQueue]->next = ptr;
                    ptr->next = ptr->prev;
                    ptr->prev = Tails[newQueue];
               }
                else
                {
                    Tails[newQueue] = ptr;
                    ptr->next = ptr->prev;
                    ptr->prev = GroupTails[gid];
                }
            }
            last = ptr;

            // preview next group
            if (ptr->next == 0 || ptr->next->grpIdx != gid)
            {
                ptr->next = groupNext;
                if (groupNext)  groupNext->prev = ptr;
                else Tails[newQueue] = ptr;
            }

        }
        else
        {
            // same group swap prev and next
            previous = ptr->prev;

            ptr->next = ptr->prev;
            ptr->prev = last;

            // preview next group
            if (ptr->next == 0 || ptr->next->grpIdx != gid)
            {
                ptr->next = groupNext;
                if (groupNext)  groupNext->prev = ptr;
                else Tails[newQueue] = ptr;
            }
            last = ptr;
        }
        ptr = previous;
    }
  

    Heads[queue] = 0;
    Tails[queue] = 0;
    Closed[queue] = 'y';
}

void PrintQueueInfo()
{
    DLItem* ptr;
    for (int q = 0; q < QueueNumber; q++)
    {
        printf("\nQ%d: ", q);
        ptr = Heads[q];
        while (ptr)
        {
            printf("%d(%d) ", ptr->id, ptr->grpIdx);
            ptr = ptr->next;
        }
    }
}

int main()
{
    char fileName1[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues001.txt";
    char fileName2[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    char fileName3[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues003.txt";
    char answer[80];

    // filePtr = fopen(fileName1, "r");
    //filePtr = fopen(fileName2, "r");
    filePtr = fopen(fileName3, "r");

    fscanf(filePtr, "%d %d %d", &QueueNumber, &EventNumber, &GroupNumber);
    // Allocate memory for these queues
    Heads = malloc(sizeof(DLItem*) * QueueNumber);
    Tails = malloc(sizeof(DLItem*) * QueueNumber);
    GroupTails = malloc( sizeof(DLItem*) * GroupNumber); // used in dismiss function

    Closed = malloc(sizeof(char) * 4);
    for (int i = 0; i < QueueNumber; i++)
    {
        Closed[i] = 'n';
        Heads[i] = 0;
        Tails[i] = 0;
    }

    int groupIdx, ID, queueIdx;
    char eventType[6];
    for (int e = 0; e < EventNumber; e++)
    {
        fscanf(filePtr, "%s", eventType);
        switch (eventType[0])
        {
        case 'e':// enter
            fscanf(filePtr, "%d %d %d", &groupIdx, &ID, &queueIdx);
            printf("\n\n\t=> %d(%d) enters Q%d ", ID, groupIdx, queueIdx);
            Enter(groupIdx, ID, queueIdx);
            break;
        case 'l': // leave
            fscanf(filePtr, "%d", &queueIdx);
            printf("\n\n\t=> Q%d tail leaves ", queueIdx);
            TailLeave(queueIdx);
            break;
        case 'g': // go
            fscanf(filePtr, "%d", &queueIdx);
            printf("\n\n\t=> Q%d head leaves ", queueIdx);
            GoDequeu(queueIdx);
            break;
        case 'c': // close
            fscanf(filePtr, "%d", &queueIdx);
            // QueueDismiss(queueIdx);
            printf("\n\n\t=> Q%d closes ", queueIdx);
            QueueDismissViaReverse(queueIdx);
            break;
        default:
            break;
        }
        PrintQueueInfo();
    }
    while ((fgets(answer, 80, filePtr)) != NULL) {}
    fclose(filePtr);

    printf("\n******** Correct Answer *************\n");
    printf("%s", answer);

    printf("\n********** Our Solution ***********\n");
    DLItem* ptr;
    for (int q = 0; q < QueueNumber; q++)
    {
        ptr = Heads[q];
        while (ptr)
        {
            printf("%d ", ptr->id);
            ptr = ptr->next;
        }
        printf("\n");
    }

    scanf("%d");
}
