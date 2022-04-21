#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "price.h"
#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
#pragma warning(disable:6011)
#pragma warning(disable:4101)

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

int A, Q, N, ES, k, r, StockSN;
node** StartOfInactivateNode;
int kcnt = 0;
int maxk = 0;
int* StockSerialNumber;
Qnode* Query;
//unsigned long long** StockPrice;
node** PriceHeap;
node** PriceHeapExtra;
unsigned long long* SortedPrice;
unsigned long long* SortedPriceExtra;

unsigned long long Price(int s, int t)
{
	if (PriceFlag == 1)
	{
		return price(s, t);
	}
	else
	{
		return (unsigned long long)(s * t);
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
			// Only left child exist
			if (tmp->value >= PriceHeap[2 * current + 1]->value)
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


void buildPriceHeap(int cur)
{
	tmp = 0;
	if (cur < A * N)
	{
		PriceHeap[cur] = malloc(sizeof(node));
		PriceHeap[cur]->stock_id = StockSerialNumber[cur / N];
		PriceHeap[cur]->seq_id = cur % N + 1;
		PriceHeap[cur]->value = Price(PriceHeap[cur]->stock_id, PriceHeap[cur]->seq_id);

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


void buildSortedPrice()
{
	for (int i = 0; i < maxk; i++)
	{
		SortedPrice[i] = PriceHeap[0]->value;
		PriceHeap[0]->value = Price(PriceHeap[0]->stock_id, PriceHeap[0]->seq_id + N);
		PriceHeap[0]->seq_id = PriceHeap[0]->seq_id + N;
		MinHeapify(0);
	}
}


// We need to find the smallest or particularly extent of small across all stocks
int main()
{

	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW2\\HW2\\hw2_testdata\\P5\\2.in", "r");
		r = fscanf(ptr, "%d %d %d", &A, &Q, &N);
	}
	else
	{
		r = scanf("%d %d %d", &A, &Q, &N);
	}


	// There may be extra stock
	StockSerialNumber = malloc(sizeof(int) * (A + 1));
	PriceHeap = malloc(sizeof(node*) * A * N);
	PriceHeapExtra = malloc(sizeof(unsigned long long) * (A + 1) * N);
	Query = malloc(sizeof(Qnode) * Q);


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

	// Extra stock assign -1 to distingusih extra or not
	StockSerialNumber[A] = -1;
	
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

	buildPriceHeap(0);

	if (DebugFlag)
	{
		PrintArray(0);
	}

	SortedPrice = malloc(sizeof(unsigned long long) * maxk);
	buildSortedPrice();

	if (DebugFlag)
	{
		//PrintArray(1);
	}

	// deal with queries
	for (int i = 0; i < Q; i++)
	{

		if (Query[i].ES == 0)
		{
			// minus 1 to get the index
			printf("%llu\n", SortedPrice[Query[i].k-1]);
		}
		else
		{

			// Build extra stock
			StockSerialNumber[A] = ES;
			// At the end of extra query, clear extra stock
			if (ES != 0)
			{
				// clear extra stock
				StockSerialNumber[A] = -1;
			}
		}
	}


	if (FileFlag && ptr != NULL)
	{
		fclose(ptr);
	}
	return 0;
}

#pragma   warning(pop)  