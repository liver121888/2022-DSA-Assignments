//#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

int IsAGreaterThanB(unsigned char a[], int* alen, unsigned char b[], int* blen)
{
    int i =0, j=0;
    while (a[i] != 0) i++;
    while (b[j] != 0) j++;
    int aGreaterThanB = 1;
    if (i < j) aGreaterThanB = 0;
    else if (i == j)
    {
        int k = 0;  
        if (a[k] == b[k] && k < alen ) { k++; }
        if (k < i && a[k] < b[k]) aGreaterThanB = 0;
    }
    *alen = i; *blen = j;
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

const unsigned char* BigIntsSubtraction(unsigned char a[], unsigned char b[], int* clen) 
{
    int aLen, bLen;
    unsigned char *c=0;
    int k = 0;
    unsigned char* big, * small;
    int i; // bigger length and index
    int j; // smaller length and index

    if ( IsAGreaterThanB(a,&aLen, b, &bLen))
    {
        big = a; small = b; i = aLen; j = bLen;
    }
    else
    {
        big = b; small = a; i = bLen; j = aLen;
    }
    if (big[0] == small[0] && i == j) k = i - 1; // digits of c will be lesser than bigger one by 1
    else k = i; // digits of c is the same with the bigger

    *clen = k;

    c = malloc(sizeof( unsigned char ) * (k + 1)); // allocate memory for c with an extra char for end of string
    c[k] = 0; // End of the string

    k--; i--; j--; // now all of them are indexes
    int sub = 0; // extra subtraction, initially zero
    while (i >= 0) // loop through from last index to zero 
    {
        if (j < 0) // no digit existing in smaller
            c[k] = big[i]; // no subtraction is required
        else
        {
            int net = big[i] - sub - small[j];
            if (net >= 0)
            {
                c[k] = '0'+net; sub = 0; // no extra subtraction for higher digit
            }
            else 
            {
                c[k] = '0'+( 10 + net); sub = 1; 
            }
        }
        i--; j--; k--;
    }
    
    return c;
}


unsigned char* BigIntsMultiplication(unsigned char a[], int k) 
{


    return NULL;
}

unsigned char* BigIntsDivisionBy2(unsigned char a[], int aLen, int* cLen ) 
{
    unsigned char* c;
    int k = 0; // length and index of c
 
    if (a[0] - '0' > 1) k = aLen;
    else k = aLen - 1;
    *cLen = k;

    c = malloc( (k+1) * sizeof(unsigned char));
    c[k] = 0;
    int previous = 0;
    k = 0;
    for (int i = 0; i < aLen; i++)
    {
        int v = (a[i] - '0')+previous*10;
        while (v < 2 && i < aLen)
        {
            i++;
            v = v * 10 + a[i];
        }
        c[k] = '0'+ v / 2;
        previous = v % 2;
        k++;
    }
    return c;
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

    int subLen;
    char* sub =  BigIntsSubtraction(buf1, buf2, &subLen );
    printf("a - b = %s\n", sub);
    
    int halfLen;
    char* half;
    if ((sub[subLen - 1] - '0') % 2 == 0)
    {
        half = BigIntsDivisionBy2(sub, subLen, &halfLen);
        printf("sub / 2 = %s", half);
    }

    int i;
    scanf("%d", &i);
}
