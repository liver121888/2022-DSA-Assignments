#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


int N, M;
char cmd[20];
int* groupCounts;

void main()
{
	int boomDay;
	int first, second;

	scanf("%d %d", &N, &M);

	groupCounts = malloc(sizeof(int) * (M + 1));
	groupCounts[0] = N;

	int currentGroupCount = N;
	for (int day = 1; day <= M; day++)
	{
		scanf("%s", cmd);
		switch (cmd[0])
		{
		case 'm': // merge
			scanf("%d %d", &first, &second);
			currentGroupCount--;
			break;
		case 'q': // query
			printf("%d\n", currentGroupCount);
			break;
		case 'b': // boom
			scanf("%d", &boomDay);
			currentGroupCount = groupCounts[boomDay];
			break;
		}
		// Store groupCount of this day
		groupCounts[day] = currentGroupCount;
	}

}