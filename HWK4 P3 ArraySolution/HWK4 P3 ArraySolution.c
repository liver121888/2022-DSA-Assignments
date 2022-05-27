#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>



typedef struct state
{	
	// Element indexed from 0 to N-1; Group indexed from 1 to N
	int* groupIds; // Length N;  1 <= value (group id) <= N
	int* elements; // Length N; 0 <= value (element id) <= N-1
	int* bounds; // Length N+1; bounds[i-1] the start index in elements array of group i; bounds[i] the end index limit (excluded) of group i.
	int groupCount; // Number of groups
} State;


typedef struct operation
{
	State* dayState;
	int first, second; // first = -1 --> print; first = -2 --> boom (second = day);  first, second > 0 --> merge
} Operation;

int N, M;
char cmd[20];
// Preview and store the commands for later execution
Operation* commandArray;


void main()
{
	int boomDay;

	scanf("%d %d", &N, &M);

	commandArray = malloc(sizeof(Operation) * (M + 1));
	commandArray[0].first = -1;
	commandArray[0].dayState = 0;

	// Preview and store the commands
	for (int day = 1; day <= M; day++)
	{
		// By default set this day isn't boom target; the container memory is null.
		// If a later day fires a boom to this day, the container memory should be 
		// allocated first. The detailed data memory will be allocated when this
		// day is actually executed
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
				// Allocate container memory of the state memory;
				// When this boomDay is actually executed, if this pointer is not null
				// this day is a boom target; detailed data memory will be allocated by then 
				commandArray[boomDay].dayState = malloc(sizeof(State)	);
			}
			break;
		}
	}

	// Allocate the initial state memory
	State * currentState = malloc(sizeof(State));
	currentState->groupIds = malloc(sizeof(int) * N);
	for (int i = 0; i < N; i++) currentState->groupIds[i] = i + 1;
	currentState->elements = malloc(sizeof(int) * N);
	for (int i = 0; i < N; i++) currentState->elements[i] = i;
	currentState->bounds = malloc(sizeof(int) * (N + 1));
	for (int i = 0; i < N + 1; i++) currentState->bounds[i] = i;
	currentState->groupCount = N; // N groups
	// Allocate temp array for merge operation to temporary store slaves
	int* buffer = malloc(sizeof(int) * N);

	// Start executing the stored commands
	for (int day = 1; day <= M; day++)
	{
		// Before stepping in this new day Check last day day-1 to see if it is a boom 
		// target (including day 0). If it is, allocated memory for detailed data and  
		// copy values of current state to them.
		State* yesterdayState = commandArray[day - 1].dayState;
		if (yesterdayState) // if yesterday state is not null, meaning the yesterday is a boom target
		{
			// copy current state as yesterday's state for booming restore
			yesterdayState->groupCount = currentState->groupCount; 
			yesterdayState->groupIds = malloc(sizeof(int) * N);
			for (int i = 0; i < N; i++) yesterdayState->groupIds[i] = currentState->groupIds[i] ;
			yesterdayState->elements = malloc(sizeof(int) * N);
			for (int i = 0; i < N; i++) yesterdayState->elements[i] = currentState->elements[i];
			// Group number keep reducing 
			yesterdayState->bounds = malloc(sizeof(int) * (yesterdayState->groupCount + 1));
			for (int i = 0; i < yesterdayState->groupCount + 1; i++) yesterdayState->bounds[i] = currentState->bounds[i];
		}		
		
		int master, slave, temp;
		// Execute command		
		switch (commandArray[day].first)
		{
		case -1: // query
			printf("%d\n", currentState->groupCount);
			break;

		case -2: // boom
			boomDay = commandArray[day].second;
			// Update current State values from stored dayState
			currentState->groupCount = commandArray[boomDay].dayState->groupCount;  
			for (int i = 0; i < N; i++) 
				currentState->groupIds[i] = commandArray[boomDay].dayState->groupIds[i];
			for (int i = 0; i < N; i++) 
				currentState->elements[i] = commandArray[boomDay].dayState->elements[i];
			for (int i = 0; i < currentState->groupCount + 1; i++)
				currentState->bounds[i] = commandArray[boomDay].dayState->bounds[i];
			break;

		default: // merge two elements
			master = currentState->groupIds[commandArray[day].first - 1];
			slave = currentState->groupIds[commandArray[day].second - 1];
			if (master == slave) break;
			// We alway let later group merged to previous group; slave merges to master
			if (master > slave) // Swap
			{
				temp = master;
				master = slave;
				slave = temp;
			}
			// 1. change group id of elements in slave group to master 
			for (int i = currentState->bounds[slave - 1]; i < currentState->bounds[slave]; i++)
				currentState->groupIds[currentState->elements[i]] = master;
			// 2. lower group id of elements in groups later than slave by one; since group slave is gone
			for (int i = currentState->bounds[slave]; i< N; i++)
				currentState->groupIds[currentState->elements[i]] --;
			// 3. move slave blocks to be adjacent to master block; 
			//    i.e., Move elements[slave-1]-elements[slave] to master-
			int slaveStart = currentState->bounds[slave-1], slaveEnd = currentState->bounds[slave];
			int numberOfSlaves = slaveEnd - slaveStart;
			int masterEnd = currentState->bounds[master];
			// 3.1 store slaves in buffer array
			for (int i = 0; i < numberOfSlaves; i++)
				buffer[i] = currentState->elements[slaveStart + i];
			// 3.2 backwardly move elements from master
			for (int i = slaveStart - 1; i >= masterEnd; i--)
				currentState->elements[i+numberOfSlaves] = currentState->elements[i];
			// 3.3 assign slaves right after masters
			for (int i = 0;  i < numberOfSlaves; i++)
				currentState->elements[masterEnd+i] = buffer[i];
			// 4. update bounds: from master to slave-1 = increased by number of slaves
			// from slave to current group count, update with next bounds; since slave group is removed
			for (int i = master; i < slave; i++)
				currentState->bounds[i] += numberOfSlaves;
			for (int i = slave; i < currentState->groupCount; i++)
				currentState->bounds[i] = currentState->bounds[i + 1];
			// 5. decrease group number by 1
			currentState->groupCount--;

			break;
		}
	}
}