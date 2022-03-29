#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#pragma warning(push)
//#pragma warning(disable:6385)
//#pragma warning(disable:6386)
//#pragma warning(disable:6011)


typedef struct node {
	struct node* prev;
	struct node* next;
	int Group;
	int ID;
	int reversed;
}node;


node** head;
node** tail;

node*** GroupEnds;
node*** GroupStarts; // used in Dismiss functions


char* closed;
int WCNumber, SituationNumber, GroupNumber;
int FileFlag = 1;


void enteringroup(int g, int id, int m)
{
	node* n = malloc(sizeof(node));
	if (n != 0)
	{
		n->ID = id;
		n->Group = g;
		n->reversed = 0;
	}
	node* groupEnd = GroupEnds[m][g];
	if (groupEnd)
	{
		// The group exist in this queue
		node* lastNext = groupEnd->next;
		groupEnd->next = n;
		n->prev = groupEnd;
		n->next = lastNext;
		if (lastNext)
			lastNext->prev = n;
		else
			tail[m] = n;
		GroupEnds[m][g] = n;
	}
	else
	{
		// This is the first guy of a new group enters this queue
		// Enter tail of the queue
		if (tail[m])
		{
			// tail exist
			tail[m]->next = n;
			n->prev = tail[m];
			n->next = 0;
			tail[m] = n;
		}
		else
		{
			// empty queue
			head[m] = n;
			tail[m] = n;
			n->prev = 0;
			n->next = 0;
		}
		GroupStarts[m][g] = n;
		GroupEnds[m][g] = n;
	}
}



void closeingroup(int m)
{
	node* Qtail = 0;
	node* endGuy;
	node* startGuy;
	node* joinStart;
	node* joinEnd;
	node* prevNext;
	Qtail = tail[m];

	int gid;
	int id;

	closed[m] = 'y';
	for (int k = m; k >= -1; k--)
	{
		if (k == -1)
			k = WCNumber - 1;

		if (closed[k] == 'n')
		{
			while (Qtail)
			{
				// current group id
				gid = Qtail->Group;
				// the next group end for next iter
				Qtail = GroupStarts[m][gid]->prev;

				// reverse id of this group
				endGuy = GroupEnds[m][gid];
				startGuy = GroupStarts[m][gid];
				id = endGuy->ID;
				while (startGuy->ID != id)
				{
					endGuy->ID = startGuy->ID;
					startGuy->ID = id;
					// if the people in queue is odd? then it would not enter the while loop
					if (endGuy->prev == startGuy || endGuy == startGuy)
						break;
					endGuy = endGuy->prev;
					id = endGuy->ID;
					startGuy = startGuy->next;
				}


				// link group gid in queue to k
				joinStart = GroupStarts[k][gid];
				joinEnd = GroupEnds[k][gid];
				startGuy = GroupStarts[m][gid];
				endGuy = GroupEnds[m][gid];

				if (joinEnd)
				{
					// group exist
					prevNext = joinEnd->next;
					joinEnd->next = startGuy;
					startGuy->prev = joinEnd;

					endGuy->next = prevNext;
					if (prevNext)
						prevNext->prev = endGuy;
					else
						// if the people in queue is odd?
						tail[k] = endGuy;

					// group added, update end
					GroupEnds[k][gid] = endGuy;
				}
				else
				{
					// The group is not in the jointQueue
					// append to tail of jointQueue
					joinEnd = tail[k];
					if (joinEnd)
					{
						joinEnd->next = startGuy;
						startGuy->prev = joinEnd;

						tail[k] = endGuy;
						endGuy->next = 0;
					}
					else
					{
						// joint queue is empty

						head[k] = startGuy;
						tail[k] = endGuy;
						startGuy->prev = 0;
						endGuy->next = 0;
					}

					GroupStarts[k][gid] = startGuy;
					GroupEnds[k][gid] = endGuy;

				}
			}
			break;
		}
	}

	for (int j = 0; j < GroupNumber; j++)
	{
		GroupEnds[m][j] = GroupStarts[m][j] = 0;
	}
	tail[m] = 0;
	head[m] = 0;
}

void go(int m)
{
	node* nextGuy;
	int gid;
	if (head[m])
	{
		gid = head[m]->Group;
		nextGuy = head[m]->next;
		if (nextGuy)
		{
			// nextGuy exist
			head[m] = nextGuy;
			nextGuy->prev = 0;
			if (nextGuy->Group == gid)
			{
				GroupStarts[m][gid] = nextGuy;
			}
			else
			{
				GroupStarts[m][gid] = 0;
				GroupEnds[m][gid] = 0;
			}
		}
		else
		{
			GroupEnds[m][gid] = 0;
			GroupStarts[m][gid] = 0;
			head[m] = 0;
			tail[m] = 0;
		}
	}
}

void leave(int m)
{
	int gid;
	node* prevGuy;
	// free?
	if (tail[m])
	{
		gid = tail[m]->Group;
		prevGuy = tail[m]->prev;
		if (prevGuy)
		{
			// previous guy exits
			tail[m] = prevGuy;
			prevGuy->next = 0;
			if (prevGuy->Group == gid)
				// same g
				GroupEnds[m][gid] = prevGuy;
			else
			{
				// different g
				GroupStarts[m][gid] = 0;
				GroupEnds[m][gid] = 0;
			}
		}
		else
		{
			// previous is null (the only item in queue), after leave, empty the queue
			GroupEnds[m][gid] = 0;
			GroupStarts[m][gid] = 0;
			head[m] = 0;
			tail[m] = 0;
		}
	}
}



int main()
{
	char c1[6];
	int m, j, i;

	FILE* ptr = 0;

	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW1\\HW1\\hw1_testdata\\P5\\2.in", "r");
		int r = fscanf(ptr, "%d %d %d", &WCNumber, &SituationNumber, &GroupNumber);
	}
	else
	{
		int r = scanf("%d %d %d", &WCNumber, &SituationNumber, &GroupNumber);
	}


	head = malloc(sizeof(node*) * WCNumber);
	tail = malloc(sizeof(node*) * WCNumber);
	GroupEnds = malloc(sizeof(node**) * WCNumber);
	GroupStarts = malloc(sizeof(node**) * WCNumber);

	closed = malloc(sizeof(char) * WCNumber);


	for (int k = 0; k < WCNumber; k++)
	{
		head[k] = 0;
		tail[k] = 0;
		closed[k] = 'n';
		GroupStarts[k] = malloc(sizeof(node*) * GroupNumber);
		GroupEnds[k] = malloc(sizeof(node*) * GroupNumber);

		for (int j = 0; j < GroupNumber; j++)
		{
			GroupStarts[k][j] = 0;
			GroupEnds[k][j] = 0;
		}
	}


	for (int k = 0; k < SituationNumber; k++)
	{
		if (FileFlag)
		{
			int re = fscanf(ptr, "%s", c1);
		}
		else
		{
			int re = scanf("%s", c1);
		}

		switch (c1[0])
		{
		case('e'):
			if (FileFlag)
			{
				int res = fscanf(ptr, "%d %d %d", &i, &j, &m);
			}
			else
			{
				int res = scanf("%d %d %d", &i, &j, &m);
			}
			enteringroup(i, j, m);
			break;
		case('l'):
			if (FileFlag)
			{
				int res = fscanf(ptr, "%d", &m);
			}
			else
			{
				int res = scanf("%d", &m);
			}
			leave(m);
			break;
		case('g'):
			if (FileFlag)
			{
				int res = fscanf(ptr, "%d", &m);
			}
			else
			{
				int res = scanf("%d", &m);
			}
			go(m);
			break;
		case('c'):
			if (FileFlag)
			{
				int res = fscanf(ptr, "%d", &m);
			}
			else
			{
				int res = scanf("%d", &m);
			}
			closeingroup(m);
			break;
		default:
			break;
		}

	}

	node* tmpptr;
	for (int k = 0; k < WCNumber; k++)
	{
		tmpptr = head[k];
		while (tmpptr)
		{
			printf("%d ", tmpptr->ID);
			tmpptr = tmpptr->next;
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