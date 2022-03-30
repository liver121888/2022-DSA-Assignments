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

node* XOR(node* a, node* b)
{
	node* n;
	n = (node*)((unsigned long)(a) ^ (unsigned long)(b));
	return (node*)((unsigned long)(a) ^ (unsigned long)(b));
}

node** head;
node** tail;

node*** GroupEnds, *** GroupEndNexts;
node*** GroupStarts, *** GroupStartPrevs; // used in Dismiss functions


char* closed;
int WCNumber, SituationNumber, GroupNumber;
int FileFlag = 1;
int PrintQueueInfoFlag = 0;


void PrintQueueInfo()
{
	node* ptr, * last;
	for (int q = 0; q < WCNumber; q++)
	{
		printf("\nQ%d: ", q);
		last = 0;
		ptr = head[q];
		while (ptr)
		{
			printf("%d(%d) ", ptr->ID, ptr->Group);
			node* next = XOR(ptr->link, last);
			last = ptr;
			ptr = next;
		}
		printf("       ##  ");
		for (int j = 0; j < GroupNumber; j++)
			printf(" G(%d) p %d s %d e %d n %d", j, GroupStartPrevs[q][j] ? GroupStartPrevs[q][j]->ID : -1, GroupStarts[q][j] ? GroupStarts[q][j]->ID : -1, GroupEnds[q][j] ? GroupEnds[q][j]->ID : -1, GroupEndNexts[q][j] ? GroupEndNexts[q][j]->ID : -1);
	}
}



void enteringroup(int g, int id, int m)
{
	node* n = malloc(sizeof(node));
	if (n != 0)
	{
		n->ID = id;
		n->Group = g;
	}
	node* groupEnd = GroupEnds[m][g];
	node* groupEndNext = GroupEndNexts[m][g];
	if (groupEnd)
	{
		// The group exist in this queue
		n->link = (unsigned int)groupEnd ^ (unsigned int)groupEndNext;
		groupEnd->link = ((unsigned int)groupEnd->link ^ (unsigned int)groupEndNext) ^ (unsigned int)n;


		if (groupEndNext)
		{
			groupEndNext->link = ((unsigned int)groupEndNext->link^ (unsigned int)groupEnd) ^ (unsigned int)n;
			GroupStartPrevs[m][groupEndNext->Group] = n;
		}
		else
			tail[m] = n;

		GroupEnds[m][g] = n;
		GroupEndNexts[m][g] = groupEndNext;
	}
	else
	{
		// This is the first guy of a new group enters this queue
		// Enter tail of the queue
		node* tailer = tail[m];
		if (tailer)
		{
			// tail exist
			n->link = (unsigned int)tailer ^ 0;
			tailer->link = ((unsigned int)tailer->link^0) ^ (unsigned int)n;
			GroupEndNexts[m][tailer->Group] = n;
			tail[m] = n;
		}
		else
		{
			// empty queue
			head[m] = n;
			tail[m] = n;
			n->link = 0;
		}
		GroupStarts[m][g] = n;
		GroupStartPrevs[m][g] = tailer;
		GroupEnds[m][g] = n;
		GroupEndNexts[m][g] = 0;
	}

	//PrintQueueInfo();
}



void closeingroup(int m)
{
	node* Qtail = 0;
	node* endGuy;
	node* endNext;
	node* startGuy;
	node* joinEnd;
	node* joinEndNext;
	Qtail = tail[m];

	int gid;
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
				Qtail = GroupStartPrevs[m][gid];

				endGuy = GroupEnds[m][gid];
				endNext = GroupEndNexts[m][gid];

				joinEnd = GroupEnds[k][gid];
				joinEndNext = GroupEndNexts[k][gid];

				startGuy = GroupStarts[m][gid];

				if (joinEnd)
				{
					// group exist, combine two segment
					joinEnd->link = (unsigned int)joinEnd->link^ (unsigned int)joinEndNext^ (unsigned int)endGuy;

					if (endGuy == startGuy)
					{
						// the segment just has one item; endguy is between joinEnd and joinEndNext
						endGuy->link = (unsigned int)joinEndNext ^ (unsigned int)joinEnd;
					}
					else
					{
						endGuy->link = (unsigned int)endGuy->link^(unsigned int)endNext^(unsigned int)joinEnd;
					}

					if (joinEndNext)
					{
						// different g, new prev is startGuy
						joinEndNext->link = (unsigned int)joinEndNext->link^ (unsigned int)joinEnd^ (unsigned int)startGuy;
						// update next g start prev
						GroupStartPrevs[k][joinEndNext->Group] = startGuy;
					}
					else
						tail[k] = startGuy;

					if (endGuy != startGuy)
					{
						startGuy->link = (unsigned int)startGuy->link^ (unsigned int)GroupStartPrevs[m][gid]^ (unsigned int)joinEndNext;
					}
					// group appended, update end and endNext
					GroupEnds[k][gid] = startGuy;
					GroupEndNexts[k][gid] = joinEndNext;
				}
				else
				{
					// The group is not in the k
					// append to tail of k
					joinEnd = tail[k];
					if (joinEnd)
					{
						joinEnd->link = (unsigned int)joinEnd->link^0^ (unsigned int)endGuy;
						GroupEndNexts[k][joinEnd->Group] = endGuy;

						if (endGuy == startGuy)
						{
							// The group just has one item; the new next will be 0; end is between joinEnd and 0
							endGuy->link = (unsigned int)joinEnd^0;
						}
						else
						{
							endGuy->link = (unsigned int)endGuy->link^ (unsigned int)endNext^ (unsigned int)joinEnd;
						}
						tail[k] = startGuy;

						if (endGuy != startGuy)
						{
							startGuy->link = (unsigned int)startGuy->link^ (unsigned int)GroupStartPrevs[m][gid]^(unsigned int)0;
						}

					}
					else
					{
						// join queue is empty
						// reverse
						head[k] = endGuy;
						tail[k] = startGuy;
						endGuy->link = (unsigned int)endGuy->link ^ 0;
						startGuy->link = (unsigned int)startGuy->link ^ (unsigned int)GroupStartPrevs[m][gid] ^ (unsigned int)0;
					}
					GroupStartPrevs[k][gid] = joinEnd;
					GroupStarts[k][gid] = endGuy;
					GroupEnds[k][gid] = startGuy;
					GroupEndNexts[k][gid] = NULL;
				}
			}
			break;
		}
	}

	for (int j = 0; j < GroupNumber; j++)
	{
		GroupEnds[m][j] = GroupStarts[m][j] = 0;
		GroupEndNexts[m][j] = GroupStartPrevs[m][j] = 0;
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
		nextGuy = (unsigned int)head[m]->link^ (unsigned int)NULL;
		if (nextGuy)
		{
			// nextGuy exist
			if (nextGuy->Group == gid)
			{
				GroupStarts[m][gid] = nextGuy;
				GroupStartPrevs[m][gid] = 0;
			}
			else
			{
				// different group
				GroupStartPrevs[m][gid] = 0;
				GroupStarts[m][gid] = 0;
				GroupEnds[m][gid] = 0;
				GroupEndNexts[m][gid] = 0;

				GroupStartPrevs[m][nextGuy->Group] = 0;
			}
			// ^ head first to get next then ^ 0 turns new head
			nextGuy->link = (unsigned int)NULL^((unsigned int)nextGuy->link^ (unsigned int)head[m]);
			head[m] = nextGuy;
		}
		else
		{
			GroupStartPrevs[m][gid] = 0;
			GroupEnds[m][gid] = 0;
			GroupStarts[m][gid] = 0;
			GroupEndNexts[m][gid] = 0;
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
		prevGuy = (unsigned int)tail[m]->link^(unsigned int)NULL;
		if (prevGuy)
		{
			// previous guy exits
			if (prevGuy->Group == gid)
			{
				// same g
				GroupEnds[m][gid] = prevGuy;
				GroupEndNexts[m][gid] = 0;
			}
			else
			{
				// different g
				GroupStartPrevs[m][gid] = 0;
				GroupStarts[m][gid] = 0;
				GroupEnds[m][gid] = 0;
				GroupEndNexts[m][gid] = 0;

				GroupEndNexts[m][prevGuy->Group] = 0;
			}
			// ^ tail get prev then ^ 0
			prevGuy->link = (unsigned int)prevGuy->link^ (unsigned int)tail[m]^ (unsigned int)NULL;
			tail[m] = prevGuy;
		}
		else
		{
			// previous is null (the only item in queue), after leave, empty the queue
			GroupStartPrevs[m][gid] = 0;
			GroupEnds[m][gid] = 0;
			GroupStarts[m][gid] = 0;
			GroupEndNexts[m][gid] = 0;
			head[m] = 0;
			tail[m] = 0;
		}
	}
}



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

	node* tmpptr, *last, *next;
	for (int k = 0; k < WCNumber; k++)
	{
		last = NULL;
		tmpptr = head[k];
		while (tmpptr)
		{
			printf("%d ", tmpptr->ID);
			next = (unsigned int)tmpptr->link^ (unsigned int)last;
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