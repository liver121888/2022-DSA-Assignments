//#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>



// Double values of the target BigInt repeatTimes
void DoubleVlueMultipleTimes(char* target, int* targetLenAdd, int repeatTimes )
{
    int count = 0;
    int add = 0;
    int  j=-1;
    int len = *targetLenAdd;
    while (count < repeatTimes)
    {
        if (target[0] >= 5) j = len; // One digit is added; Start index extended by 1
        else j = len - 1; // Same index 
        *targetLenAdd = j + 1; // Keep new length for this iteration
        add = 0;
        for (int i = len - 1; i >= 0; i--)
        {
            int v = target[i] * 2 + add;
            if (v >= 10)
            {
                target[j] = v - 10;
                add = 1;
                if (i == 0) target[j - 1] = 1; // extended (j-1) should be exactly 0
            }
            else
            {
                target[j] = v;
                add = 0;
            }
            j--;
        }
        count++;
        len = *targetLenAdd; // Update new length for next multiplication
    }
    //if (*targetLenAdd <= 0)
    //{
    //    int stopit = 0;
    //}
}


// First value subtract second value and update first value with the residual 
void FirstSubstractSecondUpdateFirst(char* first, int* firstLenAdd, char* second, int secondLen)
{

    int len = *firstLenAdd;
    int j;
    j = secondLen - 1;
    int sub = 0; // extra subtraction, initially zero

    for (int i = len - 1; i >= 0; i--)
    {
        int low = j >= 0 ? second[j] : 0;
        int net = first[i] - sub - low;
        if (net >= 0)
        {
            first[i] =  net; 
            sub = 0; // no extra subtraction for higher digit
        }
        else
        {
            first[i] =  10 + net; 
            sub = 1;
        }
        j--;
    }
    j = 0;
    for (int i = 0; i < len; i++)
        if (first[i] == 0) j++;
        else break;

    if (j == len) // all zero, set length 1 return;
    {
        *firstLenAdd = 1;
    }
    else if (j != 0) // same length, no new set
    {
        *firstLenAdd = len - j;
        // Move non-zeros forward
        for (int i = 0; i < len - j; i++)
            first[i] = first[i + j];
    }

    //if (*firstLenAdd <= 0)
    //{
    //    int stopit = 0;
    //}
}

// Set value to half of the target BigInt
void HalfTheBitInt(char* target, int* targetLenAdd )
{
    int len = *targetLenAdd;
    int previous = 0;
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        int v = target[i] + previous * 10;
        if (v < 2 && i == 0 && len > 1)
        {
            i++;
            v = v * 10 + target[i];
        }
        target[j] = v / 2;
        previous = v % 2;
        j++;
    }
    *targetLenAdd = j;
    //if (*targetLenAdd <= 0)
    //{
    //    int stopit = 0;
    //}
}

// Check whether the first value is larger than the second, return 1
// If equal return 0;
// If first smaller than the second, return -1.
int IsFirstLargerThanSecond(char* first, int firstLen, char* second, int secondLen)
{
    if (firstLen > secondLen) return 1;
    else if (secondLen > firstLen) return -1;
    for (int i = 0; i < firstLen; i++)
        if (first[i] > second[i]) return 1;
        else if (first[i] < second[i]) return - 1;
    return 0;
}

int main()
{
    FILE* fPtr;

    char a[260], b[260];
    char* large, * small, * temp;
    int alen, blen, largeLen, smallLen;


    // ***** DEBUG Test on own boundary conditions ********************
    char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\test.txt";

    fPtr = fopen(fileName, "r");

    while (fscanf(fPtr, "%s %s", a, b) != EOF)
    {
        printf("a = %s \n", a);
        printf("b = %s\n", b);
        // calculated alen and blen
        alen = 0;
        while (a[alen] != 0) alen++;
        blen = 0;
        while (b[blen] != 0) blen++;

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

        HalfTheBitInt(large,  &largeLen);
        printf("large /2 =  ");
        for (int i = 0; i < largeLen; i++) printf("%d ", large[i]);
        printf("\n\n");

        DoubleVlueMultipleTimes(large, &largeLen, 1);
        printf("large /2 * 2 =  ");
        for (int i = 0; i < largeLen; i++) printf("%d ", large[i]);
        printf("\n\n");


        HalfTheBitInt(small, &smallLen);
        printf("small /2 =  ");
        for (int i = 0; i < smallLen; i++) printf("%d ", small[i]);
        printf("\n\n");


        DoubleVlueMultipleTimes(small, &smallLen, 1);
        printf("small /2 * 2 =  ");
        for (int i = 0; i < smallLen; i++) printf("%d ", small[i]);
        printf("\n\n");


        FirstSubstractSecondUpdateFirst(large, &largeLen, small, smallLen);
        printf("a - b =  ");
        for( int i = 0; i < largeLen; i++) printf("%d ",large[i]);
        printf("\n\n");

   }
    fclose(fPtr);



    // ***************** Formally solve the given problems **********************

    for (int z = 0; z < 50; z++)
    {
        printf("\n\n*************** problem %d.int ***************\n", z);
        char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\85.in";
        sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\%d.in", z);
        fPtr = fopen(fileName, "r");
        fscanf(fPtr, "%s %s", a, b);
        fclose(fPtr);

        // Test                                                       5
        //a[0] = '1'; a[1] = '9'; a[2] = '5'; a[3] = '8'; a[4] = '1'; a[5] = '0'; a[6] = '1'; a[7] = '9'; a[8] = 0;
        //b[0] = '1'; b[1] = '2'; b[2] = '3'; b[3] = '4'; b[4] = '5'; b[5] = '6'; b[6] = '7'; b[7] = '9'; b[8] = 0;
        //a[0] = '4'; a[1] = '2'; a[2] = 0;
        //b[0] = '6'; b[1] = '6'; b[2] = 0;

        // calculated alen and blen
        alen = 0;
        while (a[alen] != 0) alen++;
        blen = 0;
        while (b[blen] != 0) blen++;

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
            if ( IsFirstLargerThanSecond(large, largeLen, small, smallLen) < 0 )
            {
                // Swap large and small
                temp = large;
                large = small;
                small = temp;
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
        printf("GCD(%d) = %s \n",smallLen, small);

        sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\gcd\\%d.out", z);
        fPtr = fopen(fileName, "r");
        fscanf(fPtr, "%s", a );
        fclose(fPtr);
        printf("Answer  = %s\n\n", a);
    } 
    int iii;
    scanf("%d", &iii);
}








/*  Old version sealed
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
        // Swqp
        big = b; small = a; i = bLen; j = aLen;
    }


    k = i;
    c = malloc(sizeof( unsigned char ) * (k + 1)); // allocate memory for c with an extra char for end of string
    for (int z = 0; z < k + 1; z++) c[z] = 0;

    c[k] = 0; // End of the string

    i--; j--; // now all of them are indexes
    int sub = 0; // extra subtraction, initially zero

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

*/


