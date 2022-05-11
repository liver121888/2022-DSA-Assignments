#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


typedef struct subString
{
	char* string;
	int id;
} SubString;

 
int length;
unsigned char low = 0x21;
unsigned char up = 0x7E;
int range = 0x7E - 0x21 + 1;
int number, size, flag;
SubString ** subStrings, ** sorted;
 
void main()
{

	scanf("%d %d %d", &number, &size, &flag);
	// allocate a chuck of contijous memory.
 	subStrings = malloc(sizeof(SubString*) * number);
	sorted = malloc(sizeof(SubString*) * number);

	for (int i = 0; i < number; i++)
	{
		subStrings[i] = malloc(sizeof(subStrings));
		subStrings[i]->string = malloc(sizeof(char) * size);
		scanf("%s", subStrings[i]->string );
		subStrings[i]->id = i;
		for (int j = 0; j < size; j++) subStrings[i]->string[j] -= low;
	}

	// Redix Sort
	int* countArray = malloc(sizeof(int) * range);
	for (int s = 0; s < size; s++)
	{
		// Reset counters
		for (int r = 0; r < range; r++) countArray[r] = 0;
		// Count value appearances
		for (int i = 0; i < number; i++)
		{
			int v = (int)( subStrings[i]->string[s]);
			countArray[v]++;
		}
		// cumulate count threshes
		for (int r = 1; r < range; r++) countArray[r] += countArray[r - 1];
		for (int i = number-1; i >= 0; i--)
		{
			int c = (int)( subStrings[i]->string[s] );
			int pos = countArray[c] - 1; // target position
			sorted[pos] = subStrings[i]; // assign pointer to the target position
			countArray[c] --; // places left the the target value reduced by one
		}
		// update the pointer list with the sorted pointers 
		for (int i = 0; i < number; i++) subStrings[i] = sorted[i];
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
					if (subStrings[i]->string[s] != subStrings[j]->string[s])
					{
						differentCount++;
						if (differentCount > 1) break;
					}
				}
				if (differentCount > 1) break;
				else
				{
					printf("Yes\n%d %d\n", subStrings[i]->id, subStrings[j]->id);
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
					if (subStrings[i]->string[s] != subStrings[j]->string[s])
					{
						differentCount++;
						if (differentCount > 1) break;
					}
				}
				if (differentCount <= 1 ) similarCount++;
			}
		}
		if (similarCount == 0) printf("No\n");
		else printf("Yes\n%d\n", similarCount);
	}
}