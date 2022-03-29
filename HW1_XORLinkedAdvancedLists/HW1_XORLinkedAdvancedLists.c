
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

typedef struct dlitem
{
    int id, grpIdx;
    struct dlitem* prevXnext;
} DLItem;

// Global Data
int QueueNumber; // M
int EventNumber; // N
int GroupNumber; // K
FILE* filePtr;
char* Closed;
DLItem** Heads;
DLItem** Tails;

DLItem*** GroupEnds, *** GroupEndNexts;
DLItem*** GroupStarts, *** GroupStartPrevs; // used in Dismiss functions


void PrintQueueInfo()
{
    DLItem* ptr, * last;
    for (int q = 0; q < QueueNumber; q++)
    {
        printf("\nQ%d: ", q);
        last = 0;
        ptr = Heads[q];
        while (ptr)
        {
            printf("%d(%d) ", ptr->id, ptr->grpIdx);
            DLItem* next = (unsigned int)ptr->prevXnext ^ (unsigned int)last;
            last = ptr;
            ptr = next;
        }
        printf("       ##  ");
        for (int j = 0; j < GroupNumber; j++)
            printf(" G(%d) p %d s %d e %d n %d", j, GroupStartPrevs[q][j] ? GroupStartPrevs[q][j]->id : -1, GroupStarts[q][j] ? GroupStarts[q][j]->id : -1, GroupEnds[q][j] ? GroupEnds[q][j]->id : -1, GroupEndNexts[q][j] ? GroupEndNexts[q][j]->id : -1);
    }
}

void TailLeave(int queue)
{
    DLItem* prevGuy;

    if (Tails[queue])
    {
        int gid = Tails[queue]->grpIdx;
        prevGuy = (unsigned int)Tails[queue]->prevXnext ^ 0; // tailer next is 0
        if (prevGuy)
        {
            // previous guy exits
            if (prevGuy->grpIdx == gid)
            {
                // SameGroup, group end leaves; update end
                GroupEnds[queue][gid] = prevGuy;
                GroupEndNexts[queue][gid] = 0; // Tails[queue];
            }
            else
            {
                // Different group
                GroupStartPrevs[queue][gid] = 0;
                GroupStarts[queue][gid] = 0;
                GroupEnds[queue][gid] = 0;
                GroupEndNexts[queue][gid] = 0;
            }
            prevGuy->prevXnext = ((unsigned int)prevGuy->prevXnext ^ (unsigned int)Tails[queue] ) ^ 0; // ^ tail get prev then ^ 0
            Tails[queue] = prevGuy;
        }
        else
        {
            // previous is null (The only item) after leave empty
            GroupStartPrevs[queue][gid] = 0;
            GroupStarts[queue][gid] = 0;
            GroupEnds[queue][gid] = 0;
            GroupEndNexts[queue][gid] = 0;
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

        nextGuy = (unsigned int)Heads[queue]->prevXnext ^ 0;  // header prev is 0; ^ 0 to get next
        if (nextGuy)
        {
            // next Guy exists
            if (nextGuy->grpIdx == gid)
            {
                // Same group, group start leaves; update starts
                GroupStarts[queue][gid] = nextGuy;
                GroupStartPrevs[queue][gid] = 0;
            }
            else
            {
                // different group dismissed
                GroupStartPrevs[queue][gid] = 0;
                GroupStarts[queue][gid] = 0;
                GroupEnds[queue][gid] = 0;
                GroupEndNexts[queue][gid] = 0;
            }
            nextGuy->prevXnext = ((unsigned int)nextGuy->prevXnext ^ (unsigned int)Heads[queue] ) ^ 0;  // ^ head firt to get next then ^ 0 turns new head
            Heads[queue] = nextGuy;
        }
        else
        {
            // no next guy; 
            GroupStartPrevs[queue][gid] = 0;
            GroupStarts[queue][gid] = 0;
            GroupEnds[queue][gid] = 0;
            GroupEndNexts[queue][gid] = 0;
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
    DLItem* groupEndNext = GroupEndNexts[queue][group];

    if (groupEnd)
    {
        // The group exist in this queue
        enteredGuy->prevXnext = (unsigned int)groupEnd ^ (unsigned int)groupEndNext;
        groupEnd->prevXnext = ((unsigned int)groupEnd->prevXnext ^ (unsigned int)groupEndNext) ^ (unsigned int)enteredGuy;

        if (groupEndNext)
        {
            groupEndNext->prevXnext = ((unsigned int)groupEndNext->prevXnext ^ (unsigned int)groupEnd) ^ (unsigned int)enteredGuy;
            // successive group startPrev updated!!
            GroupStartPrevs[queue][groupEndNext->grpIdx] = enteredGuy;
        }
        else
            Tails[queue] = enteredGuy;

        // new group end
        GroupEnds[queue][group] = enteredGuy;
        GroupEndNexts[queue][group] = groupEndNext;
    }
    else
    {
        // This is the first guy of a new group enters this queue
        // Enter tail of the queue
        DLItem* tailer = Tails[queue];
        if (tailer)
        {
            // Tail exists
            enteredGuy->prevXnext = (unsigned int)tailer ^ 0;
            tailer->prevXnext = ((unsigned int)tailer->prevXnext ^ 0) ^ (unsigned int)enteredGuy;
            // prior group endNext updated!!
            GroupEndNexts[queue][tailer->grpIdx] = enteredGuy;
            Tails[queue] = enteredGuy;
        }
        else
        {
            // Empty queue
            Heads[queue] = enteredGuy;
            Tails[queue] = enteredGuy;
            enteredGuy->prevXnext = 0;
        }

        GroupStarts[queue][group] = enteredGuy;
        GroupStartPrevs[queue][group] = tailer;

        GroupEnds[queue][group] = enteredGuy;
        GroupEndNexts[queue][group] = 0;
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
    int cnt = 0;
    while (tail)
    {
        int gid = tail->grpIdx; // The current group id
        tail = GroupStartPrevs[queue][gid]; // The next group end for next iteration

        DLItem* end = GroupEnds[queue][gid];
        DLItem* endNext = GroupEndNexts[queue][gid];

        DLItem* jointEnd = GroupEnds[jointQueue][gid];
        DLItem* jointEndNext = GroupEndNexts[jointQueue][gid];

        DLItem* start = GroupStarts[queue][gid];
        if (jointEnd)
        {
            // group exists; combine two segments;
            // connect jointEnd with end
            jointEnd->prevXnext = ((unsigned int)jointEnd->prevXnext ^ (unsigned int)jointEndNext) ^ (unsigned int)end;

            if (end == start)
            {
                // The group just has one item; end is between jointEnd and jointEndNext
                end->prevXnext = (unsigned int)jointEndNext ^ (unsigned int)jointEnd ;
            }
            else
            {
                // more than one item; keep prev as the new next
                end->prevXnext = ((unsigned int)end->prevXnext ^ (unsigned int)endNext) ^ (unsigned int)jointEnd;
            }

            if (jointEndNext)
            {
                // different group, new previous is start
                jointEndNext->prevXnext = ((unsigned int)jointEndNext->prevXnext ^ (unsigned int)jointEnd) ^ (unsigned int)start;
                // Update group start prev
                // update next group startPrev
                GroupStartPrevs[jointQueue][jointEndNext->grpIdx] = start;
            }
            else
                Tails[jointQueue] = start;

            if (end != start)
            {
                start->prevXnext = ((unsigned int)start->prevXnext ^ (unsigned int)GroupStartPrevs[queue][gid]) ^ (unsigned int)jointEndNext;
            }
            // group appended, update end and endNext
            GroupEnds[jointQueue][gid] = start; 
            GroupEndNexts[jointQueue][gid] = jointEndNext;  
        }
        else
        {
            // The group is not in the jointQueue
            // append to tail of jointQueue
            jointEnd = Tails[jointQueue];

            if (jointEnd)
            {
                // Last different group
                jointEnd->prevXnext = ((unsigned int)jointEnd->prevXnext ^ 0) ^ (unsigned int)end;
                GroupEndNexts[jointQueue][jointEnd->grpIdx] = end;

                if (end == start)
                {
                    // The group just has one item; the new next will be 0; end is between jointEnd and 0
                    end->prevXnext = (unsigned int)jointEnd ^ 0;
                }
                else
                {
                    // more than one item; keep prev as the new next
                    end->prevXnext = ((unsigned int)end->prevXnext ^ (unsigned int)endNext) ^ (unsigned int)jointEnd;
                }
                
                Tails[jointQueue] = start;

                if (end != start)
                {
                    start->prevXnext = ((unsigned int)start->prevXnext ^ (unsigned int)GroupStartPrevs[queue][gid]) ^ 0;
                }
            }
            else
            {
                // joint queue is empty
                // Update xor pointer of end and start
                end->prevXnext = ((unsigned int)end->prevXnext ^ (unsigned int)endNext) ^ 0;
                start->prevXnext = ((unsigned int)start->prevXnext ^ (unsigned int)GroupStartPrevs[queue][gid]) ^ 0;

                Heads[jointQueue] = end;
                Tails[jointQueue] = start;
            }
            // group appended, update end and start
            GroupStartPrevs[jointQueue][gid] = jointEnd;
            GroupStarts[jointQueue][gid] = end;
            GroupEnds[jointQueue][gid] = start;
            GroupEndNexts[jointQueue][gid] = 0;
        }
       // debug
        //cnt++;
        //if( cnt == 5) 
        //    PrintQueueInfo();

         //PrintQueueInfo();
    }

    for (int j = 0; j < GroupNumber; j++)
    {
        GroupEnds[queue][j] = GroupStarts[queue][j] = 0;
        GroupEndNexts[queue][j] = GroupStartPrevs[queue][j] = 0;
    }


    Heads[queue] = 0;
    Tails[queue] = 0;
    Closed[queue] = 'y';
}


int main()
{
   //char fileName1[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues001.txt";
   // char fileName2[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    char fileName3[] = "D:\\2022 GitRepos\\2022 DataStructureAlgorithmCourseHWK\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues003.txt";

    char fileName1[] = "D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues001.txt";
    char fileName2[] = "D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    char fileName4[] = "D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues004.txt";
    //char fileName2[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW1_LinkedQueueOrTree\\Samples\\DoubleLinkingQueues002.txt";
    char answer[80];

    // filePtr = fopen(fileName1, "r");
    filePtr = fopen(fileName1, "r");
    // filePtr = fopen(fileName3, "r");

    fscanf(filePtr, "%d %d %d", &QueueNumber, &EventNumber, &GroupNumber);
    // Allocate memory for these queues
    Heads = malloc(sizeof(DLItem*) * QueueNumber);
    Tails = malloc(sizeof(DLItem*) * QueueNumber);
    GroupEnds = malloc(sizeof(DLItem**) * QueueNumber); // used in dismiss function
    GroupStarts = malloc(sizeof(DLItem**) * QueueNumber); // used in dismiss function
    GroupEndNexts = malloc(sizeof(DLItem**) * QueueNumber); // used in dismiss function
    GroupStartPrevs = malloc(sizeof(DLItem**) * QueueNumber); // used in dismiss function

    Closed = malloc(sizeof(char) * 4);

    for (int i = 0; i < QueueNumber; i++)
    {
        Closed[i] = 'n';
        Heads[i] = 0;
        Tails[i] = 0;
        GroupStarts[i] = malloc(sizeof(DLItem*) * GroupNumber);
        GroupEnds[i] = malloc(sizeof(DLItem*) * GroupNumber);
        GroupStartPrevs[i] = malloc(sizeof(DLItem*) * GroupNumber);
        GroupEndNexts[i] = malloc(sizeof(DLItem*) * GroupNumber);
        for (int j = 0; j < GroupNumber; j++)
        {
            GroupStarts[i][j] = 0;
            GroupEnds[i][j] = 0;
            GroupStartPrevs[i][j] = 0;
            GroupEndNexts[i][j] = 0;
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
    DLItem* ptr, *last;
    for (int q = 0; q < QueueNumber; q++)
    {
        last = 0;
        ptr = Heads[q];
        while (ptr)
        {
            printf("%d ", ptr->id);
            DLItem* next =  (unsigned int)ptr->prevXnext ^ (unsigned int)last;
            last = ptr;
            ptr = next;
        }
        printf("\n");
    }

    scanf("%d");
}
