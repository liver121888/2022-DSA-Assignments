#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


typedef struct node
{
	int priority;
	int id;
	struct node* parent;
	struct node* left;
	struct node* right;
} Node;

 
int N, Q;
Node* root;
Node* temp;
Node* left, * right, *current;
void insertTempToTreap()
{
	if (root)
	{
		if (root->id <= temp->id)
		{
			// Left and middle
			if( temp->id == root->id && temp->priority >= root->priority)
			{
				// set temp the root
				left = root;
				left->parent = temp;
				temp->left = left;
			}
			else
			{
				current = root;
				do
				{

				} while ();
				// set temp to left of root
				left = root->left;
				left->parent = temp;
				root->left = temp;
				temp->parent = root;
				temp->left = left;
			}
		}
		else
		{

		}

	}
	else root = temp;
}
void main()
{
	scanf("%d %d %d", &N, &Q );
	// Create N nodes associated with random priority
	for (int i = 1; i <= N; i++)
	{
		temp = malloc(sizeof(Node));
		temp->id = i;
		temp->priority = rnd();
		temp->left = temp->right = temp->parent = 0;
		insertTempToTreap();
	}

	// allocate a chuck of contijous memory.
	all = malloc(sizeof(char) * number * size);
	subString = malloc(sizeof(char*) * number);
	values = malloc(sizeof(unsigned long long) * number);
	// Module value set
	module = pow(2, 8 * sizeof(int)) / range;
	RangeConstants = malloc(sizeof(unsigned long long) * size);
	for (int j = 0; j < size; j++)
		RangeConstants[j] = (int)(pow(range, j));
	masks = malloc(sizeof(unsigned long long) * size);
	for (int i = 0; i < size; i++)
	{
		masks[i] = 0;
		for (int j = 0; j < size; j++)
		{
			if (j == i) continue;
			masks[i] += (range - 1) * RangeConstants[j];

			//masks[i] += ((range - 1) * RangeConstants[j]) % module;
			// masks[i] = masks[i] % module;
		}
	}

	for (int i = 0; i < number; i++)
	{
		subString[i] = all + i * size;
		scanf("%s", subString[i]);
		values[i] = 0;
		for (int j = 0; j < size; j++)
		{
			char c = subString[i][j] - low;
			values[i] += c * RangeConstants[j];

			//values[i] += ( c * RangeConstants[j] ) % module;
			//values[i] = values[i] % module;
		}
	}

	int yesNo = 0;
	int similarCount = 0;
	int done = 0;
	for (int i = 0; i < number; i++)
	{
		for (int j = i + 1; j < number; j++)
		{
			unsigned long long xorResult = values[i] ^ values[j];
			if (xorResult == 0)
			{
				if (flag == 0)
				{
					if (yesNo == 0)
					{
						printf("Yes\n");
						yesNo = 1;
					}
					printf("%d %d\n", i, j);
					done = 1;
					break;
				}
				else similarCount++;
			}
			else
			{
				// Check whether only one character is different
				int onlyOneCharacterIsDifferent = 0;
				for (int k = 0; k < size; k++)
				{
					if ((xorResult & masks[k]) == 0)
					{
						onlyOneCharacterIsDifferent = 1;
						break;
					}
				}
				if (onlyOneCharacterIsDifferent == 1)
				{
					if (flag == 0)
					{
						if (yesNo == 0)
						{
							printf("Yes\n");
							yesNo = 1;
						}
						printf("%d %d\n", i, j);
						done = 1;
						break;
					}
					else similarCount++;
				}
			}
		}
		if (done)break;
	}
	if (flag == 0 && yesNo == 0) printf("No\n");
	else if (flag == 1)
	{
		if (similarCount == 0)printf("No\n");
		else printf("Yes\n%d\n", similarCount);
	}

	//printf("\n");

	//char answers[80];
	//scanf("%s", answers);
	//printf(answers);
	//printf("\n");
	//scanf("%s", answers);
	//printf(answers);
	//printf("\n");

}