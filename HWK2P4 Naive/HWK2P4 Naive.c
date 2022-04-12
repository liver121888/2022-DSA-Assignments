
#define _CRT_SECURE_NO_DEPRECATE


#include<stdio.h>
#include<stdlib.h>
#include<math.h>

char answer[80];
FILE* filePtr;

int N, Q, S, R;

int  method1(int start)
{
    return 1;
}

void main()
{
    char fileName1[] = "..\\HWK2 Samples\\p4sample1.txt";
    char fileName2[] = "..\\HWK2 Samples\\p4sample2.txt";
    char fileName3[] = "..\\HWK2 Samples\\p4sample3.txt";


    char answer[80];

    filePtr = fopen(fileName1, "r");
    //  filePtr = fopen(fileName2, "r");
    //     filePtr = fopen(fileName3, "r");
    // 
       // N: number of cities Q: number of queries
   // S: capital  R: resort
    fscanf(filePtr, "%d %d %d %d", &N, &Q, &S, &R);
    int from, to, start;

    for (int i = 0; i < N - 1; i++)
    {
        fscanf(filePtr,"%d %d", &from, &to);
    }
    for (int i = 0; i < Q; i++)
    {
        fscanf(filePtr,"%d", &start);
        int ans = method1(start);
        printf("Query%d answer = %d\n", start, ans);

    }

    printf("Done! Check answer ...\n");

    fscanf(filePtr, "%s", answer);
    printf("%s\n", answer);
    for (int j = 0; j < Q; j++)
    {
        fscanf(filePtr, "%s", answer);
        printf("%s\n", answer);
    }
    fclose(filePtr);

}