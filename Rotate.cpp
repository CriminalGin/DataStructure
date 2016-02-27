// Rotate.cpp : 定义控制台应用程序的入口点。

/*
Description:
Suppose a string is arranged in a ring in clockwise direction. 
Fig.1 shows how string "abcdef" is arranged.

(a)- b            (c)- d
/     \           /     \
f       c         b       e
\     /           \     /
e - d             a - f
Fig. 1            Fig. 2

We can rotate the string by n charactors (n >= 0 for counter-clockwise direction; 
n < 0 for clockwise direction). 
Fig. 2 show what it looks like after rotate "abcdef" by 2 or -4 charactors. 
You are asked to calculate the string after the rotation.

Input:
A string s and an integer n. The length of s is less than 1,000 and |n| is less than 1,000,000,000.

Output:
You should output only one line which contains the string after the rotation.

Sample input:
abcdef 2

Sample output:
cdefab
*/

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define SIZE 1000

typedef enum{ FALSE = 0, TRUE = 1 }Boolean;

int PositiveMod(int lhs, int rhs)
{
	int result;
	result = lhs % rhs;
	if (result >= 0){ return result; }
	else if(result < 0){ return (result + rhs); }
}

typedef struct{
	int realSize;
	int nominalSize;
	int front;
	int rear;
	char *element;
}cqueue;

cqueue *CreateQ(int size){
	cqueue *q = (cqueue*)malloc(sizeof(cqueue));
	q->realSize = size;
	q->nominalSize = size + 1;	// waste a slot
	q->element = (char*)malloc(sizeof(char) * q->nominalSize);
	q->front = 0;
	q->rear = 0;
	return q;
}

Boolean IsEmptyQ(cqueue *q){
	if (q->front == q->rear)
	{
		return TRUE;
	}
	return FALSE;
}

Boolean IsFullQ(cqueue *q){
	if (PositiveMod((q->rear + 1),q->nominalSize) == q->front)
	{
		return TRUE;
	}
	return FALSE;
}


void Enqueue(cqueue *q, char e){
	if (!IsFullQ(q)){
		q->element[q->rear] = e;
		q->rear = PositiveMod((q->rear + 1), q->nominalSize);
	}
}
#if 0
int Dequeue(cqueue *q){
	if (!IsEmptyQ(q)){
		char tmp;
		tmp = q->element[q->front];
		q->front = PositiveMod((q->front + 1), q->nominalSize);
		return tmp;
	}
}
#endif

int PrintAll(cqueue *q){
	int i;
	for (i = 0; i < q->realSize; ++i)
	{
		printf("%c", q->element[i]);
	}
	return 0;
}

int Rotate(cqueue *q, int step){
	int i;
	step = step % q->realSize;
	if (step < 0)
	{
		i = PositiveMod(q->rear - 1, q->realSize);
		int rearInitial = i;
		step = step * (-1);
		while (step){
			char tmp = q->element[i];
			i = PositiveMod(i - 1, q->realSize);
			int j;
			for (j = 0; j < (q->realSize - 1); ++j)
			{
				q->element[PositiveMod((i + 1), q->realSize)] = q->element[i];
				i = PositiveMod((i - 1), q->realSize);
			}
			q->element[(rearInitial + 1) % q->realSize] = tmp;
			q->rear = PositiveMod((q->rear + 1), q->realSize);
			q->front = PositiveMod((q->front + 1), q->realSize);
			--step;
		}
	}
	else if (step > 0)
	{
		while (step){
			i = PositiveMod(q->front, q->realSize);
			int frontInitial = i;
			char tmp = q->element[i];
			int j;
			for (j = 0; j < (q->realSize - 1); ++j){
				i = PositiveMod((i + 1), q->realSize);
				q->element[PositiveMod(i - 1, q->realSize)] = q->element[i];				
			}
			q->element[PositiveMod((frontInitial - 1), q->realSize)] = tmp;
			--step;
		}
	}
	
	return 0;
}

int main()
{
	char _q[SIZE];
	scanf("%s", _q);
	int i = 0;
	while (_q[i] != '\0'){ ++i; }
	int size = i;
	cqueue *q = CreateQ(size);
	for (i = 0; i < q->realSize; ++i){Enqueue(q, _q[i]);}
	int step;
	scanf("%d", &step);
	Rotate(q, step);
	PrintAll(q);
  	return 0;
}

