//#include <iostream>
#include<stdio.h>
int main()
{
    printf("Hello HWK0\n");
    int row = 5;
    int col = 6;
    int* p = malloc(row * col * sizeof(int));
    int* q = p;
    for (int i = 0; i < row * col; i++)
        *q++ = i;
    for (int i = 0; i < row * col; i++)
        printf("%d=%d\n", i, p[i]);

    printf("pointer p = %d\n", p);
    printf("pointer p+1 = %d\n", p + 1);

    q = p;
    int** z = malloc(row * sizeof(int*));
    for (int r = 0; r < row; r++)
    {
        z[r] = q;
        q += col;
    }

    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            printf("(%d,%d) = %d\n", r, c, z[r][c]);
        }
    }

    int ary[3][10];
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 10; c++)
            ary[r][c] = r * 10 + c;

    printf("ary = %d\n", ary);
    printf("ary[0] = %d\n", ary[0]);
    printf("ary[1]  = %d\n", ary[1]);
    printf("ary[2] = %d\n", ary[2]);

    printf("ary = %d\n", ary);
    printf("ary+1 = %d\n", ary + 1);
    printf("ary+2  = %d\n", ary + 2);
    printf("ary+3  = %d\n", ary + 3);

    printf("(ary+1)+1 = %d\n", (ary + 1) + 1);
    printf("(ary+1)[1] = %d\n", (ary + 1)[1]);
    printf("ary[1][1] = %d\n", ary[1][1]);
    printf("((ary+1)[1])[1] = %d\n", ((ary + 1)[1])[1]);

    printf("(ary+2)+1  = %d\n", (ary + 2) + 1);
    printf("(ary+2)[1] = %d\n", (ary + 2)[1]);
    printf("ary[2][1] = %d\n", ary[2][1]);
    printf("((ary+2)[1])[1] = %d\n", (ary + 2)[1][1]);



    void* ptr = malloc(40);
    int* ip = ptr;
    for (int i = 0; i < 10; i++)
        ip[i] = i + 5555555555;
    for (int i = 0; i < 10; i++)
        printf("ip[%d] = %d\n", i, ip[i]);

    short* sp = ptr;
    for (int i = 0; i < 20; i++)
        printf("sp[%d] = %d\n", i, sp[i]);

    char* cp = ptr;
    for (int i = 0; i < 40; i++)
        printf("cp[%d] = %c\n", i, cp[i]);

    float* fp = ptr;
    for (int i = 0; i < 10; i++)
        printf("fp[%d] = %f\n", i, fp[i]);

    double* dp = ptr;
    for (int i = 0; i < 5; i++)
        printf("dp[%d] = %f\n", i, dp[i]);


}