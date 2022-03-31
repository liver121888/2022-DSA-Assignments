#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#pragma warning(push)
//#pragma warning(disable:6385)
//#pragma warning(disable:6386)
//#pragma warning(disable:6011)


typedef struct node {
	struct node* link;
	int Group;
	int ID;
}node;


node** head;
node** tail;

node*** GroupEnds, *** GroupEndNexts;
node*** GroupStarts, *** GroupStartPrevs; // used in Dismiss functions


char* closed;
int WCNumber, SituationNumber, GroupNumber;
int FileFlag = 1;
int PrintQueueInfoFlag = 0;


int main()
{
	char c1[6];
	int m, j, i;
	int re, res, r;

	FILE* ptr = 0;

	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW1\\HW1\\hw1_testdata\\P5\\1.in", "r");
		r = fscanf(ptr, "%d %d %d", &WCNumber, &SituationNumber, &GroupNumber);
	}
	else
	{
		r = scanf("%d %d %d", &WCNumber, &SituationNumber, &GroupNumber);
	}


	head = malloc(sizeof(node*) * WCNumber);
	tail = malloc(sizeof(node*) * WCNumber);
	GroupEnds = malloc(sizeof(node**) * WCNumber);
	GroupStarts = malloc(sizeof(node**) * WCNumber);
	GroupEndNexts = malloc(sizeof(node**) * WCNumber);
	GroupStartPrevs = malloc(sizeof(node**) * WCNumber);

	closed = malloc(sizeof(char) * WCNumber);


	for (int k = 0; k < WCNumber; k++)
	{
		head[k] = 0;
		tail[k] = 0;
		closed[k] = 'n';
		GroupStarts[k] = malloc(sizeof(node*) * GroupNumber);
		GroupEnds[k] = malloc(sizeof(node*) * GroupNumber);
		GroupStartPrevs[k] = malloc(sizeof(node*) * GroupNumber);
		GroupEndNexts[k] = malloc(sizeof(node*) * GroupNumber);
		for (int j = 0; j < GroupNumber; j++)
		{
			GroupStarts[k][j] = 0;
			GroupEnds[k][j] = 0;
			GroupStartPrevs[k][j] = 0;
			GroupEndNexts[k][j] = 0;
		}
	}


	for (int k = 0; k < SituationNumber; k++)
	{
		if (FileFlag)
		{
			re = fscanf(ptr, "%s", c1);
		}
		else
		{
			re = scanf("%s", c1);
		}

		switch (c1[0])
		{
		case('e'):
			if (FileFlag)
			{
				res = fscanf(ptr, "%d %d %d", &i, &j, &m);
			}
			else
			{
				res = scanf("%d %d %d", &i, &j, &m);
			}
			enteringroup(i, j, m);
			break;
		case('l'):
			if (FileFlag)
			{
				res = fscanf(ptr, "%d", &m);
			}
			else
			{
				res = scanf("%d", &m);
			}
			leave(m);
			break;
		case('g'):
			if (FileFlag)
			{
				res = fscanf(ptr, "%d", &m);
			}
			else
			{
				res = scanf("%d", &m);
			}
			go(m);
			break;
		case('c'):
			if (FileFlag)
			{
				res = fscanf(ptr, "%d", &m);
			}
			else
			{
				res = scanf("%d", &m);
			}
			closeingroup(m);
			break;
		default:
			break;
		}
		if (PrintQueueInfoFlag)
		{
			PrintQueueInfo();
		}


	}

	node* tmpptr, * last, * next;
	for (int k = 0; k < WCNumber; k++)
	{
		last = NULL;
		tmpptr = head[k];
		while (tmpptr)
		{
			printf("%d ", tmpptr->ID);
			next = (unsigned int)tmpptr->link ^ (unsigned int)last;
			last = tmpptr;
			tmpptr = next;
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