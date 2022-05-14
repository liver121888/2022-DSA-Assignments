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
	unsigned long long RKValue;
	int	id;
	char* string;
} IDandString;


int FileFlag = 1;
int DebugFlag = 0;

unsigned long long q = ULLONG_MAX / (0x7E - 0x21 + 1) + 1;

int re;
int number, size, flag;

const unsigned char low = 0x21;
const unsigned char up = 0x7E;
const int range = 0x7E - 0x21 + 1;


IDandString* itemString, *subString;


int compare(const void* arg1, const void* arg2)
{
	/* Compare all of both strings: */
	struct idAndString* a = (struct idAndString*)arg1;
	struct idAndString* b = (struct idAndString*)arg2;

	if (a->RKValue < b->RKValue)
	{
		return -1;
	}
	else if (a->RKValue == b->RKValue)
	{
		return 0;
	}
	else
		return 1;
}


void printArray(IDandString* ptr, int len)
{
	for (int i = 0; i < len; i++)
	{
			printf("(%llu, %d), ", ptr[i].RKValue, ptr[i].id);
	}
	printf("\n");
}


int main()
{
	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW3\\HW3\\hw3_testdata\\P5\\7.in", "r");
		re = fscanf(ptr, "%d %d %d", &number, &size, &flag);
	}
	else
		re = scanf("%d %d %d", &number, &size, &flag);
	int found = 0;
	if (number == 1)
		goto done;

	itemString = malloc(sizeof(IDandString) * number);
	subString = malloc(sizeof(IDandString) * number);
	char* tmp = malloc(sizeof(char) * (size + 1));
	for (int i = 0; i < number; i++)
	{
		itemString[i].id = i;
		itemString[i].RKValue = 0;

		if (FileFlag)
			re = fscanf(ptr, "%s", tmp);
		else
			re = scanf("%s", tmp);
		itemString[i].string = malloc(sizeof(char) * (size + 1));

		for (int j = 0; j < size; j++)
		{
			if (itemString[i].string)
			{
				itemString[i].string[j] = tmp[j] - low;
				itemString[i].RKValue = (itemString[i].RKValue * (range) % q + (unsigned long long)(tmp[j] - low))%q;
			}
		}
	}
	if (DebugFlag)
	{
		printf("Non-Sorted itemString: \n");
		printArray(itemString, number);
	}

	// qsort 
	if (itemString != NULL)
		qsort((void*)itemString, (size_t)number, sizeof(struct idAndString), compare);

	if (DebugFlag)
	{
		printf("Sorted itemString: \n");
		printArray(itemString, number);
	}


	unsigned long long x = 1;

	if (flag == 0)
	{
		found = 0;
		for (int i = 1; i < number; i++)
		{
			if (itemString[i-1].RKValue == itemString[i].RKValue)
			{
				printf("Yes\n");
				printf("%d ", itemString[i].id);
				printf("%d\n", itemString[i-1].id);
				found = 1;
				goto done;
			}
		}
		for (int i = size - 1; i >= 0; i--)
		{
			for (int j = 0; j < number; j++)
			{
				subString[j].id = itemString[j].id;
				//subString[j].RKValue = ( (itemString[subString[j].id].RKValue + q) - (unsigned long long)(itemString[subString[j].id].string[i])*x %q )%q;
				subString[j].RKValue = ((itemString[j].RKValue + q) - (unsigned long long)(itemString[j].string[i]) * x % q) % q;
			}
			x = (x * range) % q;

			if (DebugFlag)
			{
				printf("Non-Sorted subString: \n");
				printArray(subString, number);
			}
			qsort((void*)subString, (size_t)number, sizeof(struct idAndString), compare);
			if (DebugFlag)
			{
				printf("Sorted subString: \n");
				printArray(subString, number);
			}
			for (int k = 1; k < number; k++)
			{
				if (subString[k-1].RKValue == subString[k].RKValue)
				{
					printf("Yes\n");
					printf("%d ", subString[k].id);
					printf("%d\n", subString[k-1].id);
					found = 1;
					goto done;
				}
			}
		}
		done:
		if (!found)
		{
			printf("No\n");
		}
	}
	else
	{
		// flag == 1
		unsigned long long similarCount = 0;
		unsigned long long totalCount = 0;
		unsigned long long identicalCount = 0;
		unsigned long long copycnt = 0;
		for (int i = 1; i < number; i++)
		{
			if (itemString[i - 1].RKValue == itemString[i].RKValue)
			{
				identicalCount++;
			}
			else
			{
				if (identicalCount != 1)
				{
					totalCount += identicalCount * (identicalCount + 1) / 2;
					copycnt += identicalCount * (identicalCount + 1) / 2;
				}
				else
				{
					totalCount += identicalCount;
					copycnt += identicalCount;
				}
				identicalCount = 0;
			}
		}
		if (identicalCount != 1)
		{
			totalCount += identicalCount * (identicalCount + 1) / 2;
			copycnt += identicalCount * (identicalCount + 1) / 2;
		}
		else
		{
			totalCount += identicalCount;
			copycnt += identicalCount;
		}

		for (int i = size - 1; i >= 0; i--)
		{
			for (int j = 0; j < number; j++)
			{
				subString[j].id = itemString[j].id;
				//subString[j].RKValue = ( (itemString[subString[j].id].RKValue + q) - (unsigned long long)(itemString[subString[j].id].string[i]) * x % q) % q;
				subString[j].RKValue = ((itemString[j].RKValue + q) - (unsigned long long)(itemString[j].string[i]) * x % q) % q;
			}
			x = (x * range) % q;


			if (DebugFlag)
			{
				printf("Non-Sorted subString: \n");
				printArray(subString, number);
			}
			qsort((void*)subString, (size_t)number, sizeof(struct idAndString), compare);
			if (DebugFlag)
			{
				printf("Sorted subString: \n");
				printArray(subString, number);
			}
			for (int k = 1; k < number; k++)
			{
				if (subString[k - 1].RKValue == subString[k].RKValue)
				{
					similarCount++;
				}
				else
				{
					if (similarCount != 1)
					{
						totalCount += similarCount * (similarCount + 1) / 2;
					}
					else
						totalCount += similarCount;
					similarCount = 0;
				}
			}
			if (similarCount != 1)
			{
				totalCount += similarCount * (similarCount + 1) / 2;
			}
			else
				totalCount += similarCount;
			similarCount = 0;
		}
		unsigned long long out = totalCount - (unsigned long long)(copycnt) * (unsigned long long)(size);

		if (out == 0) printf("No\n");
		else
		{
			printf("Yes\n");
			printf("%llu\n", out);
		}
	}

	if (FileFlag)
	{
		fclose(ptr);
	}
	return 0;
}
#pragma warning(pop)  