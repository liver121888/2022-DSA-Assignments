#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


// A single-linked list
typedef struct node
{
	struct node* groupNode;
	struct node* nextNode;
	struct node* end;
	int size;
} Node;


int N, M;
char cmd[20];
Node* nodeArray;


typedef struct command
{
	Node** dayState; // Where dayState[i] is the group leader element reference of element i
	int first, second; // first = -1 --> print; first = -2 --> boom (second = day);  first, second > 0 --> merge
} Command;

Command* commandArray;

void main()
{
	int boomDay;

	scanf("%d %d", &N, &M);

	commandArray = malloc(sizeof(Command) * (M + 1));
	commandArray[0].first = -1;
	commandArray[0].dayState = 0;

	// Preview and store the commands
	for (int day = 1; day <= M; day++)
	{
		// By default this day isn't boom target; the dateState is null.
		// If a later day fires a boom to this day, the memory is allocated by then.
		commandArray[day].dayState = 0;

		scanf("%s", cmd);
		switch (cmd[0])
		{
		case 'm': // merge
			scanf("%d %d", &commandArray[day].first, &commandArray[day].second);
			break;
		case 'q': // query
			commandArray[day].first = -1;
			break;
		case 'b': // boom
			commandArray[day].first = -2;
			scanf("%d", &boomDay);
			commandArray[day].second = boomDay;

			// Check whether the state memory is allocated?
			if (commandArray[boomDay].dayState == 0)
			{
				// this day is a boom target; detailed data memory will be allocated by then 
				commandArray[boomDay].dayState = 99;
			}
			break;
		}
	}


	// Prepare the working memory for state changing
	nodeArray = malloc(sizeof(Node) * N);
	for (int i = 0; i < N; i++)
	{
		// group head and this group list end are itself
		nodeArray[i].end = nodeArray[i].groupNode = &nodeArray[i];
		// No next node
		nodeArray[i].nextNode = 0;
		// Group size = 1
		nodeArray[i].size = 1;
	}

	int groupCount = N;
	int firstID, secondID;
	Node* master, * slave, * temp;

	// In addition to current state array, we keep a record of previously stored state
	// A state is an array of pointers to nodes.
	Node** previousStoredState = 0;

	// In the very beginning the state changed flag is 1
	// flag is set when merged or boom happened
	// flag will be reset when a new copy of current state is stored.
	// So if successive boom days does not change state, we will shallow copy the state
	int stateChanged = 1;

	// Start executing the stored commands
	for (int day = 1; day <= M; day++)
	{
		// Before stepping in this new day Check last day day-1 to see if it is a boom 
		// target (including day 0). If it is, allocated memory for current state data and  
		// copy values of current state to them.
		Node** yesterdayState = commandArray[day - 1].dayState;
		if (yesterdayState == 99) // if yesterday state is 99, meaning the yesterday is a boom target
		{
			if (stateChanged == 0 && previousStoredState)
			{
				// Directly shallow copy the previous stored state
				yesterdayState = previousStoredState;
			}
			else
			{
				// Yesterday was newly updated we need to allocate memory to store it
				// copy current state as yesterday's state for booming restore
				yesterdayState = malloc(sizeof(Node*) * (N + 1));
				for (int i = 0; i < N; i++)
					yesterdayState[i] = nodeArray[i].groupNode;
				// We hide the group number in the last addition element
				yesterdayState[N] = groupCount;
				// Record this is the newly created boom state
				previousStoredState = yesterdayState;
				// reset falg
				stateChanged = 0;
			}
			commandArray[day - 1].dayState = yesterdayState;
		}

		// Execute command		
		switch (commandArray[day].first)
		{
		case -1: // query
			stateChanged = 0;
			printf("%d\n", groupCount);
			break;

		case -2: // boom
			boomDay = commandArray[day].second;
			// Update current State values from stored dayState
			for (int i = 0; i < N; i++)
			{
				// pick up the group reference for element i
				nodeArray[i].groupNode = commandArray[boomDay].dayState[i];
				// reset size to 1
				nodeArray[i].size = 1;
				nodeArray[i].nextNode = 0;
				nodeArray[i].end = &nodeArray[i]; // itself
			}
			// Get the group size stored, we don't need to trace it
			groupCount = (int)(commandArray[boomDay].dayState[N]);

			// Reconnect the element list of each group.
			for (int i = 0; i < N; i++)
			{
				// The head of group do nothing
				if (nodeArray[i].groupNode == &nodeArray[i]) continue;
				// get the group head for this element; add this element to the end of the group
				master = nodeArray[i].groupNode;
				master->end->nextNode = &nodeArray[i];
				master->end = &nodeArray[i];
				master->size++;
			}
			// This day is updated; set flag
			stateChanged = 1;
			break;

		default: // merge two elements
			master = nodeArray[commandArray[day].first - 1].groupNode;
			slave = nodeArray[commandArray[day].second - 1].groupNode;
			if (master == slave) break; // Invalid merge do nothing
 
			// merge both groups; Swap them to make slave the small group; 
			if (master->size < slave->size)
			{
				temp = master;
				master = slave;
				slave = temp;
			}
			// set groupNode of each slave to master
			temp = slave;
			while (temp)
			{
				temp->groupNode = master;
				temp = temp->nextNode;
			}
			// update size of master
			master->size += slave->size;
			// attach slave to end of master;
			master->end->nextNode = slave;
			//slave->prevNode = master->end;
			master->end = slave->end;
			stateChanged = 1; // set flag
			groupCount--;
			break;
		}
	}


}