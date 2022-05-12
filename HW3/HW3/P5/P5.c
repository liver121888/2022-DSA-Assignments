#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)

typedef struct idAndString
{
	char* string;
	int id;
} IDandString;


int FileFlag = 0;
int DebugFlag = 0;
int re;
int number, size, flag;

const unsigned char low = 0x21;
const unsigned char up = 0x7E;
const int range = 0x7E - 0x21 + 1;
const unsigned long long q = ULONG_MAX/94;
IDandString** itemString, **sorted;

int main()
{
	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW3\\HW3\\hw3_testdata\\P5\\3.in", "r");
		re = fscanf(ptr, "%d %d %d", &number, &size, &flag);
	}
	else
		re = scanf("%d %d %d", &number, &size, &flag);

	itemString = malloc(sizeof(IDandString*) * number);
	sorted = malloc(sizeof(IDandString*) * number);
	for (int i = 0; i < number; i++)
	{
		itemString[i] = malloc(sizeof(IDandString));
		// may don't need size + 1
		itemString[i]->string = malloc(sizeof(char) * (size + 1));
		if (FileFlag)
			re = fscanf(ptr, "%s", itemString[i]->string);
		else
			re = scanf("%s", itemString[i]->string);
		itemString[i]->id = i;
		for (int j = 0; j < size; j++) 
			itemString[i]->string[j] = itemString[i]->string[j] - low;
	}

	// Radix Sort + Counting Sort
	int* countArray = malloc(sizeof(int) * range);
	for (int s = 0; s < size; s++)
	{
		// Reset counters
		for (int r = 0; r < range; r++) 
			countArray[r] = 0;
		// Count value appearances
		for (int i = 0; i < number; i++)
		{
			int v = (int)(itemString[i]->string[s]);
			countArray[v] = countArray[v] + 1;
		}
		// cumulate
		for (int r = 1; r < range; r++) 
			countArray[r] = countArray[r] + countArray[r - 1];
		for (int i = number - 1; i >= 0; i--)
		{
			int c = (int)(itemString[i]->string[s]);
			int pos = countArray[c] - 1; // target position
			sorted[pos] = itemString[i]; // assign pointer to the target position
			countArray[c] = countArray[c] - 1;
		}
		// update the pointer list with the sorted pointers 
		for (int i = 0; i < number; i++) 
			itemString[i] = sorted[i];
	}



	// For flag = 0 will simply cross check pairs of substrings until we find the first
	if (flag == 0)
	{
		int done = 0;
		for (int i = 0; i < number; i++)
		{
			for (int j = i + 1; j < number; j++)
			{
				int differentCount = 0;
				for (int s = 0; s < size; s++)
				{
					if (itemString[i]->string[s] != itemString[j]->string[s])
					{
						differentCount++;
						if (differentCount > 1) break;
					}
				}
				if (differentCount <= 1)
				{
					// i, j? j, i?
					printf("Yes\n");
					printf("%d ", itemString[j]->id);
					printf("%d\n", itemString[i]->id);
					//printf("Yes\n%d %d\n", itemString[j]->id, itemString[i]->id);
					done = 1;
					break;
				}
			}
			if (done) break;
		}
		if (!done)
		{
			printf("No\n");
		}
	}
	else
	{
		// flag == 1
		int similarCount = 0;
		for (int i = 0; i < number; i++)
		{
			for (int j = i + 1; j < number; j++)
			{
				int differentCount = 0;
				for (int s = 0; s < size; s++)
				{
					if (itemString[i]->string[s] != itemString[j]->string[s])
					{
						differentCount++;
						if (differentCount > 1) break;
					}
				}
				if (differentCount <= 1) similarCount++;
			}
		}
		if (similarCount == 0) printf("No\n");
		else
		{
			printf("Yes\n");
			printf("%d\n", similarCount);
		}
	}



	if (FileFlag)
	{
		fclose(ptr);
	}
	return 0;
}
#pragma   warning(pop)  