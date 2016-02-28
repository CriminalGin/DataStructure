// Matrix Multiplication.cpp : 定义控制台应用程序的入口点。
/*
Description:
Given two matrices A and B with size m * n and n * p respectively (1 <= m, n , p <= 50), 
suppose all elements in the matrices are integers whose absolute value is less than 1000. 
Calculate result of A * B.

Input:
Matrices A and B are input in the following format.
m n
A_1_1 A_1_2 ... A_1_n
A_2_1 A_2_2 ... A_2_n
...   ...  ...  ...
A_m_1 A_m_2 ... A_m_n
n p
B_1_1 B_1_2 ... B_1_p
B_2_1 B_2_2 ... B_2_p
...   ...  ...  ...
B_n_1 B_n_2 ... B_n_p

Output:
Let C = A * B, you should output matrix C in the following format.
C_1_1 C_1_2 ... C_1_p
C_2_1 C_2_2 ... C_2_p
...   ...  ...  ...
C_m_1 C_m_2 ... C_m_p

Sample input:
2 2
1 1
-1 1
2 2
1 2
3 4

Sample output:
4 6
2 2

Hints:
You can use "malloc" to allocate memory and use row-major representation for matrices.
*/

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct{
	int *data;
	int width;
	int height;
}matrice;

matrice *CreateM(int width, int height)
{
	matrice *_matrice;
	_matrice = (matrice *)malloc(sizeof(matrice));
	_matrice->data = (int *)malloc(width * height * sizeof(int));
	_matrice->width = width;
	_matrice->height = height;
	return _matrice;
}

int *Store(int row, int column, matrice *_matrice)
{
	if (row >= 0 && row < _matrice->height && column >= 0 && column < _matrice->width){
		return (_matrice->data + row * _matrice->width + column);
	}
	return NULL;
}

int Retrieve(int row, int column, matrice * _matrice)
{
	if (row >= 0 && row < _matrice->height && column >= 0 && column < _matrice->width){
		return *((_matrice->data) + row * _matrice->width + column);
	}
	return -1;
}

int PrintAll(matrice *_matrice)
{
	int i, j;
	for (i = 0; i < _matrice->height; ++i){
		for (j = 0; j < _matrice->width; ++j){
			printf("%d ", Retrieve(i, j, _matrice));
		}
		printf("\n");
	}
	return 0;
}


int ScanfAll(matrice *_matrice)
{
	int i, j;
	for (i = 0; i < _matrice->height; ++i){
		for (j = 0; j < _matrice->width; ++j){
			scanf("%d", Store(i, j, _matrice));
		}
	}
	return 0;
}



int main()
{
	int width, height;
	scanf("%d", &width), scanf("%d", &height);
	matrice* m1 = CreateM(width, height);
	ScanfAll(m1);

	scanf("%d", &width), scanf("%d", &height);
	matrice* m2 = CreateM(width, height);
	ScanfAll(m2);

	PrintAll(m1), PrintAll(m2);

	if (m2->height == m1->width)
	{
		matrice* result = CreateM(m2->width, m1->height);
		int i, j, k;

		for (i = 0; i < m1->height; ++i){
			for (j = 0; j < m2->width; ++j){
				int tmp = 0;
				for (k = 0; k < m1->width; ++k){
					tmp += Retrieve(i, k, m1) * Retrieve(k, j, m2);
				}
				*Store(i, j, result) = tmp;
			}
		}
		PrintAll(result);
		return 0;
	}
	return -1;
}

