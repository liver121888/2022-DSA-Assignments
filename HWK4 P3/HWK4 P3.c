#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int N, M, size;
int i, j, k;

char cmd[20];

struct node
{
	int groupCount;
	struct node* sibling;
	struct node* root;
};

void main()
{
	
	scanf("%d %d", N, M);
	size = N;

	for (int day = 1; day <= M; day++)
	{
		scanf("%s", cmd);
		
		switch (cmd[0])
		{
		case 'm': // merge
			scanf("%d %d", &i, &j);
			break;
		case 'q': // query
			printf("%d\n", size);
			break;
		case 'b': // boom
			scanf("%d", &k);
			break;
		}
	}
}