#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct operand {
	struct operand* nxt;
	long long data;
}operand;

typedef struct operator {
	struct operator* nxt;
	char symbol;
}operator;

operand* opdhead;
operator* opthead;

long long num = -1;
char symbol;

void PushOperand(){
	operand* tmp = malloc(sizeof(operand));
	if (tmp != NULL)
	{
		tmp->nxt = opdhead;
		tmp->data = num;
		opdhead = tmp;
		//num = -1;
		num = -1;
	}

}

void PushOperator(){
	operator* tmp = malloc(sizeof(operator));
	if (tmp != NULL)
	{
		tmp->nxt = opthead;
		tmp->symbol = symbol;
		opthead = tmp;
	}
}


long long NegativeAndPositiveFloorDivision(long long a, long long b)
{
	//long long overflow, do not use a*b
	if (((a < 0 && b > 0) || (a > 0 && b < 0)) && a % b != 0)
	{
		return a / b - 1;
	}
	return a / b;
}


void CalculateOperation(int type) {

	while (1)
	{
		// pop
		char operation = opthead->symbol;
		opthead = opthead->nxt;

		if (operation == '(')
		{
			// here is the end of the stack
			break;
		}

		// pop
		long long value = opdhead->data;
		opdhead = opdhead->nxt;

		switch (operation)
		{
		case('+'):
			opdhead->data += value;
			break;
		case('-'):
			opdhead->data -= value;
			break;
		case('*'):
			opdhead->data *= value;
			break;
		case('/'):
			opdhead->data = NegativeAndPositiveFloorDivision(opdhead->data, value);
			//opdhead->data /= value;
			break;
		case('%'):
			opdhead->data = opdhead->data % value;
			break;
		default:
			break;
		}
		if (type == 0) break;
		else if(type == 1)
		{
			// to the butotm of the stack or '('
			if (opdhead == 0 || opthead == 0 || opthead->symbol == '(')
				break;
		}
		else if (type == 2)
		{
			// operate to previous '(', and remove '('
			if (opdhead == 0 || opthead == 0) break;
			if (opthead->symbol == '(')
			{
				// pop
				opthead = opthead->nxt;
				break;
			}

		}
	}
}


	

void Output()
{
	//operand* ptr = opdhead;
	//while (ptr != 0)
	//{
	//	printf("%d  ", (int)(ptr->data));
	//	ptr = ptr->nxt;
	//}
	if(opdhead != 0)
		printf("Print: %lld\n", opdhead->data);
}

int main()
{
	int c1length = 0;
	int tmp;
	char c1[16300];
	//int InitialNegative = 0;

	//int r = scanf("%s", c1);

	FILE* ptr;
	ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW1\\HW1\\hw1_testdata\\P4\\18.in", "r");
	int r = fscanf(ptr, "%s", c1);
	fclose(ptr);

	while (c1[c1length] != 0) c1length++;

	for (int i = 0; i < c1length; i++)
	{
		symbol = c1[i];
		tmp = symbol - '0';
		if (tmp < 0 || tmp > 9)
		{
			//if (i == 0 && symbol == '-')
			//{
			//	InitialNegative = 1;
			//	continue;
			//}
			// operator
			// finish reading num, push the num into the operand stacks
			//if (num >= 0)
			if (num != -1)
				PushOperand();
			switch (symbol)
			{
			case'(':
				PushOperator();
				break;
			case')':
				CalculateOperation(2);
				break;
			case'=':
				if (opthead != 0)
					CalculateOperation(1);
				Output();
				break;
			// same priority
			case'+':
			case'-':
				if (opthead != 0 && opthead->symbol != '(')
					CalculateOperation(1);
				PushOperator();
				break;

			// same priority
			case'*':
			case'/':
			case'%':
				if (opthead != 0 && (opthead->symbol == '*' || opthead->symbol == '/' || opthead->symbol == '%'))
					CalculateOperation(0);
				PushOperator();
				break;
			default:
				break;
			}
		}
		else
		{
			// operand
			if (num < 0)
			{

				num = tmp;
				//if (num == 0)
				//{

				//}
				//else
				//{
				//	//if (InitialNegative == 1)
				//	//	num = -tmp;
				//	//else
				//	//	num = tmp;
				//}
			}
			else
			{
				num = num * 10 + tmp;
			}

		}
	}
	return 0;
}