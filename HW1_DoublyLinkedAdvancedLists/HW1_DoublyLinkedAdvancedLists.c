
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

typedef struct dlitem
{
    int id, grpIdx;
    struct dlitem* prevXnext, * next;
} DLItem;

// Global Data
int QueueNumber; // M
int EventNumber; // N
int GroupNumber; // K
FILE* filePtr;
char* Closed;
DLItem** Heads;
DLItem** Tails;

DLItem*** GroupEnds;
DLItem*** GroupStarts; // used in Dismiss functions

void TailLeave(int queue)
{
    DLItem* prevGuy;

    if (Tails[queue])
    {
        int gid = Tails[queue]->grpIdx;
        prevGuy = Tails[queue]->prevXnext;
        if (prevGuy)
        {
            // previous guy exits
            Tails[queue] = prevGuy;
            prevGuy->next = 0;
            if (prevGuy->grpIdx == gid)
            {
                // SameGroup
                GroupEnds[queue][gid] = prevGuy;
            }
            else
            {
                // Different group
                GroupStarts[queue][gid] = 0;
                GroupEnds[queue][gid] = 0;
            }
        }
        else
        {
            // previous is null (The only item) after leave empty
            GroupEnds[queue][gid] = 0;
            GroupStarts[queue][gid] = 0;
            Heads[queue] = 0;
            Tails[queue] = 0;
        }
    }
}

void GoDequeu(int queue)
{
    DLItem* nextGuy;

    if (Heads[queue])
    {
        int gid = Heads[queue]->grpIdx;

        nextGuy = Heads[queue]->next;
        if (nextGuy)
        {
            // next Guy exists
            Heads[queue] = nextGuy;
            nextGuy->prevXnext = 0;
            if (nextGuy->grpIdx == gid)
            {
                GroupStarts[queue][gid] = nextGuy;
            }
            else
            {
                // different group pop up
                GroupStarts[queue][gid] = 0;
                GroupEnds[queue][gid] = 0;
            }
        }
        else
        {
            // no next guy; 
            GroupEnds[queue][gid] = 0;
            GroupStarts[queue][gid] = 0;
            Heads[queue] = 0;
            Tails[queue] = 0;
        }



    }
}

void Enter(int group, int id, int queue)
{
    DLItem* enteredGuy = malloc(sizeof(DLItem));
    enteredGuy->id = id;
    enteredGuy->grpIdx = group;

    DLItem* groupEnd = GroupEnds[queue][group];
    if (groupEnd)
    {
        // The group exist in this queue
        DLItem* lastNext = groupEnd->next;
        groupEnd->next = enteredGuy;
        enteredGuy->prevXnext = groupEnd;
        enteredGuy->next = lastNext;
        if (lastNext) lastNext->prevXnext = enteredGuy;
        else Tails[queue] = enteredGuy;
        // new group end
        GroupEnds[queue][group] = enteredGuy;
    }
    else
    {
        // This is the first guy of a new group enters this queue
        // Enter tail of the queue
        if (Tails[queue])
        {
            // Tail exists
            Tails[queue]->next = enteredGuy;
            enteredGuy->prevXnext = Tails[queue];
            enteredGuy->next = 0;
            Tails[queue] = enteredGuy;
        }
        else
        {
            // Empty queue
            Heads[queue] = enteredGuy;
            Tails[queue] = enteredGuy;
            enteredGuy->prevXnext = 0;
            enteredGuy->next = 0;
        }
        GroupStarts[queue][group] = enteredGuy;
        GroupEnds[queue][group] = enteredGuy;
    }
}



void QueueDismissWithGroupStartsNEnds(int queue)
{
    int jointQueue = queue;
    do
    {
        jointQueue--;
        if (jointQueue < 0) jointQueue += QueueNumber;
    } while (Closed[jointQueue] == 'y');

    DLItem* tail;
    tail = Tails[queue]; // copy The last group end
    while (tail)
    {
        int gid = tail->grpIdx; // The current group id
        tail = GroupStarts[queue][gid]->prevXnext; // The next group end for next iteration

        // Reverse id of this group
        DLItem* endGuy = GroupEnds[queue][gid];
        DLItem* startGuy = GroupStarts[queue][gid];
        int id = endGuy->id;
        while ( startGuy->id != id )
        {
            endGuy->id = startGuy->id;
            startGuy->id = id;
            if (endGuy->prevXnext == startGuy) break;
            endGuy = endGuy->prevXnext;
            id = endGuy->id;
            startGuy = startGuy->next;
        }

        // link group gid in queue to jointQueue
        DLItem* joinStart = GroupStarts[jointQueue][gid], *jointEnd = GroupEnds[jointQueue][gid];
        startGuy = GroupStarts[queue][gid];
        endGuy = GroupEnds[queue][gid];

        if (jointEnd) 
        {
            // group exist
            DLItem* prevNext = jointEnd->next;
            jointEnd->next = startGuy;
            startGuy->prevXnext = jointEnd;

            endGuy->next = prevNext;
            if (prevNext) prevNext->prevXnext = endGuy;
            else Tails[jointQueue] = endGuy;

            GroupEnds[jointQueue][gid] = endGuy; // group added, update end
        }
        else
        {
            // The group is not in the jointQueue
            // append to tail of jointQueue
            jointEnd = Tails[jointQueue];
            if (jointEnd)
            {
                // Tail is not empty
                jointEnd->next = startGuy;
                startGuy->prevXnext = jointEnd;

                Tails[jointQueue] = endGuy;
                endGuy->next = 0;
            }
            else
            {
                // joint queue is empty

                Heads[jointQueue] = startGuy;
                Tails[jointQueue] = endGuy;
                startGuy->prevXnext = 0;
                endGuy->next = 0;
            }

            GroupStarts[jointQueue][gid] = startGuy;
            GroupEnds[jointQueue][gid] = endGuy;
        }

    }

    for (int j = 0; j < GroupNumber; j++)
    {
        GroupEnds[queue][j] = GroupStarts[queue][j] = 0;
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
        printf("       ##  ");
        for (int j = 0; j < GroupNumber; j++)
            printf("G(%d) s:%d e:%d  ", j, GroupStarts[q][j] ? GroupStarts[q][j]->id : -1, GroupEnds[q][j] ? GroupEnds[q][j]->id : -1);
    }
}

int main()
{
    // ntu
    char fileName1[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues001.txt";
    char fileName2[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    char fileName4[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues004.txt";

    // char fileName1[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues001.txt";
    // char fileName2[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    // char fileName3[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues003.txt";

    // char fileName1[] = "D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues001.txt";
    // char fileName2[] = "D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    // char fileName4[] = "D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues004.txt";
    char answer[80];

    // filePtr = fopen(fileName1, "r");
    filePtr = fopen(fileName4, "r");
    // filePtr = fopen(fileName3, "r");

    fscanf(filePtr, "%d %d %d", &QueueNumber, &EventNumber, &GroupNumber);
    // Allocate memory for these queues
    Heads = malloc(sizeof(DLItem*) * QueueNumber);
    Tails = malloc(sizeof(DLItem*) * QueueNumber);
    GroupEnds = malloc(sizeof(DLItem**) * QueueNumber); // used in dismiss function
    GroupStarts = malloc(sizeof(DLItem**) * QueueNumber); // used in dismiss function

    Closed = malloc(sizeof(char) * 4);

    for (int i = 0; i < QueueNumber; i++)
    {
        Closed[i] = 'n';
        Heads[i] = 0;
        Tails[i] = 0;
        GroupStarts[i] = malloc(sizeof(DLItem*) * GroupNumber);
        GroupEnds[i] = malloc(sizeof(DLItem*) * GroupNumber);
        for (int j = 0; j < GroupNumber; j++)
        {
            GroupStarts[i][j] = 0;
            GroupEnds[i][j] = 0;
        }
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
            //QueueDismiss(queueIdx);
            QueueDismissWithGroupStartsNEnds(queueIdx);
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
