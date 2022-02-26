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
        // equal or smaller
        int k = 0;  
        if (a[k] == b[k] && k < alen ) { k++; }
        if (k < i)
        {
            if (a[k] < b[k]) aGreaterThanB = -1;
        }
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
    //if (a[0] == '1' && a[1] == '7' && b[0] == '4' && b[1] == '9')
    //{
    //    int stop = 1;
    //}
    //if (a[0] == 0 || b[0] == 0)
    //{
    //    int stop = 1;
    //}
    int same = IsAGreaterThanBAndGetLengths(a, &aLen, b, &bLen);
    //if (a[0] == 0 || b[0] == 0)
    //{
    //    int stop = 1;
    //}
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
        // Swqp 
        big = b; small = a; i = bLen; j = aLen;
    }

    // big and small never the same from here
    //if (i == j)
    //{
    //    k = i;  // digits of c will be lesser than or equal to bigger one
    //    int p = 0;
    //    while (big[p] == small[p] && p < i)
    //    {
    //        k--;
    //        p++;
    //    }
    //}
    //else k = i;// digits of c is the same with the bigger
   // *clen = k;

    k = i;
    c = malloc(sizeof( unsigned char ) * (k + 1)); // allocate memory for c with an extra char for end of string
    for (int z = 0; z < k + 1; z++) c[z] = 0;

    c[k] = 0; // End of the string

    i--; j--; // now all of them are indexes
    int sub = 0; // extra subtraction, initially zero

    //if (big[0] == 0 || small[0] == 0)
    //{
    //    int stop = 2;
    //}
    while (i >= 0) // loop through from last index to zero 
    {
 
        int net = big[i]-'0' - sub - (j >= 0 ? small[j]-'0' : 0);
        if (net >= 0)
        {
            c[i] = '0' + net; sub = 0; // no extra subtraction for higher digit
        }
        else
        {
            c[i] = '0' + (10 + net); sub = 1;
        }
        i--; j--;  
    }
    j = 0;
    for( int ii = 0; ii < k; ii++)
        if (c[ii] != '0') { j = ii; break; }

    int zeroCount = j;

    i = 0;
    if (j != 0)
    {
        for ( int p = j; p <= k; p++)
            c[i++] = c[p];
        k = i-1;
    }


    int p;
    for ( p = 0; ; p++)  if (c[p] == 0) break;
    if (p != k)
        printf("\n error!! \n");

    *clen = k;
    return c;
}


unsigned char* BigIntsMultiplication(unsigned char a[], int aLen, int p, int* cLen) 
{
    if (p == 0)
    {
        *cLen = aLen;
        return a;
    }
    unsigned char* c;
    
    unsigned char values[512]; // In-place update
    int lastLen=-1, newLen=-1;
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
        if (v < 2 && i == 0)
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

unsigned char* GetGCD( unsigned char buf1[], unsigned char buf2[])
{
    char* big, * small;
    int count = 0;
    int len1, len2, bigLen, smallLen;
    if (IsAGreaterThanBAndGetLengths(buf1, &len1, buf2, &len2) > 0)
    {
        big = buf1; bigLen = len1, small = buf2, smallLen = len2;
    }
    else
    {
        big = buf2; bigLen = len2, small = buf1, smallLen = len1;
    }
    while (big[0] != '0' && small[0] != '0')
    {

        if ((big[bigLen - 1] - '0') % 2 == 0 && (small[smallLen - 1] - '0') % 2 == 0) count++;
         int kkk = 0;
        if ((small[smallLen - 1] - '0') % 2 == 0)
        {
            kkk = 1;
            small = BigIntsDivisionBy2(small, smallLen, &smallLen);
        }
        if ((big[bigLen - 1] - '0') % 2 == 0)
        {
            kkk = 2;
            big = BigIntsDivisionBy2(big, bigLen, &bigLen);
        }
        if (IsAGreaterThanBAndGetLengths(big, &bigLen, small, &smallLen) < 0)
        {
            unsigned char* temp = big;
            big = small;
            small = temp;
            int len = bigLen;
            bigLen = smallLen;
            smallLen = len;
        }
        big = BigIntsSubtraction(big, small, &bigLen);
    }
    small = BigIntsMultiplication(small, smallLen, count, &smallLen);
    return small;
}


int oldmain()
{
    unsigned char aa[] = "42";
    unsigned char bb[] = "66";
    unsigned char* cc;
    cc = GetGCD(aa, bb);



    char buf1[260], buf2[260];
    FILE* fPtr;
  // ptr = fopen("D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW0\\HW0\\x64\\Debug\\test.txt", "r");
    char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\test.txt";
    
     fPtr = fopen(fileName, "r");

     while (fscanf(fPtr, "%s %s", buf1, buf2) != EOF)
     {


         printf("a = %s \n", buf1);
         printf("b = %s\n", buf2);

         int subLen;
         unsigned char* sub = BigIntsSubtraction(buf1, buf2, &subLen);
         printf("a - b = %s    len = %d\n", sub, subLen);

         int halfLen;
         unsigned char* half;
         if ((sub[subLen - 1] - '0') % 2 == 0)
         {
             half = BigIntsDivisionBy2(sub, subLen, &halfLen);
             printf("(a-b) / 2 = %s   len = %d\n\n", half, halfLen );
         }

         int powLen;
         unsigned char* pow;
         pow = BigIntsMultiplication(sub, subLen, 2, &powLen);
         printf("(a-b)**2 = %s   len = %d\n", pow, powLen);

         pow = BigIntsMultiplication(sub, subLen, 3, &powLen);
         printf("(a-b)**3 = %s   len = %d\n\n", pow, powLen);


     }

    fclose(fPtr);






    for (int b = 0; b < 50; b++)
    {

        printf("\n\n*************** problem %d.int ***************\n", b);
        char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\85.in";
        sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\%d.in", b);

        FILE* fptr = fopen(fileName, "r");

        fscanf(fPtr, "%s %s", buf1, buf2);
        fclose(fPtr);

        printf("a = %s\nb = %s\n", buf1, buf2 );

        char* gcd;
        gcd = GetGCD(buf1, buf2);

        printf("Computed GCD = %s\n", gcd);

        sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\%d.out", b);
        fptr = fopen(fileName, "r");
        fscanf(fPtr, "%s", buf1 );
        fclose(fPtr);

        printf("Answer = %s\n\n", buf1);


    }
    int i;
    scanf("%d", &i);
}

void f()
{
    int i, j;
    i = 33;
}

void g()
{
    int x, y;
    y = 44;
}

void h()
{
    int xx, yy;
}

// Double values of the target BigInt repeatTimes
void DoubleVlueMultipleTimes(char* target, int* targetLenAdd, int repeatTimes )
{

}


// First value subtract second value and update first value with the residual 
void FirstSubstractSecondUpdateFirst(char* first, int* firstLenAdd, char* second, int secondLen)
{

}

// Set value to half of the target BigInt
void HalfTheBitInt(char* target, int* targetLenAdd )
{

}

// Check whether the first value is larger than the second, return 1
// If equal return 0;
// If first smaller than the second, return -1.
int IsFirstLargerThanSecond(char* first, int firstLen, char* second, int secondLen)
{
    if (firstLen > secondLen) return 1;
    else if (secondLen > firstLen) return -1;

    return 1;
}

int main()
{
    FILE* fPtr;

    char a[260], b[260];
    char* large, * small, * temp;
    int alen, blen, largeLen, smallLen;

    for (int z = 0; z < 50; z++)
    {
        printf("\n\n*************** problem %d.int ***************\n", z);
        char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\85.in";
        sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\%d.in", z);
        fPtr = fopen(fileName, "r");
        fscanf(fPtr, "%s %s", a, b);
        fclose(fPtr);

        // calculated alen and blen
        alen = 0;
        while (a[alen] != '0') alen++;
        blen = 0;
        while (b[blen] != '0') blen++;

        // convert chars to values for all functions to directly access values, not characters       
        for (int i = 0; i < alen; i++)
            a[i] -= '0';
        for (int i = 0; i < blen; i++)
            b[i] -= '0';

        if (IsFirstLargerThanSecond(a, alen, b, blen) > 0)
        {
            large = a; largeLen = alen; small = b; smallLen = blen;
        }
        else
        {
            large = b; largeLen = blen; small = a; smallLen = alen;
        }
        int halfCount = 0;



        while (large[0] != 0 && small[0] != 0)
        {
            int isLargeEven = large[largeLen - 1] % 2 == 0;
            int isSmallEven = small[smallLen - 1] % 2 == 0;
            if (isLargeEven && isSmallEven) halfCount++;
            if (isLargeEven) HalfTheBitInt(large, &largeLen);
            if (isSmallEven) HalfTheBitInt(small, &smallLen);
            if (!IsFirstLargerThanSecond(large, largeLen, small, smallLen))
            {
                // Swap large and small
                temp = large;
                large = small;
                small = large;
                int t = largeLen;
                largeLen = smallLen;
                smallLen = t;
            }
            FirstSubstractSecondUpdateFirst(large, &largeLen, small, smallLen);
        }
        DoubleVlueMultipleTimes(small, &smallLen, halfCount);

        // GCD is store in small, convert back to chars
        for (int i = 0; i < smallLen; i++) small[i] += '0';
        small[smallLen] = 0;
        printf("GCD = %s \n", small);

        sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\%d.out", z);
        fPtr = fopen(fileName, "r");
        fscanf(fPtr, "%s", a );
        fclose(fPtr);
        printf("Answer = %s\n\n", a);
    } 
    int iii;
    scanf("%d", &iii);
}
