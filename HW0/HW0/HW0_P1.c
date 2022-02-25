#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH 512
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int alength, blength, clength;


char BigIntsComparator(char a[], char b[], int alen, int blen)
{
	char aisbigger = 0;
	if (alen > blen)
		aisbigger = 1;
	else if (alen < blen)
		goto out;
	else
	{
		for (size_t i = 0; i < alen; i++)
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

char* BigIntsSubtraction(char a[], char b[], int alen, int blen, int* clen)
{
	char* bigger, * smaller;
	char c[MAX_LENGTH];
	int biggerlength = 0, smallerlength = 0;
	int _clen = 0;

	char isabigger = BigIntsComparator(a, b, alen, blen);
	if (isabigger == 1)
	{
		bigger = a;
		biggerlength = alen;
		smaller = b;
		smallerlength = blen;
	}
	else
	{
		bigger = b;
		biggerlength = blen;
		smaller = a;
		smallerlength = alen;
	}

	int borrower = 0;
	int tmp = 0;
	for (int bb = biggerlength - 1, ss = smallerlength - 1; bb >= 0; bb--, ss--)
	{
		if (ss < 0)
		{
			c[bb] = bigger[bb] - borrower;
			borrower = 0;
			_clen++;
		}
		else
		{
			tmp = bigger[bb] - borrower - smaller[ss];
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
			_clen++;
		}
	}
	c[biggerlength] = 0;
	//remember to modify the upperbound of i
	for (size_t i = 0; i < MAX_LENGTH; i++)
	{
		if (c[i] != '0')
			break;
		else
		{
			for (size_t j = 0; j < MAX_LENGTH; j++)
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
	*clen = _clen;
	return c;
}

char* BigIntsMultiplication(char* array, int* length, int k)
{
	int carrier = 0;
	int net = 0;
	//char* c = malloc(sizeof(char) * MAX_LENGTH);
	char c[260];
	for (size_t i = 0; i < *length; i++)
	{
		c[i] = array[i] - '0';
	}
	c[*length] = 0;
	int clength = 0;
	for (size_t j = 1; j <= k; j++)
	{
		while (c[clength] != 0) clength++;
		if (c[0] >= 5)
		{
			clength = clength + 1;
		}
		for (size_t i = clength - 1; i >= 0; i--)
		{
			net = (c[i] + carrier) * 2;
			if (net >= 10)
			{
				carrier = 1;
				c[i] = net - 10;
			}
			else
			{
				carrier = 0;
				c[i] = net;
			}
		}
	}
	for (size_t i = 0; i < clength; i++)
	{
		c[i] = c[i] + '0';
	}
	return c;
}

unsigned char* BigIntsDivision(unsigned char a[], int k)
{


	return NULL;
}

int main()
{
	FILE* ptr;
	char c1[MAX_LENGTH], c2[MAX_LENGTH];
	//char* c3 = malloc(sizeof(char) * MAX_LENGTH);
	char* c3;
	ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW0\\HW0\\hw0_testdata\\gcd\\50.in", "r");
	fscanf(ptr, "%s %s", c1, c2);

	char* aptr = c1, * bptr = c2;

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

	//while (c1[0] != 0 && c2[0] != 0)
	//{
	//	if (c1)
	//	{

	//	}
	//}





	c3 = BigIntsSubtraction(c1, c2, alength, blength, &clength);

	c3 = BigIntsMultiplication(c3, &clength,2);



	fclose(ptr);
	return 0;
}
