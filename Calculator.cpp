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
#include <string.h>

#define MAXNUM 400
#define PRECEDENCELEN 9

typedef enum{ FALSE = 0, TRUE = 1 } Boolean;

typedef enum{ lparen, rparen, plus, minus, times, divide, eos, operand, space } precedence;

int isp[] = { 0, 19, 12, 12, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 0 };

char *expr = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars for an infix expression
char *post = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars after postfix


typedef struct{
	int size;
	int top;
	void *stack;
}stack;

stack *createS(int size, char *type){
	stack *s;
	s = (stack*)malloc(sizeof(stack));
	s->size = size;
	s->top = -1;
	if (strcmp(type, "precedence") == 0){
		s->stack = (void*)malloc(size * sizeof(precedence));
	}
	else if (strcmp(type, "int") == 0){
		s->stack = (void*)malloc(size * sizeof(int));
	}
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


void pushPrecedence(stack *s, precedence e){
	if (IsFull(s) == FALSE)
	{
		++s->top;
		precedence *tmp = (precedence *)s->stack;
		tmp[s->top] = e;
	}
}

void pushInt(stack *s, int e){
	if (IsFull(s) == FALSE)
	{
		++s->top;
		int *tmp = (int *)s->stack;
		tmp[s->top] = e;
	}
}

int pop(stack *s){
	if (IsEmpty(s) == FALSE)
	{
		int *tmp = (int *)s->stack;
		int _tmp = tmp[s->top--];
		return _tmp;
#if 0
		if (strcmp(type, "precedence") == 0){
			precedence *tmp = (precedence *)s->stack;
			return (int)tmp[s->top--];
		}
		else if (strcmp(type, "int") == 0){
			precedence *tmp = (precedence *)s->stack;
			return tmp[s->top--];
		}
#endif
	}
}

int top(stack *s, char *type){
	if (IsEmpty(s) == FALSE)
	{
		if (strcmp(type, "precedence") == 0){
			precedence *tmp = (precedence *)s->stack;
			return tmp[s->top];
		}
		else if (strcmp(type, "int") == 0){
			int *tmp = (int *)s->stack;
			return tmp[s->top];
		}
	}
}

precedence getToken(char *expr, char *symbol, int *n){
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

char printSymbol(char symbol){
	return symbol;
}

#if 0
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
#endif

char printToken(precedence token){
	switch (token){
	case lparen: return '('; 
	case rparen: return ')'; 
	case plus: return '+'; 
	case minus: return '-'; 
	case times: return '*'; 
	case divide: return '/'; 
	default: break;
	}
}

void postfix(){
	char symbol;
	precedence token;
	int n = 0;
	stack *s = createS(MAXNUM, "precedence");
	pushPrecedence(s, eos);

#if 0
	for (token = getToken(&symbol, &n); token != eos;  token = getToken(&symbol, &n)){
		if (token == operand){ printSymbol(symbol); }
		else if (token == space);
		else if (token == rparen){
			while (top(s) != lparen){ printToken((precedence)pop(s)); }
			(precedence)pop(s);		// discard the left parameters
		}
		else{
			while (isp[(precedence)top(s)] >= icp[token]){ printToken((precedence)pop(s)); }
			pushPrecedence(s, token);
		}
	}
	while ((token = (precedence)pop(s)) != eos){ printToken(token); }
#endif

	int postPosition = 0;
	for (token = getToken(expr, &symbol, &n); token != eos; token = getToken(expr, &symbol, &n)){
		if (token == operand){ post[postPosition++] = printSymbol(symbol); }
		else if (token == space);
		else if (token == rparen){
			while (top(s, "precedence") != lparen){ post[postPosition++] = printToken((precedence)pop(s)); }
			(precedence)pop(s);		// discard the left parameters
		}
		else{
			while (isp[top(s, "precedence")] >= icp[token]){ post[postPosition++] = printToken((precedence)pop(s)); }
			pushPrecedence(s, token);
		}
	}
	while ((token = (precedence)pop(s)) != eos){ post[postPosition++] = printToken(token); }
	post[postPosition++] = '\0';
	postPosition = 0;
}

int eval(void){
	precedence token; char symbol; stack *s; int op1, op2;
	int n = 0;	// counter for the expression string expr*
	s = createS(MAXNUM, "int");
	token = getToken(post, &symbol, &n);
	while (token != eos){	// char conversion to int
		if (token == operand){ op1 = symbol - '0'; pushInt(s, op1); }
		else{
			op2 = pop(s); op1 = pop(s);
			switch (token){
			case plus: pushInt(s, op1 + op2); break;
			case minus: pushInt(s, op1 - op2); break;
			case times: pushInt(s, op1 * op2); break;
			case divide: pushInt(s, op1 / op2); break;
			}
		}
		token = getToken(post, &symbol, &n);
	}
	int tmp;
	tmp = (int)pop(s);	
	return tmp;	// return result
}

int _tmain(int argc, _TCHAR* argv[])
{
	char *test[8];
	int i = 0;
	for (i = 0; i < 8; ++i){
		test[i] = (char *)malloc(sizeof(char) * MAXNUM);
	}
	strcpy(test[0], "1+2-3/4/5*6+7");
	strcpy(expr, test[0]);
	postfix();
	printf("\nresult is %d\n", eval());
	system("PAUSE");
	return 0;
}