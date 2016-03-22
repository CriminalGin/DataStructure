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
#include <math.h>

#define MAXNUM 400
#define PRECEDENCELEN 9
#define TESTNUM 4

typedef enum{ FALSE = 0, TRUE = 1 } Boolean;

typedef enum{ lparen, rparen, plus, minus, times, divide, eos, operand, space } precedence;

int isp[] = { 0, 19, 12, 12, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 0 };



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
	else if (strcmp(type, "double") == 0){
		s->stack = (void*)malloc(size * sizeof(double));
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

void pushDouble(stack *s, double e){
	if (IsFull(s) == FALSE)
	{
		++s->top;
		double *tmp = (double *)s->stack;
		tmp[s->top] = e;
	}
}

precedence popPrecedence(stack *s){
	if (IsEmpty(s) == FALSE)
	{
		precedence *tmp = (precedence *)s->stack;
		precedence _tmp = tmp[s->top--];
		return _tmp;
	}		
}

double popDouble(stack *s){
	if (IsEmpty(s) == FALSE){
		double *tmp = (double *)s->stack;
		double _tmp = tmp[s->top--];
		return _tmp;
	}	
}

double topDouble(stack *s){
	if (IsEmpty(s) == FALSE)
	{
			double *tmp = (double *)s->stack;
			return tmp[s->top];
	}
}

precedence topPrecedence(stack *s){
	if (IsEmpty(s) == FALSE)
	{
		precedence *tmp = (precedence *)s->stack;
		return tmp[s->top];
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

void postfix(char *expr, char *post){
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
			while (topPrecedence(s) != lparen){ post[postPosition++] = printToken(popPrecedence(s)); }
			popPrecedence(s);		// discard the left parameters
		}
		else{
			while (isp[topPrecedence(s)] >= icp[token]){ post[postPosition++] = printToken(popPrecedence(s)); }
			pushPrecedence(s, token);
		}
	}
	while ((token = popPrecedence(s)) != eos){ post[postPosition++] = printToken(token); }
	post[postPosition++] = '\0';
	postPosition = 0;
}

double eval(char *post){
	precedence token; char symbol; stack *s; double op1, op2;
	int n = 0;	// counter for the expression string expr*
	s = createS(MAXNUM, "double");
	token = getToken(post, &symbol, &n);
       	while (token != eos){	// char conversion to int
		if (token == operand){ op1 = symbol - '0'; pushDouble(s, op1); }
		else{
			op2 = popDouble(s); op1 = popDouble(s);
			switch (token){
			case plus: pushDouble(s, op1 + op2); break;
			case minus: pushDouble(s, op1 - op2); break;
			case times: pushDouble(s, op1 * op2); break;
			case divide: pushDouble(s, op1 / op2); break;
			}
		}
		token = getToken(post, &symbol, &n);
	}
	double tmp;
	tmp = popDouble(s);
	return tmp;	// return result
}

int _tmain(int argc, _TCHAR* argv[])
{
	char *test[TESTNUM];
	double answer[TESTNUM];
	double error;
	double result;
	int i = 0;
	for (i = 0; i < TESTNUM; ++i){
		test[i] = (char *)malloc(sizeof(char) * MAXNUM);

	}
	strcpy(test[0], "1+2-3/4/5*6+7");
	answer[0] = 9.1;
	strcpy(test[1], "1 + (2 - 3 / (4 / 5)) * 6 + 7");
	// strcpy(test[1], "1+(2-3/(4/5))*6+7");
	answer[1] = -2.5;
	strcpy(test[2], "441.43+(32.30-3.0/(0.4/9.5))*6.0+123.7");
	answer[2] = 331.43;
	strcpy(test[3], "-441.43+(-32.30-3.0/(0.4/-9.5))*-6.0+-123.7");
	answer[3] = -798.83;
	
	

	for (i = 0; i < TESTNUM; ++i){
		char *expr = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars for an infix expression
		char *post = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars after postfix
		strcpy(expr, test[i]);
		postfix(expr, post);
		result = eval(post);
		if ((error = abs((result - answer[i]) / answer[i])) < 1e-9){ printf("Test %d is right.\n"); }
		else{ printf("Result is %lf, answer is %lf, error is %lf\n", result, answer[i], error); }
		free(expr); free(post);
	}
	system("PAUSE");
	return 0;
}