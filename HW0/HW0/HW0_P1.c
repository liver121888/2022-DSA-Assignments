//#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

int IsAGreaterThanB(unsigned char a[], unsigned char b[])
{
    int alen=0, blen=0;
    while (a[alen] != 0) alen++;
    while (b[blen] != 0) blen++;
    int aGreaterThanB = 1;
    if (alen < blen) aGreaterThanB = 0;
    else if (alen == blen)
    {
        int i = 0;  
        if (a[i] == b[i] && i < alen ) { i++; }
        if (i < alen && a[i] < b[i]) aGreaterThanB = 0;
    }
    return aGreaterThanB;
}

int IsAGreaterThanBPointerWay(unsigned char a[], unsigned char b[])
{
    char *aPtr = a, *bPtr = b;
    int aLen = 0, bLen = 0;
    while (*aPtr++ != 0) aLen++;
    while (*bPtr++ != 0) bLen++;
    int aGreaterThanB = 1;
    if (aLen < bLen) aGreaterThanB = 0;
    else if (aLen == bLen)
    {
        aPtr = a; bPtr = b;
        while (*aPtr++ == *bPtr++ && *aPtr != 0);
        if (*aPtr < *bPtr) aGreaterThanB = 0;
    }
    return aGreaterThanB;
}

const unsigned char* BigIntsSubtraction(unsigned char a[], unsigned char b[]) 
{
    unsigned char *c=0;

    unsigned char* big, * small;
    big = a; small = b;
    if (!IsAGreaterThanB(a, b))
    {
        big = b; small = a;
    }
    int bigLen = 0, smallLen = 0, cLen = 0;
    while (big[bigLen] != 0)bigLen++;
    while (small[smallLen] != 0)smallLen++;
    if (big[0] == small[0] && bigLen == smallLen) cLen = bigLen - 1;
    else cLen = bigLen - 1;
    c = malloc(sizeof( unsigned char ) * (cLen + 1));
 
    c[cLen] = 0; // End of the string
    cLen--; bigLen--; smallLen--;
    int sub = 0; // extra subtraction
    while (bigLen >= 0)
    {
        if (smallLen < 0)
            c[cLen] = big[bigLen];
        else
        {
            int net = big[bigLen] - sub - small[smallLen];
            if (net > 0)
            {
                c[cLen] = '0'+net; sub = 0;
            }
            else 
            {
                c[cLen] = '0'+( 10 + net); sub = 1;
            }
        }
        bigLen--;
        smallLen--;
        cLen--;
    }
    return c;
}


unsigned char* BigIntsMultiplication(unsigned char a[], int k) 
{


    return NULL;
}

unsigned char* BigIntsDivision(unsigned char a[], int k) 
{


    return NULL;
}


int main()
{
    char buf1[256], buf2[256];
    FILE* ptr;
  // ptr = fopen("D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW0\\HW0\\x64\\Debug\\test.txt", "r");
    char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\test.txt";
    
     ptr = fopen(fileName, "r");
    fscanf(ptr, "%s", buf1 );
    fscanf(ptr, "%s", buf2);
 
    printf("a = %s\n", buf1);
    printf("b = %s\n", buf2);
    fclose(ptr);

    char* res =  BigIntsSubtraction(buf1, buf2);

    printf("a - b = %s\n", res);

    int i;
    scanf("%d", &i);
}
