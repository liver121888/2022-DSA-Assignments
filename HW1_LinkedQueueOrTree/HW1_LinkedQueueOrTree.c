
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
DLItem** QueueHeads;
DLItem** QueueTails;


void TailLeave(int queue)
{
    if (QueueTails[queue])
    {
        QueueTails[queue] = QueueTails[queue]->prev;
        QueueTails[queue]->next = 0;
    }
}

void GoDequeu(int queue)
{
    if (QueueHeads[queue])
    {
        QueueHeads[queue] = QueueHeads[queue]->next;
        QueueHeads[queue]->prev = 0;
    }
}

void Enter(int group, int id, int queue)
{
    DLItem* ptr = QueueTails[queue];
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
            if (ptr == QueueTails[queue])QueueTails[queue] = newOne;
            return;
        }
        ptr = ptr->prev;
    }
    // Added to tail
    DLItem* newOne = malloc(sizeof(DLItem));
    newOne->id = id;
    newOne->grpIdx = group;
    if (QueueTails[queue])
    {
        QueueTails[queue]->next = newOne;
        newOne->prev = QueueTails[queue];
        newOne->next = 0;
        QueueTails[queue] = newOne;
    }
    else
    {
        newOne->next = 0;
        newOne->prev = 0;
        QueueHeads[queue] = newOne;
        QueueTails[queue] = newOne;
    }
}

void QueueDismiss(int queue)
{
    int newQueue;
    do
    {
        newQueue = queue - 1;
        if (newQueue < 0) newQueue += QueueNumber;
    } while (Closed[newQueue] == 'y');

    DLItem* target, * frontOne;
    target = QueueTails[queue];
    while (target)
    {
        frontOne = target->prev;
        Enter(target->grpIdx, target->id, newQueue);
        target = frontOne;
    }
    QueueHeads[queue] = 0;
    QueueTails[queue] = 0;
    Closed[queue] = 'y';
}

void PrintQueueInfo()
{
    DLItem* ptr;
    for (int q = 0; q < QueueNumber; q++)
    {
        printf("\nQ%d: ", q);
        ptr = QueueHeads[q];
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
    char answer[80];

    filePtr = fopen(fileName1, "r");
    // filePtr = fopen(fileName2, "r");

    fscanf(filePtr, "%d %d %d", &QueueNumber, &EventNumber, &GroupNumber);
    // Allocate memory for these queues
    QueueHeads = malloc(sizeof(DLItem*) * QueueNumber);
    QueueTails = malloc(sizeof(DLItem*) * QueueNumber);
    Closed = malloc(sizeof(char) * 4);
    for (int i = 0; i < QueueNumber; i++)
    {
        Closed[i] = 'n';
        QueueHeads[i] = 0;
        QueueTails[i] = 0;
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
            Enter(groupIdx, ID, queueIdx);
            printf("\n\t=> %d(%d) enters Q%d ", ID, groupIdx, queueIdx);
            break;
        case 'l': // leave
            fscanf(filePtr, "%d", &queueIdx);
            TailLeave(queueIdx);
            printf("\n\t=> Q%d tail leaves ", queueIdx);
            break;
        case 'g': // go
            fscanf(filePtr, "%d", &queueIdx);
            GoDequeu(queueIdx);
            printf("\n\t=> Q%d head leaves ", queueIdx);
            break;
        case 'c': // close
            fscanf(filePtr, "%d", &queueIdx);
            QueueDismiss(queueIdx);
            printf("\n\t=> Q%d closes ", queueIdx);
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
        ptr = QueueHeads[q];
        while (ptr)
        {
            printf("%d ", ptr->id);
            ptr = ptr->next;
        }
        printf("\n");
    }

    scanf("%d");
}
