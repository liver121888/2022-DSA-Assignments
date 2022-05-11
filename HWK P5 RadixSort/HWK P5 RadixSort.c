#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


typedef struct idAndString
{
	char* string;
	int id;
} IDandString;

 
int length;
unsigned char low = 0x21;
unsigned char up = 0x7E;
int range = 0x7E - 0x21 + 1;
int number, size, flag;
IDandString ** itemString, ** sorted;
 
void main()
{

	scanf("%d %d %d", &number, &size, &flag);
	// allocate a chuck of contijous memory.
 	itemString = malloc(sizeof(IDandString*) * number);
	sorted = malloc(sizeof(IDandString*) * number);

	for (int i = 0; i < number; i++)
	{
		itemString[i] = malloc(sizeof(IDandString));
		itemString[i]->string = malloc(sizeof(char) * (size+1) );
		scanf("%s", itemString[i]->string );
		itemString[i]->id = i;
		for (int j = 0; j < size; j++) itemString[i]->string[j] = itemString[i]->string[j] - low;
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
			int v = (int)( itemString[i]->string[s]);
			countArray[v] = countArray[v] + 1;
		}
		// cumulate count threshes
		for (int r = 1; r < range; r++) countArray[r] = countArray[r] + countArray[r - 1];
		for (int i = number-1; i >= 0; i--)
		{
			int c = (int)( itemString[i]->string[s] );
			int pos = countArray[c] - 1; // target position
			sorted[pos] = itemString[i]; // assign pointer to the target position
			countArray[c] = countArray[c] - 1; // places left the the target value reduced by one
		}
		// update the pointer list with the sorted pointers 
		for (int i = 0; i < number; i++) itemString[i] = sorted[i];
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
					printf("Yes\n%d %d\n", itemString[i]->id, itemString[j]->id);
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
				if (differentCount <= 1 ) similarCount++;
			}
		}
		if (similarCount == 0) printf("No\n");
		else printf("Yes\n%d\n", similarCount);
	}
}