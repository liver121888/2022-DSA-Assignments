#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "price.h"
#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)

typedef struct Qnode
{
	int k;
	int ES;
} Qnode;

typedef struct node
{
	unsigned long long value;
	int stock_id;
	int seq_id;
} node;

int FileFlag = 1;
int DebugFlag = 1;
int PriceFlag = 1;

int A, Q, N, ES, k, r, StockSN, x;
int maxk = 0;
int* StockSerialNumber;
Qnode* Query;
//unsigned long long** StockPrice;
node** PriceHeap;
node** PriceHeapExtra;
node** extraHeapArray;
unsigned long long* SortedPrice;
unsigned long long SortedPriceExtra1;
unsigned long long SortedPriceExtra2;
int heaplen = 0;
//unsigned long long* SortedPriceExtra;

unsigned long long Price(int s, int t)
{
	if (PriceFlag == 1)
	{
		return price(s, t);
	}
	else
	{
		return (unsigned long long)(s + t);
	}
}

node* tmp;
void MinHeapify(int current)
{
	tmp = PriceHeap[current];
	if (2 * current + 1 >= A * N)
	{
		// single root
		return;
	}
	else
	{
		// root with child
		if (2 * current + 2 >= A * N)
		{
			// Only left child exist, >= maybe
			if (tmp->value > PriceHeap[2 * current + 1]->value)
			{
				PriceHeap[current] = PriceHeap[2 * current + 1];
				PriceHeap[2 * current + 1] = tmp;
				MinHeapify(2 * current + 1);
			}
		}
		else
		{
			if (tmp->value >= PriceHeap[2 * current + 1]->value && PriceHeap[2 * current + 1]->value <= PriceHeap[2 * current + 2]->value)
			{
				// Both children exist, left is the smallest
				PriceHeap[current] = PriceHeap[2 * current + 1];
				PriceHeap[2 * current + 1] = tmp;
				MinHeapify(2 * current + 1);
			}
			else if (tmp->value >= PriceHeap[2 * current + 2]->value && PriceHeap[2 * current + 2]->value <= PriceHeap[2 * current + 1]->value)
			{
				// Both children exist, right is the smallest
				PriceHeap[current] = PriceHeap[2 * current + 2];
				PriceHeap[2 * current + 2] = tmp;
				MinHeapify(2 * current + 2);
			}
			else
			{
				// root is the smallest, do nothing
			}
		}
	}
}

void PrintArray(int select)
{
	switch (select)
	{
	case(0):
		// Print PriceHeap
		printf("PriceHeap (value, stock_id, seq_id): \n");
		for (int i = 0; i < A * N; i++)
		{
			printf("(%llu, %d, %d), ", PriceHeap[i]->value, PriceHeap[i]->stock_id, PriceHeap[i]->seq_id);
		}
		printf("\n");
		break;
	case(1):
		// Print SortedPrice
		printf("SortedPrice (value): \n");
		for (int i = 0; i < maxk; i++)
		{
			printf("%llu, ", SortedPrice[i]);
		}
		printf("\n");
		break;
	case(2):
		// Print PriceHeapExtra
		printf("PriceHeapExtra (value, stock_id, seq_id): \n");
		for (int i = 0; i < heaplen; i++)
		{
			printf("(%llu, %d, %d), ", PriceHeapExtra[i]->value, PriceHeapExtra[i]->stock_id, PriceHeapExtra[i]->seq_id);
		}
		printf("\n");
		break;
	case(3):
		// Print SortedPriceExtra
		printf("SortedPriceExtra1, SortedPriceExtra2: \n");
		printf("%llu, %llu", SortedPriceExtra1, SortedPriceExtra2);
		printf("\n");
		break;
	default:
		break;
	}
}

void heapSwap(int cur)
{
	// if current value is smaller than root, swap them
	if (PriceHeap[cur]->value < PriceHeap[(cur - 1) / 2]->value)
	{
		tmp = PriceHeap[cur];
		PriceHeap[cur] = PriceHeap[(cur - 1) / 2];
		PriceHeap[(cur - 1) / 2] = tmp;
		// recursive swap
		heapSwap((cur - 1) / 2);
	}
}
void heapSwapExtra(int cur)
{
	// if current value is smaller than root, swap them
	if (PriceHeapExtra[cur]->value < PriceHeapExtra[(cur - 1) / 2]->value)
	{
		tmp = PriceHeapExtra[cur];
		PriceHeapExtra[cur] = PriceHeapExtra[(cur - 1) / 2];
		PriceHeapExtra[(cur - 1) / 2] = tmp;
		// recursive swap
		heapSwapExtra((cur - 1) / 2);
	}
}


void buildPriceHeap(int cur)
{
	if (cur < A * N)
	{
		PriceHeap[cur] = malloc(sizeof(node));
		PriceHeap[cur]->stock_id = StockSerialNumber[cur / N];
		PriceHeap[cur]->seq_id = cur % N + 1;

		PriceHeap[cur]->value = price(PriceHeap[cur]->stock_id, PriceHeap[cur]->seq_id);

		if (cur == 0)
		{
			cur++;
			buildPriceHeap(cur);
		}
		else
		{
			// compare with root		
			heapSwap(cur);
			buildPriceHeap(cur + 1);
		}

	}
}

int down;
void assignExtraPriceHeap(int cur)
{
	//if (cur < 2 * N)
	//{
	//	// maybe can be move to outside
	//	PriceHeapExtra[cur] = malloc(sizeof(node));
	//	heaplen++;
	//	PriceHeapExtra[cur]->stock_id = StockSerialNumber[A];
	//	// can be move out
	//	if (x-N < 0)
	//	{
	//		down = 0;
	//	}
	//	else
	//	{
	//		down = x - N;
	//	}
	//	
	//	PriceHeapExtra[cur]->seq_id = cur + 1 + down;
	//	PriceHeapExtra[cur]->value = Price(PriceHeapExtra[cur]->stock_id, PriceHeapExtra[cur]->seq_id);

	//	if (cur == 0)
	//	{
	//		assignExtraPriceHeap(cur + 1);
	//	}
	//	else
	//	{
	//		// compare with root		
	//		heapSwapExtra(cur);
	//		assignExtraPriceHeap(cur + 1);
	//	}

	//}
}



int bound = 0;
void MinHeapifyExtra(int current)
{
	tmp = PriceHeapExtra[current];
	if (2 * current + 1 >= heaplen)
	{
		// single root
		return;
	}
	else
	{
		// root with child
		if (2 * current + 2 >= heaplen)
		{
			// Only left child exist, >= maybe
			if (tmp->value > PriceHeapExtra[2 * current + 1]->value)
			{
				PriceHeapExtra[current] = PriceHeapExtra[2 * current + 1];
				PriceHeapExtra[2 * current + 1] = tmp;
				MinHeapifyExtra(2 * current + 1);
			}
		}
		else
		{
			if (tmp->value >= PriceHeapExtra[2 * current + 1]->value && PriceHeapExtra[2 * current + 1]->value <= PriceHeapExtra[2 * current + 2]->value)
			{
				// Both children exist, left is the smallest
				PriceHeapExtra[current] = PriceHeapExtra[2 * current + 1];
				PriceHeapExtra[2 * current + 1] = tmp;
				MinHeapifyExtra(2 * current + 1);
			}
			else if (tmp->value >= PriceHeapExtra[2 * current + 2]->value && PriceHeapExtra[2 * current + 2]->value <= PriceHeapExtra[2 * current + 1]->value)
			{
				// Both children exist, right is the smallest
				PriceHeapExtra[current] = PriceHeapExtra[2 * current + 2];
				PriceHeapExtra[2 * current + 2] = tmp;
				MinHeapifyExtra(2 * current + 2);
			}
			else
			{
				// root is the smallest, do nothing
			}
		}
	}
}

void assign2SortedPriceExtra()
{
	if (x - N < 0)
		bound = x;
	else
		bound = N;
	heaplen = 2 * N;

	PrintArray(2);

	for (int i = 0; i < bound; i++)
	{
		SortedPriceExtra1 = PriceHeapExtra[0]->value;
		PriceHeapExtra[0] = PriceHeapExtra[heaplen-1];
		heaplen--;
		MinHeapifyExtra(0);
		PrintArray(2);
	}
	SortedPriceExtra2 = PriceHeapExtra[0]->value;
}



void buildSortedPrice()
{
	for (int i = 0; i < maxk; i++)
	{
		SortedPrice[i] = PriceHeap[0]->value;
		PriceHeap[0]->value = price(PriceHeap[0]->stock_id, PriceHeap[0]->seq_id + N);
		PriceHeap[0]->seq_id = PriceHeap[0]->seq_id + N;
		MinHeapify(0);
	}
}

unsigned long long binarySeach()
{
	int l = 0;
	int r = k;
	int m = 0;
	while (l < r - 1)
	{
		m = (l + r) / 2;
		x = k - m;
		// heap size should be 2N+2
		heaplen = 0;
		assignExtraPriceHeap(0);
		if (DebugFlag)
		{
			PrintArray(2);
		}
		assign2SortedPriceExtra();
		if (DebugFlag)
		{
			PrintArray(3);
			printf("x - N: %d\n", x - N);
			printf("A[m-1]: %llu, A[m]: %llu\n", SortedPrice[m-1], SortedPrice[m]);
		}
		// SortedPriceExtra1 = s[k-m], SortedPriceExtra2 = s[k-m+1]
		if (SortedPriceExtra1 >= SortedPrice[m-1])
		{
			if (SortedPriceExtra1 <= SortedPrice[m])
			{
				return SortedPriceExtra1;
			}
			else
				l = m;
		}
		else if (SortedPrice[m-1] >= SortedPriceExtra1)
		{
			if (SortedPrice[m-1] <= SortedPriceExtra2)
			{
				return SortedPrice[m-1];
			}
			else
				r = m;
		}
	}
	if (SortedPrice[m] < SortedPriceExtra1)
	{
		return SortedPrice[k - 1];
	}
	if (SortedPrice[m-1] > SortedPriceExtra1)
	{
		return SortedPriceExtra1;
	}
	return 0;
}

int checkExtraHeap()
{
	if (N == 1)
	{
		return 1;
	}
	for (int p = 1; p < N; p++)
	{
		if (extraHeapArray[p - 1] > extraHeapArray[p])
			return 0;
	}
	return 1;
}


int pID;
int cID;
void buildPriceHeapnonRecursive()
{

	int c = 0;
	for (int s = 0; s < A; s++)
		for (int d = 0; d < N; d++)
		{
			// Allocate memory for each node and complete the
			// first non-sorted heap array 
			PriceHeap[c] = malloc(sizeof(node));
			PriceHeap[c]->stock_id = StockSerialNumber[s];
			PriceHeap[c]->seq_id = d + 1; // 1 + p * increasePeriod;
			PriceHeap[c]->value = price(PriceHeap[c]->stock_id, PriceHeap[c]->seq_id);
			c++;
		}



	for (int parent = A * N / 2; parent >= 0; parent--)
	{
		pID = parent;
		cID = parent * 2 + 1;

		while (cID < A * N)
		{
			// pick the smallest child
			if (cID + 1 < A * N && PriceHeap[cID + 1]->value < PriceHeap[cID]->value)
			{
				cID++;
			}
			if (PriceHeap[pID]->value <= PriceHeap[cID]->value)
			{
				break;
			}
			else
			{
				tmp = PriceHeap[pID];
				PriceHeap[pID] = PriceHeap[cID];
				PriceHeap[cID] = tmp;
				pID = cID;
				cID = pID * 2 + 1;
			}
		}
	}
}

void buildSortedPriceNonRecursive()
{
	SortedPrice[0] = PriceHeap[0]->value;
	for (int k = 1; k < maxk; k++)
	{
		PriceHeap[0]->seq_id += N;
		PriceHeap[0]->value = price(PriceHeap[0]->stock_id, PriceHeap[0]->seq_id);

		pID = 0;
		cID = 1;
		while (cID < A * N)
		{
			if (cID + 1 < A * N && PriceHeap[cID+1]->value < PriceHeap[cID]->value)
			{
				cID++;
			}
			if (PriceHeap[pID]->value <= PriceHeap[cID]->value)
			{
				break;
			}
			else
			{
				tmp = PriceHeap[pID];
				PriceHeap[pID] = PriceHeap[cID];
				PriceHeap[cID] = tmp;
				pID = cID;
				cID = pID * 2 + 1;
			}
		}
		SortedPrice[k] = PriceHeap[0]->value;
	}


}



// We need to find the smallest or particularly extent of small across all stocks
int main()
{

	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW2\\HW2\\hw2_testdata\\P5\\4.in", "r");
		r = fscanf(ptr, "%d %d %d", &A, &Q, &N);
	}
	else
	{
		r = scanf("%d %d %d", &A, &Q, &N);
	}

	// There may be extra stock
	StockSerialNumber = malloc(sizeof(int) * (A + 1) * N);
	PriceHeap = malloc(sizeof(node*) * A * N);
	// k-m-N+2 < k-m+1 < k-m+N
	// k-m-N+1 < k-m < k-m+N-1
	// the thing we want would be the Nth smallest number in extra heap
	// it might be k-m+1 or k-m
	// so the size should be 2N+1
	//PriceHeapExtra = malloc(sizeof(node*) * (2 * N));
	Query = malloc(sizeof(Qnode) * Q);



	extraHeapArray = malloc(N * sizeof(node*));
	for (int i = 0; i < N; i++)
	{
		extraHeapArray[i] = malloc(sizeof(node));
	}


	for (int i = 0; i < A; i++)
	{
		if (FileFlag)
		{
			r = fscanf(ptr, "%d", &StockSN);
		}
		else
		{
			r = scanf("%d", &StockSN);
		}
		StockSerialNumber[i] = StockSN;
	}
	
	// read all queries
	for (int i = 0; i < Q; i++)
	{
		if (FileFlag)
		{
			r = fscanf(ptr, "%d %d", &ES, &k);
		}
		else
		{
			r = scanf("%d %d", &ES, &k);
		}
		if (k > maxk)
			maxk = k;
		Query[i].ES = ES;
		Query[i].k = k;
	}
	//buildPriceHeap(0);

	buildPriceHeapnonRecursive();

	if (DebugFlag)
	{
		PrintArray(0);
	}

	SortedPrice = malloc(sizeof(unsigned long long) * maxk);
	//buildSortedPrice();
	buildSortedPriceNonRecursive();

	if (DebugFlag)
	{
		PrintArray(1);
	}

	// deal with queries
	for (int i = 0; i < Q; i++)
	{
		// no extra stock
		if (Query[i].ES == 0)
		{
			// minus 1 to get the index
			printf("%llu\n", SortedPrice[Query[i].k-1]);
		}
		else
		{
			//// Build extra stock
			//StockSerialNumber[A] = Query[i].ES;
			//printf("%llu\n", binarySeach());
			//// Clear extra stock, SortedPriceExtra?

			for (int p = 0; p < N; p++)
			{
				// first non-sorted heap array 
				extraHeapArray[p]->stock_id = Query[i].ES;
				extraHeapArray[p]->seq_id = p + 1; // 1 + p * increasePeriod;
				extraHeapArray[p]->value = price(extraHeapArray[p]->stock_id, extraHeapArray[p]->seq_id);
			}

			for (int parent = N / 2; parent >= 0; parent--)
			{
				int parentID = parent;
				int childID = parentID * 2 + 1;

				while (childID < N) // child is traversed one by one
				{
					// Select the child with the smaller value
					if (childID + 1 < N && extraHeapArray[childID + 1]->value < extraHeapArray[childID]->value)
						childID++; // second child is smaller than first child
					if (extraHeapArray[parentID]->value <= extraHeapArray[childID]->value)
						break; // Done! Since the parent is smaller or equal to the smaller child
					else
					{
						// Let the smaller child turn parent, and downgrade the parent to the child
						// Swap child and parent
						tmp = extraHeapArray[parentID];
						extraHeapArray[parentID] = extraHeapArray[childID];
						extraHeapArray[childID] = tmp;
						// Since parent and child are swapped, we need to traverse down further
						parentID = childID;
						childID = parentID * 2 + 1;
					}
				}
			}
			if (DebugFlag)
			{
				printf("is heap valid?: %d", checkExtraHeap());
			}

			int bankIdx = Query[i].k - 1;
			unsigned long long value = SortedPrice[bankIdx];
			// next value is in sequence previous
			unsigned long long prevValue = SortedPrice[bankIdx-1];
			unsigned long long extraValue = extraHeapArray[0]->value;
			unsigned long long answer;

			while (1)
			{
				if (extraValue >= value)
				{
					answer = value;
					break;
				}
				else if ((bankIdx > 0 && extraValue >= prevValue && extraValue < value) ||
					(bankIdx == 0 && extraValue < value))
				{
					answer = extraValue;
					break;
				}
				else
				{
					// Lower down data bank one item
					bankIdx--;
					value = SortedPrice[bankIdx];
					if (bankIdx > 0)
						prevValue = SortedPrice[bankIdx - 1];

					// extra heap root upgrade 
					// The smallest node upgrades to next value
					extraHeapArray[0]->seq_id += N; // day id jump to next increased day
					unsigned long long v = price(extraHeapArray[0]->stock_id, extraHeapArray[0]->seq_id); // get upgraded value
					extraHeapArray[0]->value = v; // update value

					int parentID = 0; // The root is upgraded
					int childID = 1;
					// Top-down min heapify??
					while (childID < N) // child is traversed one by one
					{
						// Select the child with the smaller value
						if (childID + 1 < N && extraHeapArray[childID + 1]->value < extraHeapArray[childID]->value)
							childID++; // second child is smaller than first child
						if (extraHeapArray[parentID]->value <= extraHeapArray[childID]->value)
						{
							break; // Done! Since the parent is smaller or equal to the smaller child
						}
						else
						{
							// Let the smaller child turn parent, and downgrade the parent to the child
							// Swap child and parent
							tmp = extraHeapArray[parentID];
							extraHeapArray[parentID] = extraHeapArray[childID];
							extraHeapArray[childID] = tmp;
							// Since parent and child are swapped, we need to traverse down further
							parentID = childID;
							childID = parentID * 2 + 1;
						}
					}
					extraValue = extraHeapArray[0]->value;
				}
			}
			printf("%llu\n", answer);
		}
	}


	if (FileFlag && ptr != NULL)
	{
		fclose(ptr);
	}
	return 0;
}

#pragma   warning(pop)  