#define _CRT_SECURE_NO_DEPRECATE

#include<stdio.h>
#include<stdlib.h>


int row, col;
int** board;
int** rowHints;
int** colHints;
int trial = 0;

void printBoard(int** a, int row, int col)
{
	for (int r = 0; r < row; r++)
	{

		for (int c = 0; c < col; c++)
		{
			switch (a[r][c])
			{
			case 1:
				printf("o");
				break;
			case 0:
				printf("-");
				break;
			case -1:
				printf("x");
				break;
			}
		}
		printf("\n");
	}
}

void printGlobalBoard()
{
	for (int r = 0; r < row; r++)
	{

		for (int c = 0; c < col; c++)
		{
			switch (board[r][c])
			{
			case 1:
				printf("o");
				break;
			case 0:
				printf("-");
				break;
			case -1:
				printf("x");
				break;
			}
		}
		printf("\n");
	}
}

int success()
{
	trial++;
	//printGlobalBoard();
	//printf("\n");
	int cnt, c, r, j;
	for (int r = 0; r < row; r++)
	{
		cnt = 0;
		c = 0;
		j = 0;
		do
		{
			while (c < col && board[r][c] == 0) c++;  // c < col checking must be in first place
			while (c < col && board[r][c] == 1) { c++;  cnt++; } // c < col checking must be in first place
			if (cnt > 0 && cnt != rowHints[r][j]) return 0; // fail
			cnt = 0;  j++;
		} while (c < col);
	}
	for (int c = 0; c < col; c++)
	{
		cnt = 0;
		r = 0;
		j = 0;
		do
		{
			while (r < row && board[r][c] == 0) r++; // r < row checking must be in the first place
			while (r < row && board[r][c] == 1) { r++; cnt++; } // r < row checking must be in the first place
			if (cnt > 0 && cnt != colHints[c][j]) return 0;
			cnt = 0; j++;
		} while (r < row);

	}

	return 1;
}

int BrutalSolve(int* board, int totalLength, int yourindex)
{
	// Try black first
	board[yourindex] = 1;
	if (yourindex == totalLength - 1)
	{
		// complete the board, check correctness;
		if (success() == 1) return 1;
	}
	else
	{
		if (BrutalSolve(board, totalLength, yourindex + 1))
			return 1;
	}

	board[yourindex] = 0;
	if (yourindex == totalLength - 1)
	{
		// complete the board, check correctness;
		if (success() == 1) return 1;
	}
	else
	{
		if (BrutalSolve(board, totalLength, yourindex + 1)) return 1;
	}
	return 0;
}

void main()
{
	for (int b = 0; b < 100; b++)
	{

		printf("\n\n*************** problem %d.int ***************\n", b );
		char fileName[] = "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\nonogram\\85.in";
		sprintf(fileName, "D:\\2022 GitHubProjects\\DataStructureAndAlgorithm2022\\HW0\\HW0\\hw0_testdata\\nonogram\\%d.in", b);

		FILE* fptr = fopen(fileName, "r");


		fscanf(fptr, "%d %d", &row, &col);

		int* all = malloc(row * col * sizeof(int*));

		board = malloc(row * sizeof(int*));

		rowHints = malloc(row * sizeof(int*));
		colHints = malloc(col * sizeof(int*));

		for (int r = 0; r < row; r++)
		{
			// board memory allocation of row r
			board[r] = &all[r * col];
			// initialize elements
			for (int c = 0; c < col; c++)
				board[r][c] = -1; // -1 empty, 0 white, 1 black

			// Read in row r hints count
			int num;
			fscanf(fptr, "%d", &num);
			// allocate memory for hints of row r
			rowHints[r] = malloc(num * sizeof(int));
			// Read in the num row hints
			for (int i = 0; i < num; i++)
				fscanf(fptr, "%d", &rowHints[r][i]);
		}


		for (int c = 0; c < col; c++)
		{
			int num;
			fscanf(fptr, "%d", &num);
			colHints[c] = malloc(num * sizeof(int));
			for (int i = 0; i < num; i++)
				fscanf(fptr, "%d", &colHints[c][i]);
		}
		fclose(fptr);

		//all[0] = 0; all[1] = 1; all[2] = 0; 
		//all[3] = 0; all[4] = 1; all[5] = 1; 
		//all[6] = 1; all[7] = 1; all[8] = 0; 
		//all[9] = 1; all[10] = 1; all[11] = 1; 
		//all[12] = 1; all[13] = 1; all[14] = 1; 
		//all[15] = 0; all[16] = 1; all[17] = 0;
		//all[18] = 1; all[19] = 0; all[20] = 1; 
		//all[21] = 1; all[22] = 0; all[23] = 0;

		//int ii = success();



		//printBoard (board, row, col);

		if (BrutalSolve(all, row * col, 0))
		{
			printf("Found!   Trial = %d\n", trial);
			printGlobalBoard();
		}
		else
		{
			printf("Failed!  Trial = %d\n", trial);
		}

	}


	int i;
	scanf("%d", &i);
}