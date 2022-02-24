//#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>


// return 0 if a = b, 1 for a > b, -1 for a < b
int IsAGreaterThanBAndGetLengths(unsigned char a[], int* alen, unsigned char b[], int* blen)
{
    int i =0, j=0;
    while (a[i] != 0) i++;
    while (b[j] != 0) j++;
    *alen = i; *blen = j;
    int aGreaterThanB = 1;
    if (i < j) aGreaterThanB = -1;
    else if (i == j)
    {
        int k = 0;  
        if (a[k] == b[k] && k < alen ) { k++; }
        if (k < i && a[k] < b[k]) aGreaterThanB = -1;
        else aGreaterThanB = 0;
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

const unsigned char* BigIntsSubtraction(unsigned char a[], unsigned char b[], int* clen) 
{
    int aLen, bLen;
    unsigned char *c=0;
    int k = 0;
    unsigned char* big, * small;
    int i; // bigger length and index
    int j; // smaller length and index
    int same = IsAGreaterThanBAndGetLengths(a, &aLen, b, &bLen);
    if (same == 0)
    {
        *clen = 1;
        c = malloc(2);
        c[1] = 0;
        c[0] = '0';
        return c;
    }
    else if ( same > 0 )
    {
        big = a; small = b; i = aLen; j = bLen;
    }
    else
    {
        big = b; small = a; i = bLen; j = aLen;
    }

    // big and small never the same from here
    if (i == j)
    {
        k = i;  // digits of c will be lesser than or equal to bigger one
        int p = 0;
        while (big[p] == small[p] && p < i)
        {
            k--;
            p++;
        }
    }
    else k = i;// digits of c is the same with the bigger
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


unsigned char* BigIntsMultiplication(unsigned char a[], int aLen, int p, int* cLen) 
{
    unsigned char* c;
    
    unsigned char values[512]; // In-place update
    int lastLen, newLen;
    // copy 
    int i, k = 0; // length and index of c
    for (k = 0; k < aLen; k++) values[k] = a[k] - '0';
    lastLen = aLen;
  
    int count = 0;
    int add = 0;
    while (count < p)
    {
        if (values[0] >= 5) newLen = lastLen + 1;
        else newLen = lastLen;
        add = 0;
        k = newLen-1;
        for (int i = lastLen - 1; i >= 0; i--)
        {
            int v = values[i] * 2 + add;
            if (v >= 10)
            {
                values[k] = v - 10;
                add = 1;
                if (i == 0) values[k - 1] = 1;
            }
            else
            {
                values[k] = v;
                add = 0;
            }
            k--;
        }
        lastLen = newLen;
        count++;
    }

    *cLen = newLen;
    c = malloc(newLen + 1);
    for (int i = 0; i < newLen; i++)
        c[i] = values[i] + '0';
    c[newLen] = 0;

    return c;
}

unsigned char* BigIntsDivisionBy2(unsigned char a[], int aLen, int* cLen ) 
{
    unsigned char* c;
    int k = 0; // length and index of c
 
    if (aLen == 1)
    {
        k = 1;
        *cLen = k;
        c = malloc(k + 1);
        c[k] = 0;
        c[0] = (a[0]-'0' ) / 2 + '0';
        return c;
    }

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
            v = v * 10 + ( a[i]-'0');
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
    FILE* fPtr;
  // ptr = fopen("D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW0\\HW0\\x64\\Debug\\test.txt", "r");
    char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\test.txt";
    
     fPtr = fopen(fileName, "r");

     while (fscanf(fPtr, "%s %s", buf1, buf2) != EOF)
     {


         printf("a = %s\n", buf1);
         printf("b = %s\n", buf2);

         int subLen;
         unsigned char* sub = BigIntsSubtraction(buf1, buf2, &subLen);
         printf("a - b = %s\n", sub);

         int halfLen;
         unsigned char* half;
         if ((sub[subLen - 1] - '0') % 2 == 0)
         {
             half = BigIntsDivisionBy2(sub, subLen, &halfLen);
             printf("(a-b) / 2 = %s\n\n", half);
         }

         int powLen;
         unsigned char* pow;
         pow = BigIntsMultiplication(sub, subLen, 2, &powLen);
         printf("(a-b)**2 = %s\n", pow);

         pow = BigIntsMultiplication(sub, subLen, 3, &powLen);
         printf("(a-b)**3 = %s\n\n", pow);


     }

    fclose(fPtr);
    int i;
    scanf("%d", &i);
}
