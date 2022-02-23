//#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>



const unsigned char* BigIntsSubtraction(unsigned char a[], unsigned char b[]) 
{
    char c[256];
    int alen = 0, blen =0;
    unsigned char* aa, * bb;
    aa =a; bb = b;
    while (*aa++ != 0) alen++;
    while (*bb++ != 0) blen++;

    unsigned char* big =0, * small=0;
    int Blen, Slen;
    if (alen > blen)
    {
        big = a; small = b; Blen = alen; Slen = blen;  
    }
    else if (alen == blen)
    {
        aa = a;
        bb = b;
        while (*aa++ == *bb++)
        if (*aa > *bb)
        {
            big = a; small = b; Blen = alen; Slen = blen;
        }
        else
        {
            big = b; small = a; Blen = blen; Slen = alen;
        }
    }
    else
    {

        big = b; small = a; Blen = blen; Slen = alen;
    }
    int Clen;
    if (big[0] == small[0]) Clen = Blen - 1;
    else Clen = Blen;
    c[Clen] = 0;
    Clen--; Blen--; Slen--;
    int sub = 0;
    
    while (Blen >= 0)
    {
        if (Slen < 0)
            c[Blen] = big[Blen];
        else
        {
            int net = big[Blen] - sub - small[Slen];
            if (net > 0)
            {
                c[Clen] = '0'+net; sub = 0;
            }
            else 
            {
                c[Clen] = '0'+( 10 + net); sub = 1;
            }
        }
        Blen--;
        Slen--;
        Clen--;
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
   ptr = fopen("D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW0\\HW0\\x64\\Debug\\test.txt", "r");
    // ptr = fopen("test.txt", "r");
    fscanf(ptr, "%s", buf1 );
    fscanf(ptr, "%s", buf2);
 
    printf("%s", buf1);
    printf("%s", buf2);
    fclose(ptr);

    char* res =  BigIntsSubtraction(buf1, buf2);

    int i;
    scanf("%d", &i);
}
