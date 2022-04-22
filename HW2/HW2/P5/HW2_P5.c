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
unsigned long long* SortedPrice;
unsigned long long SortedPriceExtra1;
unsigned long long SortedPriceExtra2;
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
		for (int i = 0; i < 2 * N; i++)
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

int down;
void assignExtraPriceHeap(int cur)
{
	if (cur < 2 * N)
	{
		// maybe can be move to outside
		PriceHeapExtra[cur] = malloc(sizeof(node));
		PriceHeapExtra[cur]->stock_id = StockSerialNumber[A];
		if (x-N < 0)
		{
			down = 0;
		}
		else
		{
			down = x - N;
		}
		
		PriceHeapExtra[cur]->seq_id = cur + 1 + down;
		PriceHeapExtra[cur]->value = Price(PriceHeapExtra[cur]->stock_id, PriceHeapExtra[cur]->seq_id);

		if (cur == 0)
		{
			cur++;
			assignExtraPriceHeap(cur);
		}
		else
		{
			// compare with root		
			heapSwapExtra(cur);
			assignExtraPriceHeap(cur + 1);
		}

	}
}



int bound = 0;
int heaplen = 0;
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

	for (int i = 0; i < bound; i++)
	{
		SortedPriceExtra1 = PriceHeapExtra[0]->value;
		PriceHeapExtra[0] = PriceHeapExtra[heaplen-1];
		heaplen--;
		MinHeapifyExtra(0);
	}
	SortedPriceExtra2 = PriceHeapExtra[0]->value;
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

unsigned long long binarySeach()
{
	int l = 0;
	int r = k;
	int m = 0;
	while (l < r - 1)
	{
		m = (l + r) / 2;
		x = k - m;
		assignExtraPriceHeap(0);
		if (DebugFlag)
		{
			//PrintArray(2);
		}
		assign2SortedPriceExtra();
		if (DebugFlag)
		{
			PrintArray(3);
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
}


// We need to find the smallest or particularly extent of small across all stocks
int main()
{

	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW2\\HW2\\hw2_testdata\\P5\\3.in", "r");
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
	PriceHeapExtra = malloc(sizeof(node*) * (2 * N));
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
		//PrintArray(0);
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
			StockSerialNumber[A] = Query[i].ES;
			printf("%llu\n", binarySeach());
			// Clear extra stock, SortedPriceExtra?
		}
	}


	if (FileFlag && ptr != NULL)
	{
		fclose(ptr);
	}
	return 0;
}

#pragma   warning(pop)  