#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

typedef struct num
{ 
	long long value; 
	struct num *next;
} num;

typedef struct opt
{
	char op;
	struct opt *next;
} opt;


// Global data
opt *Operator = 0;
num *Operand = 0;
long long Number = -1;
long long Answer = 0;
char Symbol;
char* EXP;



void PushOperatorStackWithSymbol()
{
	// The operand value is completely read; Push operand value to operand stack
	opt* oo = malloc(sizeof(opt));
	oo->op = Symbol;
	oo->next = Operator;
	Operator = oo;
}


void PushOperandStackWithNumber( )
{
	// The operand value is completely read; Push operand value to operand stack
	num *vv = malloc(sizeof(num));
	vv->value = Number;
	vv->next = Operand;
	Operand = vv;
	Number = -1;
}

void EvalueTopOperatorAndOperands( int EvaluateToID )
{
	do
	{
		// Pop out top operator
		char op = Operator->op;
		Operator = Operator->next;
		if (op == '(') break;

		// Pop out top operand
		long long v1 = Operand->value;
		Operand = Operand->next;

		switch (op)
		{
		case '+':
			Operand->value += v1;
			break;
		case '-':
			Operand->value -= v1;
			break;
		case '*':
			Operand->value *= v1;
			break;
		case '/':
			Operand->value /= v1;
			break;
		case '%':
			Operand->value = Operand->value % v1;
			break;
		default:
			break;
		}

		if (EvaluateToID == 0) break; // Only the top expression
		else  
		{
			if (Operand == 0 || Operator == 0) break;
			if (EvaluateToID == 2)// to Left Paraphesis;
			{
				if (Operator->op == '(')
				{
					Operator = Operator->next;
					break;
				}
			}
		}

	} while (1);
 
}

void PrintOperandStack()
{
	num* ptr = Operand;
	printf("%s ", EXP);
	while (ptr != 0)
	{
		printf("%d  ", ptr->value);
		ptr = ptr->next;
	}
}

void Print()
{
	num* ptr = Operand;
	printf("%s ", EXP);
	while (ptr != 0)
	{
		printf("%d  ", ptr->value);
		ptr = ptr->next;
	}
	printf("Print: %d\n", Operand->value);
}


void main()
{
	char* expression[80];
	char* c = expression;
	c = "5+4-3*8/8%2-5+8 =";//   / 8 % 2 - 5 + 8 = "; // 11
	c = "(5+4)=";
	c = "((5+4))=";
	c = "((((5+4))))=";
	c = "(5+4)*(5*(3-2*3))=";

	c = "1+2=*5+(1+7+11)=%5-11="; //  // Print: 3, Print: 34, Print: -7 
//	c = "1+3-2*4="; // Print: -4
//	c = "1+2*(3+(8-6/3)*5)="; // Print: 67
//	c = "1+(2*3*(1+2+3*4/2*7+(1+8))+4)=/(11/5)+7-19*2="; // Print: 329, Print: 133
	c = "1+(2*3*(1+2+3*4/2*7+(1+8))+4)=";

	EXP = c;
	do
	{
		Symbol = *c++;
		if (Symbol == 0) break;
		int v = Symbol - '0';
		if (0 <= v && v <= 9)
		{
			if (Number < 0) Number = v;
			else Number = Number * 10 + v;
		}
		else
		{
			if (Number >= 0)
			{
				// The operand value is completely read; Push operand value to operand stack
				PushOperandStackWithNumber();
			}
			switch (Symbol)
			{
			case '(':
				PushOperatorStackWithSymbol();
				break;
			case ')':
				EvalueTopOperatorAndOperands(2);
				break;
			case '=':
				if( Operator != 0 ) 
					EvalueTopOperatorAndOperands(1);
				//PrintOperandStack();
				Print();
				break;
			case '+':
			case '-':
				//if (Operator != 0 && (Operator->op == '+' || Operator->op == '-'))
					// Same level operator; Evaluate previous operation
				if (Operator != 0 && Operator->op != '(') 
					EvalueTopOperatorAndOperands(1);
				PushOperatorStackWithSymbol();
				break;
			case '*':
			case '/':
			case '%':
				// High level operator, previous expression can be evaluated
			//	if (Operator != 0 && Operand != 0 ) EvalueTopOperatorAndOperands();
				if (Operator != 0 && (Operator->op == '*' || Operator->op == '/' || Operator->op == '%'))
					// Same level operator; Evaluate previous operation
					EvalueTopOperatorAndOperands(0);
				PushOperatorStackWithSymbol();
				break;
			default:
				break;
			}
		}

	} while (1);

	printf("\nHello");
	scanf("%d");
}