#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)

// References:
// Ref: https://www.geeksforgeeks.org/minimum-characters-added-front-make-string-palindrome/?ref=lbp
// Ref: https://www.geeksforgeeks.org/minimum-number-appends-needed-make-string-palindrome/?ref=lbp
// For finding the LCA of two nodes


int FileFlag = 1;
int DebugFlag = 0;
int re;
char s[10 * 1000 * 1000 + 1];
char* concat;
int* lps;
int concatlen;
int len;

int computeLPSArray(char* c)
{
	int index = 0;
	lps[0] = 0;

	// the loop calculates lps[i] for i = 1 to M-1
	int i = 1;
	while (i < concatlen)
	{
		if (c[i] == c[index])
		{
			index++;
			lps[i] = index;
			i++;
		}
		else // (str[i] != str[len])
		{
			// This is tricky. Consider the example.
			// AAACAAAA and i = 7. The idea is similar
			// to search step.
			if (index != 0)
			{
				index = lps[index - 1];

				// Also, note that we do not increment
				// i here
			}
			else // if (len == 0)
			{
				lps[i] = 0;
				i++;
			}
		}
	}
	return len - lps[concatlen - 1];
}

int Compute_Prefix_Function(char* P)
{
	int k = 0;
	lps[0] = 0;
	for (int q = 1; q < concatlen; q++)
	{
		while (k > 0 && P[k] != P[q])
		{
			k = lps[k - 1];
		}
		if (P[k] == P[q])
			k = k + 1;
		lps[q] = k;
	}
	if (DebugFlag)
	{
		printf("lps: \n");
		for (int i = 0; i < concatlen; i++)
			printf("%d, ", lps[i]);
		printf("\n");
	}
	return len - lps[concatlen - 1];
}






int main()
{
	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW3\\HW3\\hw3_testdata\\P4\\1.in", "r");
		re = fscanf(ptr, "%s", &s);
	}
	else
		re = scanf("%s", &s);
	len = strlen(s);
	concatlen = 2 * len + 1;
	concat = malloc(sizeof(char) * (concatlen));
	// reversed is insert at front
	for (int i = 0; i < len; i++)
		concat[i] = s[len - 1 - i];
	concat[len] = 7;
	for (int i = len + 1; i < concatlen; i++)
		concat[i] = s[i - (len + 1)];
	

	if (DebugFlag)
	{
		for (int i = 0; i < concatlen; i++)
			printf("%c, ", concat[i]);
		printf("\n");
	}

	lps = malloc(sizeof(int) * (concatlen));
	int b = Compute_Prefix_Function(concat);
	if (DebugFlag)
	{
		printf("b: %d\n", b);
	}

	// reversed is insert at back
	for (int i = 0; i < len; i++)
	{
		concat[i] = s[i];
	}
	concat[len] = 7;
	for (int i = len + 1; i < concatlen; i++)
	{
		concat[i] = s[concatlen - i - 1];
	}
	if (DebugFlag)
	{
		for (int i = 0; i < concatlen; i++)
			printf("%c, ", concat[i]);
		printf("\n");
	}

	int f = Compute_Prefix_Function(concat);
	if (DebugFlag)
	{
		printf("f: %d\n", f);
	}



	if (f == b)
	{
		printf("%d\n", f);
		for (int i = 0; i < f; i++)
		{
			printf("%c", s[len - 1 - i]);
		}
		printf("%s\n", s);

		if (f != 0)
		{
			printf("%s", s);
			for (int i = b - 1; i >= 0; i--)
			{
				printf("%c", s[i]);
			}
			printf("\n");
		}
	}
	else if (f < b)
	{
		printf("%d\n", f);
		for (int i = 0; i < f; i++)
		{
			printf("%c", s[len - 1 - i]);
		}
		printf("%s\n", s);
	}
	else
	{
		printf("%d\n", b);
		printf("%s", s);
		for (int i = b - 1; i >= 0; i--)
		{
			printf("%c", s[i]);
		}
		printf("\n");
	}

	if (FileFlag && ptr != NULL)
	{
		fclose(ptr);
	}
	return 0;
}
#pragma   warning(pop)  