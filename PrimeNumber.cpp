// PrimeNumber.cpp : 定义控制台应用程序的入口点。

/*

Description:
A prime number (or a prime) is a natural number greater than 1 
that has no positive divisors other than 1 and itself. 
Given an integer n (n <= 2,000), find the n-th prime number.

Input:
An integer n.

Output:
You should output only one line which contains an integer, the n-th prime number.

Sample input:
2

Sample output:
3

*/

#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

typedef enum{FALSE = 0, TRUE = 1}Boolean;

Boolean IsPrime(double rhs){
	int i;
	if (rhs <= 1){ return FALSE; }
	if (rhs == 2){ return TRUE; }
	for (i = 2; i <= floor(sqrt(rhs)); ++i){
		if ((rhs / i - floor(rhs / i)) == 0){ return FALSE; }
	}
	return TRUE;
}

int *FindPrime(int max){
	int counter = 0, num = 2;
	int *primeArray = (int*)malloc(max * sizeof(int));
	while (counter < max){
		while (!IsPrime(num)){ ++num; }
		primeArray[counter] = num;
		++num;
		++counter;
	}
	return primeArray;
}

int main()
{
	int i;
	scanf("%d", &i);
	int *primeArray = FindPrime(i);
	printf("%d", primeArray[i - 1]);
	return 0;
}