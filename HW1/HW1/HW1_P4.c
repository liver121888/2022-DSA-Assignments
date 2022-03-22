#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH 260
#include <stdio.h>
#include <stdlib.h>

operand* opdhead;
operator* opthead;

long long num = -1;
char symbol;


typedef struct operand {
	operand* nxt;
	long long data;
}operand;

typedef struct operator {
	operator* nxt;
	char symbol;
}operator;


void PushOperand(){
	operand* tmp = malloc(sizeof(operand));
	tmp->nxt = opdhead;
	tmp->data = num;
	opdhead = tmp;
	num = -1;
}

void PushOperator(){
	operator* tmp = malloc(sizeof(operator));
	tmp->nxt = opthead;
	tmp->symbol = symbol;
	opthead = tmp;
	//symbol = 's';
}

void CalculateOperation(int type) {

	while (1)
	{
		char operation = opthead->symbol;
		opthead = opthead->nxt;
		if (operation == '(')
		{
			// why?
			break;
		}
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
			opdhead->data /= value;
			break;
		case('%'):
			opdhead->data = opdhead->data % value;
			break;
		default:
			break;
		}
	}


}

int main()
{
	FILE* ptr;
	int c1length = 0;
	char c1[MAX_LENGTH];
	int tmp;



	ptr = fopen("D:\\Senior_Spring\\DSA\\NTUCSIE-2022-DSA-Assignments\\HW1\\HW1\\hw1_testdata\\P4\\0.in", "r");
	scanf("%s", c1);
	fclose(ptr);

	while (c1[c1length] != 0) c1length++;

	for (size_t i = 0; i < c1length; i++)
	{
		symbol = c1[i];
		tmp = symbol - '0';
		if (tmp < 0 || tmp > 9)
		{
			// operator
			// finish reading num, push the num into the operand stacks
			if (num >= 0)
				PushOperand();
			switch (symbol)
			{
			case'(':
				PushOperator();
				break;
			case')':
				CalculateOperation(1);
				break;
			case'=':

				break;
			// same priority
			case'+':
			case'-':

				break;

			// same priority
			case'*':
			case'/':
			case'%':

				break;
			default:
				break;
			}
		}
		else
		{
			// operand
			if (i == 0)
				num = tmp;
			else
			{
				num = num + (long long)(tmp) * 10;
			}

		}
	}
	return 0;
}