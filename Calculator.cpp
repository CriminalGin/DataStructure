// Calculator.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"

/*
I, <JIANG Zhijian>, am submitting the assignment for
an individual project.
I declare that the assignment here submitted is original except for
source material explicitly acknowledged, the piece of work, or a part
of the piece of work has not been submitted for more than one purpose
(i.e. to satisfy the requirements in two different courses) without
declaration. I also acknowledge that I am aware of University policy
and regulations on honesty in academic work, and of the disciplinary
guidelines and procedures applicable to breaches of such policy and
regulations, as contained in the University website
http://www.cuhk.edu.hk/policy/academichonesty/.
It is also understood that assignments without a properly signed
declaration by the student concerned will not be graded by the
teacher(s).
*/

#include <stdlib.h>
#include <stdio.h>

#define MAXNUM 400

typedef enum{ FALSE = 0, TRUE = 1 } Boolean;

typedef enum{ lparen, rparen, plus, minus, times, divide, eos, operand, space } precedence;

int isp[] = { 0, 19, 12, 12, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 0 };

char *expr = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars for an infix expression

typedef struct{
	int size;
	int top;
	precedence *stack;
}stack;

stack *createS(int size){
	stack *s;
	s = (stack*)malloc(sizeof(stack));
	s->size = size;
	s->top = -1;
	s->stack = (precedence*)malloc(size * sizeof(precedence));
	return s;
}

Boolean IsFull(stack *s){
	if (s->top == s->size - 1){ return TRUE; }
	else{ return FALSE; }
}

Boolean IsEmpty(stack *s){
	if (s->top == -1){ return TRUE; }
	else{ return FALSE; }
}

void push(stack *s, precedence e){
	if (IsFull(s) == FALSE)
	{
		++s->top;
		s->stack[s->top] = e;
	}
}

precedence pop(stack *s){
	if (IsEmpty(s) == FALSE)
	{
		return (precedence)s->stack[s->top--];
	}
}

precedence top(stack *s){
	if (IsEmpty(s) == FALSE)
	{
		return s->stack[s->top];
	}
}

precedence getToken(char *symbol, int *n){
	*symbol = expr[(*n)++];
	switch (*symbol){
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '*': return times;
	case '/': return divide;
	case '1': case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':
		return operand;
	case ' ': return space;
	case '\0': return eos;
	default: return operand;
	}
}

void printSymbol(char symbol){
	printf("%c", symbol);
}

void printToken(precedence token){
	switch (token){
	case lparen: printf("("); break;
	case rparen: printf(")"); break;
	case plus: printf("+"); break;
	case minus: printf("-"); break;
	case times: printf("*"); break;
	case divide: printf("/"); break;
	default: break;
	}
}

#if 0
int eval(void){
	precedence token; char symbol; stack *s; int op1, op2;
	int n = 0;	// counter for the expression string expr*
	s = createS(MAXNUM);
	token = getToken(&symbol, &n);
	while (token != eos){	// char conversion to int
		if (token == operand){ op1 = symbol - '0'; push(s, op1); }
		else{ 
			op2 = pop(s); op1 = pop(s);
			switch (token){
			case plus: push(s, op1 + op2); break;
			case minus: push(s, op1 - op2); break;

			}
		}
	}
}
#endif

void postfix(){
	char symbol;
	precedence token;
	int n = 0;
	stack *s = createS(MAXNUM);
	push(s, eos);
	for (token = getToken(&symbol, &n); token != eos;  token = getToken(&symbol, &n)){
		if (token == operand){ printSymbol(symbol); }
		else if (token == space);
		else if (token == rparen){
			while (top(s) != lparen){ printToken(pop(s)); }
			pop(s);		// discard the left parameters
		}
		else{
			while (isp[top(s)] >= icp[token]){ printToken(pop(s)); }
			push(s, token);
		}
	}
	while ((token = pop(s)) != eos){ printToken(token); }
}

int _tmain(int argc, _TCHAR* argv[])
{
	int result = 0;
	gets(expr);
	postfix();
	printf("%d\n", result);
	system("PAUSE");
	return 0;
}