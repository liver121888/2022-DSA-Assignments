#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH 260
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int BigIntsComparator(char a[], char b[], int alen, int blen)
{
	if (alen > blen) return 1;
	else if (alen < blen) return -1;
	for (size_t i = 0; i < alen; i++)
		if (a[i] - b[i] > 0) return 1;
		else if (a[i] - b[i] < 0) return -1;
	return 0;
}

void BigIntsSubtraction(char a[], char b[], int* alen, int blen)
{
	int l = *alen;

	int borrower = 0;
	int tmp = 0;
	for (int bb = l - 1, ss = blen - 1; bb >= 0; bb--, ss--)
	{
		int low = ss >= 0 ? b[ss] : 0;
		tmp = a[bb] - borrower - low;
		if (tmp < 0)
		{
			borrower = 1;
			a[bb] = 10 + tmp;
		}
		else
		{
			borrower = 0;
			a[bb] = tmp;
		}
	}
	tmp = 0;
	for (size_t i = 0; i < l; i++)
		if (a[i] == 0) tmp++;
		else break;

	if (tmp == l)
		*alen = 1;
	else if (tmp != 0)
	{
		*alen = l - tmp;
		for (size_t i = 0; i < l - tmp; i++)
			a[i] = a[i + tmp];
	}
}

void BigIntsMultiplication(char* array, int* length, int k)
{
	int carrier = 0;
	int net = 0;
	int len = *length;
	int index = -1;

	for (size_t j = 1; j <= k; j++)
	{
		if (array[0] >= 5) index = len;
		else index = len - 1;
		*length = index + 1;
		carrier = 0;
		for (int i = len - 1; i >= 0; i--)
		{
			net = array[i] * 2 + carrier;
			if (net >= 10)
			{
				carrier = 1;
				array[index] = net - 10;
				if (i == 0) array[index - 1] = 1;
			}
			else
			{
				carrier = 0;
				array[index] = net;
			}
			index--;
		}
		len = *length;
	}
}

void BigIntsDivision(char a[], int* alen)
{
	int len = *alen;
	int tmp = 0;
	int tmp2 = 0;
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		tmp2 = a[i] + tmp * 10;
		// special case when leading digit is 1, the len have to -1, plus i one more time to achieve this
		if (tmp2 < 2 && i == 0 && len>1)
		{
			i++;
			tmp2 = tmp2 * 10 + a[i];
		}
		a[j] = tmp2 / 2;
		tmp = tmp2 % 2;
		j++;
	}
	*alen = j;
}

int main()
{
	//FILE* ptr;
	int c1length = 0, c2length = 0, blen, slen, inttmp;
	char c1[MAX_LENGTH], c2[MAX_LENGTH];
	//char* c3 = malloc(sizeof(char) * MAX_LENGTH);
	char* big, * small, * tmp;
	//ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW0\\HW0\\hw0_testdata\\gcd\\50.in", "r");
	scanf("%s %s", c1, c2);

	//fclose(ptr);
	while (c1[c1length] != 0) c1length++;
	while (c2[c2length] != 0) c2length++;

	for (size_t i = 0; i < c1length; i++)
		c1[i] = c1[i] - '0';
	for (size_t i = 0; i < c2length; i++)
		c2[i] = c2[i] - '0';


	if (BigIntsComparator(c1, c2, c1length, c2length) > 0)
	{
		big = c1; small = c2; blen = c1length; slen = c2length;
	}
	else
	{
		big = c2; small = c1; blen = c2length; slen = c1length;
	}

	int ans = 0;
	int isbeven;
	int isseven;
	while (big[0] != 0 && small[0] != 0)
	{
		isbeven = big[blen - 1] % 2 == 0;
		isseven = small[slen - 1] % 2 == 0;

		if (isseven && isbeven)
		{
			ans++;
			BigIntsDivision(small, &slen);
			BigIntsDivision(big, &blen);
		}
		else if (isseven)
			BigIntsDivision(small, &slen);
		else if (isbeven)
			BigIntsDivision(big, &blen);

		if (BigIntsComparator(big, small, blen, slen) < 0)
		{
			tmp = big;
			big = small;
			small = tmp;
			inttmp = blen;
			blen = slen;
			slen = inttmp;
		}
		BigIntsSubtraction(big, small, &blen, slen);
	}
	BigIntsMultiplication(small, &slen, ans);
	for (size_t i = 0; i < slen; i++) 
		small[i] += '0';
	small[slen] = 0;
	printf(small);

	return 0;
}
