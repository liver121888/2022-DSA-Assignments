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
	int seq_id;
} node;

typedef struct Snode
{
	int stock_id;
	node** nodeHeap;
}Snode;

int FileFlag = 1;
int DebugFlag = 0;

int A, Q, N, ES, k, r;
int maxk = 0;
int* StockSerialNumber;
node* tmp;
Snode* Stmp;

Qnode* Query;
Snode** stockHeapArray;
node** extraHeapArray;
node** extraHeapArrayForBinary;

unsigned long long* SortedPrice;
int heaplen;
unsigned long long SortedPriceExtra1, SortedPriceExtra2;


void PrintArray(int select)
{
	switch (select)
	{
		//case(0):
		//	// Print PriceHeap
		//	printf("PriceHeap (value, stock_id, seq_id): \n");
		//	for (int i = 0; i < A * N; i++)
		//	{
		//		printf("(%llu, %d, %d), ", PriceHeap[i]->value, PriceHeap[i]->stock_id, PriceHeap[i]->seq_id);
		//	}
		//	printf("\n");
		//	break;
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
		printf("extraHeapArray (value, seq_id): \n");
		for (int i = 0; i < heaplen; i++)
		{
			printf("(%llu, %d), ", extraHeapArrayForBinary[i]->value, extraHeapArrayForBinary[i]->seq_id);
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

int parentID;
int childID;
void NodeMinHeapify(node** heap)
{
	for (int parent = N / 2; parent >= 0; parent--)
	{
		parentID = parent;
		childID = 2 * parent + 1;
		while (childID < N)
		{
			if (childID + 1 < N && heap[childID + 1]->value < heap[childID]->value)
				childID++;
			if (heap[parentID]->value <= heap[childID]->value)
				break;
			else
			{
				tmp = heap[parentID];
				heap[parentID] = heap[childID];
				heap[childID] = tmp;
				parentID = childID;
				childID = parentID * 2 + 1;
			}
		}
	}
}

void constructSnodeHeap()
{
	for (int parent = A / 2; parent >= 0; parent--)
	{
		parentID = parent;
		childID = parent * 2 + 1;

		while (childID < A)
		{
			if (childID + 1 < A && stockHeapArray[childID + 1]->nodeHeap[0]->value < stockHeapArray[childID]->nodeHeap[0]->value)
				childID++;
			if (stockHeapArray[parentID]->nodeHeap[0]->value <= stockHeapArray[childID]->nodeHeap[0]->value)
				break;
			else
			{
				Stmp = stockHeapArray[parentID];
				stockHeapArray[parentID] = stockHeapArray[childID];
				stockHeapArray[childID] = Stmp;
				parentID = childID;
				childID = parentID * 2 + 1;
			}
		}

	}
}

void buildSortedPrice()
{
	SortedPrice[0] = stockHeapArray[0]->nodeHeap[0]->value;
	for (int k = 1; k < maxk; k++)
	{
		stockHeapArray[0]->nodeHeap[0]->seq_id += N;
		stockHeapArray[0]->nodeHeap[0]->value = price(stockHeapArray[0]->stock_id, stockHeapArray[0]->nodeHeap[0]->seq_id);

		parentID = 0;
		childID = 1;
		// Heapify the priceheap within the stock node
		while (childID < N)
		{
			if (childID + 1 < N && stockHeapArray[0]->nodeHeap[childID + 1]->value < stockHeapArray[0]->nodeHeap[childID]->value)
				childID++;
			if (stockHeapArray[0]->nodeHeap[parentID]->value <= stockHeapArray[0]->nodeHeap[childID]->value)
				break;
			else
			{
				tmp = stockHeapArray[0]->nodeHeap[parentID];
				stockHeapArray[0]->nodeHeap[parentID] = stockHeapArray[0]->nodeHeap[childID];
				stockHeapArray[0]->nodeHeap[childID] = tmp;
				parentID = childID;
				childID = parentID * 2 + 1;
			}
		}
		// Heapify entire stockHeap
		parentID = 0;
		childID = 1;
		while (childID < A)
		{
			if (childID + 1 < A && stockHeapArray[childID + 1]->nodeHeap[0]->value < stockHeapArray[childID]->nodeHeap[0]->value)
				childID++;
			if (stockHeapArray[parentID]->nodeHeap[0]->value <= stockHeapArray[childID]->nodeHeap[0]->value)
				break;
			else
			{
				Stmp = stockHeapArray[parentID];
				stockHeapArray[parentID] = stockHeapArray[childID];
				stockHeapArray[childID] = Stmp;
				parentID = childID;
				childID = parentID * 2 + 1;
			}
		}
		SortedPrice[k] = stockHeapArray[0]->nodeHeap[0]->value;
	}
}


void ExtraMinHeapify()
{
	for (int parent = heaplen / 2; parent >= 0; parent--)
	{
		parentID = parent;
		childID = 2 * parent + 1;
		while (childID < heaplen)
		{
			if (childID + 1 < heaplen && extraHeapArrayForBinary[childID + 1]->value < extraHeapArrayForBinary[childID]->value)
				childID++;
			if (extraHeapArrayForBinary[parentID]->value <= extraHeapArrayForBinary[childID]->value)
				break;
			else
			{
				tmp = extraHeapArrayForBinary[parentID];
				extraHeapArrayForBinary[parentID] = extraHeapArrayForBinary[childID];
				extraHeapArrayForBinary[childID] = tmp;
				parentID = childID;
				childID = parentID * 2 + 1;
			}
		}
	}
}


int x;
int down, bound;
unsigned long long binarySeach()
{
	int l = 0;
	int r = k;
	int m = 0;
	while (l < r - 1)
	{
		m = (l + r) / 2;
		x = k - m;
		// heap size should be 2N
		heaplen = 0;

		if (x - N < 0)
		{
			down = 0;
		}
		else
		{
			down = x - N;
		}
		heaplen = 2 * N;

		for (int p = 0; p < heaplen; p++)
		{
			// first assign non-sorted heap array 
			extraHeapArrayForBinary[p]->seq_id = p + 1 + down; // 1 + p * increasePeriod;
			extraHeapArrayForBinary[p]->value = price(StockSerialNumber[A], extraHeapArrayForBinary[p]->seq_id);
		}
		// heapify
		ExtraMinHeapify();

		if (DebugFlag)
		{
			PrintArray(2);
		}

		if (x - N < 0)
			bound = x;
		else
			bound = N;

		for (int i = 0; i < bound; i++)
		{
			SortedPriceExtra1 = extraHeapArrayForBinary[0]->value;
			// here we have to copy value, we cannot directly point to it
			extraHeapArrayForBinary[0]->seq_id = extraHeapArrayForBinary[heaplen - 1]->seq_id;
			extraHeapArrayForBinary[0]->value = price(StockSerialNumber[A], extraHeapArrayForBinary[0]->seq_id);
			heaplen--;
			ExtraMinHeapify();
		}
		SortedPriceExtra2 = extraHeapArrayForBinary[0]->value;

		if (DebugFlag)
		{
			PrintArray(3);
			printf("x - N: %d\n", x - N);
			printf("A[m-1]: %llu, A[m]: %llu\n", SortedPrice[m - 1], SortedPrice[m]);
		}
		// SortedPriceExtra1 = s[k-m], SortedPriceExtra2 = s[k-m+1]
		if (SortedPriceExtra1 >= SortedPrice[m - 1])
		{
			if (SortedPriceExtra1 <= SortedPrice[m])
			{
				return SortedPriceExtra1;
			}
			else
				l = m;
		}
		else if (SortedPrice[m - 1] >= SortedPriceExtra1)
		{
			if (SortedPrice[m - 1] <= SortedPriceExtra2)
			{
				return SortedPrice[m - 1];
			}
			else
				r = m;
		}
	}
	if (SortedPrice[m] < SortedPriceExtra1)
	{
		return SortedPrice[k - 1];
	}
	if (SortedPrice[m - 1] > SortedPriceExtra2)
	{
		return SortedPriceExtra1;
	}
	return 0;
}



int main()
{

	FILE* ptr = 0;
	if (FileFlag)
	{
		ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW2\\HW2\\hw2_testdata\\P5\\6.in", "r");
		r = fscanf(ptr, "%d %d %d", &A, &Q, &N);
	}
	else
	{
		r = scanf("%d %d %d", &A, &Q, &N);
	}

	StockSerialNumber = malloc(sizeof(int) * (A + 1));

	for (int i = 0; i < A; i++)
	{
		if (FileFlag)
		{
			r = fscanf(ptr, "%d", &StockSerialNumber[i]);
		}
		else
		{
			r = scanf("%d", &StockSerialNumber[i]);
		}
	}

	Query = malloc(sizeof(Qnode) * Q);

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


	stockHeapArray = malloc(sizeof(Snode*) * A);


	for (int s = 0; s < A; s++)
	{
		stockHeapArray[s] = malloc(sizeof(Snode));
		stockHeapArray[s]->stock_id = StockSerialNumber[s];
		stockHeapArray[s]->nodeHeap = malloc(N * sizeof(node*));
		for (int p = 0; p < N; p++)
		{
			stockHeapArray[s]->nodeHeap[p] = malloc(sizeof(node));
			stockHeapArray[s]->nodeHeap[p]->seq_id = p + 1;
			stockHeapArray[s]->nodeHeap[p]->value = price(stockHeapArray[s]->stock_id, stockHeapArray[s]->nodeHeap[p]->seq_id);
		}
		// Construct nodeHeap
		NodeMinHeapify(stockHeapArray[s]->nodeHeap);
	}

	// for FC
	extraHeapArray = malloc(N * sizeof(node*));
	for (int i = 0; i < N; i++)
	{
		extraHeapArray[i] = malloc(sizeof(node));
	}

	// for Binary
	extraHeapArrayForBinary = malloc(2 * N * sizeof(node*));
	for (int i = 0; i < 2 * N; i++)
	{
		extraHeapArrayForBinary[i] = malloc(sizeof(node));
	}


	SortedPrice = malloc(sizeof(unsigned long long) * maxk);
	constructSnodeHeap();
	buildSortedPrice();



	if (DebugFlag)
	{
		PrintArray(1);
	}

	int kIdx;
	unsigned long long value;
	unsigned long long prevValue;
	unsigned long long extraValue;
	unsigned long long answer;

	// deal with queries
	for (int i = 0; i < Q; i++)
	{
		// no extra stock
		if (Query[i].ES == 0)
		{
			// minus 1 to get the index
			printf("%llu\n", SortedPrice[Query[i].k - 1]);
		}
		else
		{
			if (N < 600)
			{
				// Build extra stock
				StockSerialNumber[A] = Query[i].ES;
				k = Query[i].k;
				printf("%llu\n", binarySeach());
				//// Clear extra stock, SortedPriceExtra?
			}
			else
			{
				for (int p = 0; p < N; p++)
				{
					// first assign non-sorted heap array 
					extraHeapArray[p]->seq_id = p + 1; // 1 + p * increasePeriod;
					extraHeapArray[p]->value = price(Query[i].ES, extraHeapArray[p]->seq_id);
				}

				for (int parent = N / 2; parent >= 0; parent--)
				{
					parentID = parent;
					childID = parentID * 2 + 1;

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

				kIdx = Query[i].k - 1;
				value = SortedPrice[kIdx];
				// next value is in sequence previous
				prevValue = SortedPrice[kIdx - 1];
				extraValue = extraHeapArray[0]->value;

				while (1)
				{
					if (extraValue >= value)
					{
						answer = value;
						break;
					}
					else if ((kIdx > 0 && extraValue >= prevValue && extraValue < value) || (kIdx == 0 && extraValue < value))
					{
						answer = extraValue;
						break;
					}
					else
					{
						// Lower down one item
						kIdx--;
						value = SortedPrice[kIdx];
						if (kIdx > 0)
							prevValue = SortedPrice[kIdx - 1];

						// extra heap root upgrade 
						// The smallest node upgrades to next value
						extraHeapArray[0]->seq_id += N; // day id jump to next increased day
						unsigned long long v = price(Query[i].ES, extraHeapArray[0]->seq_id); // get upgraded value
						extraHeapArray[0]->value = v; // update value

						parentID = 0; // The root is upgraded
						childID = 1;
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
	}


	if (FileFlag && ptr != NULL)
	{
		fclose(ptr);
	}
	return 0;
}

#pragma   warning(pop)  