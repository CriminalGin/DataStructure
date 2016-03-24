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
#define PRECEDENCELEN 10
#define TESTNUM 8

typedef enum{ FALSE = 0, TRUE = 1 } Boolean;

typedef enum{ lparen, rparen, plus, minus, times, divide, eos, lg, negative, operand, space, dot } precedence;

int isp[] = { 0, 19, 12, 12, 13, 13, 0, 15, 14 };
int icp[] = { 20, 19, 12, 12, 13, 13, 0, 15, 14 };

typedef struct{
	int size;
	int top;
	void *stack;
}stack;

typedef struct{
	char **element;
	int size;
}post;

post *createP(){
	post *p; p = (post*)malloc(sizeof(post));
	p->element = (char **)malloc(sizeof(char*) * MAXNUM);
	int i;
	for (i = 0; i < MAXNUM; ++i){ p->element[i] = (char *)malloc(sizeof(char) * MAXNUM); }
	p->size = 0;
	return p;
}

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

precedence getTokenPostfix(char *expr, char *symbol, int *n){
	
	*symbol = expr[(*n)++];
	int *tmp = (int*)malloc(sizeof(int));
	*tmp = *n - 2;
	switch (*symbol){
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-':
		if (expr[*tmp] != '0' && expr[*tmp] != '1' && expr[*tmp] != '2' && expr[*tmp] != '3'\
			&& expr[*tmp] != '4' && expr[*tmp] != '5' && expr[*tmp] != '6' && expr[*tmp] != '7'\
			&& expr[*tmp] != '8' && expr[*tmp] != '9' && expr[*tmp] != ')'){
			return negative;
		}
		else{ return minus; }
	case '*': return times;
	case '/': return divide;
	case '1': case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':
		return operand;
	case ' ': return space;
	case '\0': return eos;
	case '.': return dot;
	case 'l': return lg;
	default: return operand;
	}
}

precedence getTokenEval(char *expr, char *symbol, int *n){

	*symbol = expr[(*n)++];
	int *tmp = (int*)malloc(sizeof(int));
	*tmp = *n - 2;
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
	case '.': return dot;
	case 'l': return lg;
	case 'n': return negative;
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

#if 0
char printToken(precedence token){
	switch (token){
	case lparen: return '('; 
	case rparen: return ')'; 
	case plus: return '+'; 
	case minus: return '-'; 
	case times: return '*'; 
	case divide: return '/';
	case lg: return;
	default: break;
	}
}
#endif

char *printToken(precedence token){
	switch (token){
	case lparen: return "(\0";
	case rparen: return ")\0";
	case plus: return "+\0";
	case minus: return "-\0";
	case negative: return "n\0";
	case times: return "*\0";
	case divide: return "/\0";
	case lg: return "l\0";	// "l" represents "log"
	default: break;
	}
}

char *compact(char *string){
	char *_compact = (char *)malloc(sizeof(char) * MAXNUM);
	int i = 0, j = 0;
	for (i = 0; i < MAXNUM; ++i){
		if (string[i] != ' '){ _compact[j++] = string[i]; }
	}
	_compact[j++] = '\0';
	return _compact;
}

char *removeMinus(char *string){
	int i = 0, strLength = strlen(string);
	char *result = (char *)malloc(sizeof(char) * strLength);
	int k = 0;
	for (i = 0; i < strLength;){
		if (string[i] == '-'){
			int count = 1;
			while (string[++i] == '-'){ ++count; }
			if ((count % 2) == 0){ }
			else{ result[k] = '-'; ++k; }
		}
		else{
			result[k] = string[i];
			++i, ++k;
		}
	}
	result[k] = '\0';
	return result;
}

void postfix(char *tmp, post *p){
	char *tmp2 = compact(tmp);
	char *expr = removeMinus(tmp2);
	char symbol; precedence token; int n = 0; stack *s = createS(strlen(expr), "precedence");
	pushPrecedence(s, eos);
	token = getTokenPostfix(expr, &symbol, &n);
	precedence savePopPrecedence;
	while (token != eos){
		int k = 0; int nMinus2 = n - 2; 
		if ((token == operand) || (token == dot)){
			while ((token == operand) || (token == dot)){
				p->element[p->size][k++] = printSymbol(symbol);
				token = getTokenPostfix(expr, &symbol, &n);
				if (token == eos){ break; }
			} p->element[p->size++][k++] = '\0'; k = 0;
		}
		else if (token == space){ token = getTokenPostfix(expr, &symbol, &n); }
		else if (token == rparen){
			while ((savePopPrecedence = topPrecedence(s)) != lparen){
				while (strcpy(p->element[p->size], printToken(popPrecedence(s))) == NULL);
				++p->size;
				// p->element[p->size++][1] = '\0';
			}
			popPrecedence(s);		// discard the left parameters
			token = getTokenPostfix(expr, &symbol, &n);
		}
#if 0
		else if ((token == minus) && (expr[n - 2] != '0') && (expr[n - 2] != '1') && (expr[n - 2] != '2')\
			&& (expr[n - 2] != '3') && (expr[n - 2] != '4') && (expr[n - 2] != '5') && (expr[n - 2] != '6')\
			&& (expr[n - 2] != '7') && (expr[n - 2] != '8') && (expr[n - 2] != '9') && (expr[n - 2] != ')')){
			p->element[p->size][k++] = printSymbol(symbol);
			token = getTokenPostfix(expr, &symbol, &n);
			while ((token == operand) || (token == dot)){
				p->element[p->size][k++] = printSymbol(symbol);
				token = getTokenPostfix(expr, &symbol, &n);
				if (token == eos){ break; }
			} p->element[p->size++][k++] = '\0'; k = 0;
		}
#endif
		else if ((token == lg)){
			while (isp[topPrecedence(s)] >= icp[token]){
				while (strcpy(p->element[p->size], printToken(popPrecedence(s))) == NULL);
				++p->size;
			}
			pushPrecedence(s, token);
			++n;
			++n;
			token = getTokenPostfix(expr, &symbol, &n);
#if 0
			p->element[p->size][k++] = printSymbol(symbol);
			while (k < 3){
				token = getTokenPostfix(expr, &symbol, &n);
				p->element[p->size][k++] = printSymbol(symbol);
			}
			pushPrecedence(s, token);
			p->element[p->size++][k++] = '\0'; k = 0;
#endif
		}
		else{
			while (isp[topPrecedence(s)] >= icp[token]){
				// strcpy(p->element[p->size], printToken(popPrecedence(s)));
				while (strcpy(p->element[p->size], printToken(popPrecedence(s))) == NULL);
				//p->element[p->size][0] = printToken(popPrecedence(s)); 
				//p->element[p->size++][1] = '\0';
				++p->size;
			}
			pushPrecedence(s, token);
			token = getTokenPostfix(expr, &symbol, &n);
		}
	}
	while ((token = popPrecedence(s)) != eos){ 
		while (strcpy(p->element[p->size], printToken(token)) == NULL);
		++p->size;
		//p->element[p->size][0] = printToken(token); 
		//p->element[p->size++][1] = '\0';
	}
	p->element[p->size++][0] = '\0';
}

double eval(post *p){
	precedence token; char symbol; stack *s; double op1, op2; int n = 0;	// counter for the expression string expr*
	s = createS(MAXNUM, "double");
	int initSize = p->size; char *charNum = p->element[initSize - (p->size)];
	token = getTokenEval(p->element[initSize - (p->size--)], &symbol, &n); n = 0;
       	while (token != eos){	// char conversion to int
			if (token == operand){ 
				op1 = atof(charNum); pushDouble(s, op1); 
			}
			else if (token == lg){
				op1 = popDouble(s); 
				pushDouble(s, log10(op1));
			}
			else if (token == negative){
				op1 = popDouble(s); 
				pushDouble(s, (-1) * op1);
			}
			else{
				op2 = popDouble(s); op1 = popDouble(s);
				switch (token){
				case plus: pushDouble(s, op1 + op2); break;
				case minus: pushDouble(s, op1 - op2); break;
				case times: pushDouble(s, op1 * op2); break;
				case divide: pushDouble(s, op1 / op2); break;
				}
			}
			free(charNum);
			char *charNum = (char *)malloc(sizeof(char) * MAXNUM); strcpy(charNum, p->element[initSize - (p->size)]);
			token = getTokenEval(p->element[initSize - (p->size--)], &symbol, &n); n = 0;
	}
	double tmp = popDouble(s);
	return tmp;	// return result
}

int *findNegative(char *string){
	int *position = (int *)malloc(sizeof(int) * MAXNUM);
	int i, j = 0;
	if (strcmp(string, "-")){
		for (i = 0; i < strlen(string); ++i){
			if (string[i] == '-'){
				switch (string[i + 1]){
				case '0': case '1': case '2': case '3': case '4': 
				case '5': case '6': case '7': case '8': case '9':
					position[j++] = i;
				default: break;;
				}
			}
		}
		int *result = (int *)malloc(j * sizeof(int));
		for (i = 0; i < j; ++i){
			result[i] = position[i];
		}
		return result;
	}
	return NULL;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char *test[TESTNUM];
	double answer[TESTNUM], error, result; int i = 0;
	printf("%f\n", log(1.0));
	for (i = 0; i < TESTNUM; ++i){
		test[i] = (char *)malloc(sizeof(char) * MAXNUM);
	}
	strcpy(test[0], "1+2-3/4/5*6+7"); answer[0] = 9.1;
	// strcpy(test[0], "-log(0.01)"); answer[0] = -log10((0.01));

	strcpy(test[1], "1 + (2 - 3 / (4 / 5)) * 6 + 7"); answer[1] = -2.5;
	strcpy(test[2], "441.43+(32.30-3.0/(0.4/9.5))*6.0+123.7"); answer[2] = 331.43;
	strcpy(test[3], "-441.43+(-32.30-3.0/(0.4/-9.5))*-6.0+-123.7"); answer[3] = -798.83;
	strcpy(test[4], "---1"); answer[4] = -1;
	// strcpy(test[5], "-log(log(441.43))"); answer[5] = -log10(log10(441.43));
	// strcpy(test[5], "log(-0.01)"); answer[5] = -log10((441.43));

	strcpy(test[5], "-log(log(441.43))+(-32.30-3.0/(0.4/log(-log(0.01))))*-6.0+-123.7"); answer[5] = 83.2239468134;
	// strcpy(test[5], "log(log(441.43))+(-32.30-3.0/(0.4/log(log(100))))*-6.0+-123.7"); answer[5] = 0;
	// strcpy(test[5], "log(441.43)"); answer[5] = 0;
	strcpy(test[6], "-loglog441.43+(-32.30-3.0/(0.4/log-log0.01))*-6.0+-123.7"); answer[6] = 83.2239468134;
	strcpy(test[7], "1 + 3 + - 3 + log 1 + log      (   ( 12.3 )   )  / ((1))"); answer[7] = 2.0899051114;
	// strcpy(test[5], "log(10)"); answer[5] = 83.2239468134;

	for (i = 0; i < TESTNUM; ++i){
		char *expr = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars for an infix expression
		// char *post = (char*)malloc(MAXNUM * sizeof(char));	// an array of chars after postfix
		post *p = createP(); strcpy(expr, test[i]); 
		postfix(expr, p);
		int j = 0;
		for (j = 0; j < p->size; ++j){
			printf("%s", p->element[j]);
		}printf("\n");
		result = eval(p);
		if ((error = abs((result - answer[i]) / answer[i])) < 1e-9){ printf("Test %d is right.\n", i); }
		else{ printf("Result is %.10f, answer is %.10f, error is %.10f\n", result, answer[i], error); }
		free(expr); free(p->element);
	}
	system("PAUSE");
	return 0;
}