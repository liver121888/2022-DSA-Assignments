#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char BigIntsComparator(char a[], char b[], int alength, int blength)
{
	char aisbigger = 0;
	if (alength > blength)
		aisbigger = 1;
	else if (alength < blength)
		goto out;
	else
	{
		for (size_t i = 0; i < alength; i++)
		{
			if (a[i] - b[i] > 0)
			{
				aisbigger = 1;
				goto out;
			}
			else if (a[i] - b[i] < 0)
				goto out;
		}
	}
out:
	return aisbigger;
}

unsigned char* BigIntsSubtraction(char a[], char b[])
{
	char* bigger, * smaller, * aptr = a, * bptr = b;
	char c[256];
	int biggerlength = 0, smallerlength = 0, alength = 0, blength = 0;
	while (*aptr != 0)
	{
		aptr++;
		alength++;
	}
	while (*bptr != 0)
	{
		bptr++;
		blength++;
	}
	char isabigger = BigIntsComparator(a, b, alength, blength);
	if (isabigger == 1)
	{
		bigger = a;
		biggerlength = alength;
		smaller = b;
		smallerlength = blength;
	}
	else
	{
		bigger = b;
		biggerlength = blength;
		smaller = a;
		smallerlength = alength;
	}

	int borrower = 0;
	for (int bb = biggerlength - 1, ss = smallerlength - 1; ss >= 0; bb--, ss--)
	{
		int tmp = bigger[bb] - borrower - smaller[ss];
		if (tmp < 0)
		{
			borrower = 1;
			c[bb] = '0' + (10 + tmp);
		}
		else
		{
			borrower = 0;
			c[bb] = '0' + tmp;
		}
	}
	for (size_t i = 0; i < 256; i++)
	{
		if (c[i] != '0')
			break;
		else
		{
			for (size_t j = 0; j < 256; j++)
			{
				if (c[j + 1] >= '0')
					c[j] = c[j + 1];
				else
				{
					c[j] = NULL; //TOOD
					break;
				}
			}
		}
	}
	return c;
}

unsigned char* BigIntsMultiplication(unsigned char a[], int k) {


	return NULL;
}

unsigned char* BigIntsDivision(unsigned char a[], int k) {


	return NULL;
}

int main()
{
	FILE* ptr;
	char c1[256], c2[256];
	ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW0\\HW0\\hw0_testdata\\gcd\\2.in", "r");
	fscanf(ptr, "%s %s", c1, c2);

	BigIntsSubtraction(c1, c2);

	fclose(ptr);
	return 0;
}
