//#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>



unsigned char* BigIntsSubtraction(unsigned char a[], unsigned char b[]) {


    return NULL;
}

unsigned char* BigIntsMultiplication(unsigned char a[], int k) {


    return NULL;
}

unsigned char* BigIntsDivision(unsigned char a[], int k) {


    return NULL;
}


int main()
{
    char buf1[100], buf2[100];
    FILE* ptr;
   ptr = fopen("D:\\2022 GitHubProject\\DatasStructureAlgorithm\\HW0\\HW0\\x64\\Debug\\test.txt", "r");
    // ptr = fopen("test.txt", "r");
    fscanf(ptr, "%s", buf1 );
    fscanf(ptr, "%s", buf2);
 
    printf("%s", buf1);
    printf("%s", buf2);
    fclose(ptr);
    int i;

    char* res =  BigIntsSubtraction(buf1, buf2);

    scanf("%d", &i);
}
