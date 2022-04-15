
#define _CRT_SECURE_NO_DEPRECATE


#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct  arc
{
    int first;
    int second;
} Arc;

//typedef struct treeNode
//{
//    int value;
//    int numberOfChildren;
//    struct treeNode** children;
//} TreeNode;

char answer[80];
FILE* filePtr;
Arc* arcs;
int* eulerArray, * depthArray;
int arrayIndex;
int N, Q, S, R, Nm1;
int level = 0;

void printArray()
{
    printf("\narray = " );
    for (int i = 0; i < (2 * N -1); i++)
        printf("%d ", eulerArray[i]);
    printf("\n" );
    printf("depth = ");
    for (int i = 0; i < (2 * N - 1); i++)
        printf("%d ", depthArray[i]);
    printf("\n");
}

int LowestCommonAncester(int a, int b)
{
    if (a == b) return a;
    int start = -1, startTarget = -1, end = -1, endTarget = -1;
    int i, j;
    for( i = 0, j =  2 * N - 2; i<=j; i++, j-- )
    {
            if (eulerArray[i] == a || eulerArray[i] == b)
            {
                if (start < 0)
                {
                    start = i;
                    if (eulerArray[i] == a)
                    {
                        startTarget = a;
                        endTarget = b;

                    }
                    else
                    {
                        startTarget = b;
                        endTarget = a;
                    }
                    while (end < 0)
                    {
                        if (eulerArray[j] == endTarget)
                        {
                            end = j;
                        }
                        j--;
                    }

                    break;
                }
            }
            if (eulerArray[j] == b || eulerArray[j] == a)
            {
                if (end < 0)
                {
                    end = j;
                    if (eulerArray[j] == b)
                    {
                        endTarget = b;
                        startTarget = a;
                    }
                    else
                    {
                       endTarget = a;
                        startTarget = b;
                    }
                    while (start < 0)
                    {
                        if (eulerArray[i] == startTarget)
                        {
                            start = i;
                        }
                        i++;
                    }
                }
            }
    }

    int endUpdate = 0;
    for ( int k = i; k< j; k++ )
    {
        if (eulerArray[k] == startTarget)
            start = k;
        else if (endUpdate == 0 && eulerArray[k] == endTarget)
        {
            end = k;
            endUpdate = 1;
        }

    }

    int minDepth = N;
    int minID = -1;

    for (int k = start; k <= end; k++)
    {
        if (depthArray[k] < minDepth)
        {
            minDepth = depthArray[k];
            minID = k;
        }
    }
    return eulerArray[minID];
}


// Lowest Common Ancestor for two nodes
void TraverseTreeToConstructArray(int parent, int target)
{
    depthArray[arrayIndex] = level;
    eulerArray[arrayIndex++] = target;
    // loop through all arcs; if time longer than judge acceptance, build a map 
    for (int a = 0; a < Nm1; a++)
    {
        if (arcs[a].first == target && arcs[a].second != parent)
        {
            level++;
            TraverseTreeToConstructArray(target, arcs[a].second);
            level--;
        }
        else if (arcs[a].second == target && arcs[a].first != parent)
        {
            level++;
            TraverseTreeToConstructArray(target, arcs[a].first);
            level--;
        }
    }
    // add backward record
    depthArray[arrayIndex ] = level-1;
    eulerArray[arrayIndex++] = parent;

}


int method1(int start)
{
    arrayIndex = 0;
    level = 0;
    TraverseTreeToConstructArray(0, start);
    printf("Start is %d => ", start);
    printArray();

   return LowestCommonAncester(S, R);
 
}


void main()
{
    char fileName1[] = "..\\HWK2 Samples\\p4sample1.txt";
    char fileName2[] = "..\\HWK2 Samples\\p4sample2.txt";
    char fileName3[] = "..\\HWK2 Samples\\p4sample3.txt";


    char answer[80];

    // filePtr = fopen(fileName1, "r");
    //  filePtr = fopen(fileName2, "r");
    //     filePtr = fopen(fileName3, "r");
   
    for (int benchmark = 1; benchmark < 4; benchmark++)
    {
        sprintf( answer, "..\\HWK2 Samples\\p4sample%d.txt", benchmark  );
        filePtr = fopen(answer, "r");

        printf("\nSolving Benchmark%d =>\n", benchmark);

        // N: number of cities Q: number of queries
        // S: capital  R: resort
        fscanf(filePtr, "%d %d %d %d", &N, &Q, &S, &R);

        Nm1 = N - 1;
        arcs = malloc(Nm1 * sizeof(Arc));
        eulerArray = malloc((N * 2 - 1) * sizeof(int));
        depthArray = malloc((N * 2 - 1) * sizeof(int));

        int from, to, start;

        for (int i = 0; i < N - 1; i++)
        {
            fscanf(filePtr, "%d %d", &from, &to);
            //arcs[i] = malloc(sizeof(Arc));
            arcs[i].first = from;
            arcs[i].second = to;
        }
        for (int i = 0; i < Q; i++)
        {
            fscanf(filePtr, "%d", &start);
            printf("\n");

            int ans = method1(start);

            printf("Query%d answer = %d\n", i+1, ans);
        }

        printf("\nDone! Check answer ...\n");

        fscanf(filePtr, "%s", answer);
        printf("%s\n", answer);
        for (int j = 0; j < Q; j++)
        {
            fscanf(filePtr, "%s", answer);
            printf("%s\n", answer);
        }
        fclose(filePtr);

    }

}