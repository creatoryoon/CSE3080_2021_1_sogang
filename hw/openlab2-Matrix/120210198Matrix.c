#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>


struct MATRIX {
	int r, c, dat;//Sparse matrix
};


void ToSparse(struct MATRIX** mat, int* idx, int* capacity, int r, int c, int dat) {
	if (*idx >= *capacity) {	
		(*capacity) *= 2;	// 부족시 저장 공간 2배후 복사, 메모리 할당
		struct MATRIX* tmp = (struct MATRIX*)malloc(sizeof(struct MATRIX) * (*capacity));	
		for (int k = 0; k < *idx; ++k)	
			tmp[k] = (*mat)[k];
		free((*mat));	
		(*mat) = tmp;	
	}
	(*mat)[(*idx)].r = r;	
	(*mat)[(*idx)].c = c;
	(*mat)[(*idx)++].dat = dat;
}

struct MATRIX* LoadMatrix(const char* fileName, int * idx, int * capacity, int * n, int * m) {
	FILE* fp = fopen(fileName, "r");	
	if (fp == NULL) {	
		printf("파일 없음");
		return 0;
	}
	*idx = 0;	//idx를 0으로
	*capacity = 20;	
	struct MATRIX* res = (struct MATRIX*)malloc(sizeof(struct MATRIX) * (*capacity));	//행렬을 저장할 배열
	fscanf(fp, "%d %d", n, m);	// 행렬의 크기
	for (int i = 0; i < *n; ++i) {
		for (int j = 0; j < *m; ++j) {
			int num; fscanf(fp, "%d", &num);	
			if (num != 0)	
				ToSparse(&res, idx, capacity, i, j, num);	//Sparse matrix삽입
		}
	}
	return res;
}




struct MATRIX* Sum(struct MATRIX* A, struct MATRIX* B, int idx1, int idx2, int * idx, int * capacity) {
	int i1 = 0, i2 = 0;	
	*idx = 0;	
	*capacity = 20;
	struct MATRIX* res = (struct MATRIX*)malloc(sizeof(struct MATRIX) * (*capacity));	

	while (i1 < idx1 && i2 < idx2) {	
		if (A[i1].r == B[i2].r && A[i1].c == B[i2].c) {	
			ToSparse(&res, idx, capacity, A[i1].r, A[i1].c, A[i1].dat + B[i2].dat);
			++i1;	
			++i2;
		}
		else {
			int pos1 = A[i1].r * 10000 + A[i1].c;	
			int pos2 = B[i2].r * 10000 + B[i2].c;
			if (pos1 > pos2) {	
				ToSparse(&res, idx, capacity, B[i2].r, B[i2].c, B[i2].dat);	
				++i2;
			}
			else {	
				ToSparse(&res, idx, capacity, A[i1].r, A[i1].c, A[i1].dat);	
				++i1;
			}

		}
	}

	for (; i1 < idx1; ++i1)
		ToSparse(&res, idx, capacity, A[i1].r, A[i1].c, A[i1].dat);
	for (; i2 < idx2; ++i2)
		ToSparse(&res, idx, capacity, B[i2].r, B[i2].c, B[i2].dat);
	return res;
}

struct MATRIX* FastTranspose(struct MATRIX* A, int idx, int * n, int * m) {
	struct MATRIX* res = (struct MATRIX*)malloc(sizeof(struct MATRIX) * idx);	//새로운 결과를 저장할 배열
	int cols = (*m) + 1;	
	int* rowTerms = (int*)malloc(sizeof(int) * cols);	//새로운 메모리 할당
	int tmp = *n;	//n과 m을 교체변수
	*n = *m;
	*m = tmp;

	for (int i = 0; i < cols; ++i)	
		rowTerms[i] = 0;

	for (int i = 0; i < idx; ++i)	
		++rowTerms[A[i].c + 1];

	for (int i = 1; i < cols; ++i)	
		rowTerms[i] += rowTerms[i - 1];

	for (int i = 0; i < idx; ++i) {
		int pos = rowTerms[A[i].c];	
									
		res[pos].c = A[i].r;	
		res[pos].r = A[i].c;
		res[pos].dat = A[i].dat;
		++rowTerms[A[i].c];
	}
	return res;
}


int main() {
	const char* file1 = "../A.txt";
	const char* file2 = "../B.txt";

	int idx1, cap1, idx2, cap2;
	int n1, n2, m1, m2;	

	struct MATRIX* A = LoadMatrix(file1, &idx1, &cap1, &n1, &m1);	
	struct MATRIX* B = LoadMatrix(file2, &idx2, &cap2, &n2, &m2);	

	int idx3, cap3;
	struct MATRIX* sum = Sum(A, B, idx1, idx2, &idx3, &cap3);	


	printf("%d %d %d\n", n1, m1, idx3);
	for (int i = 0; i < idx3; ++i)
		printf("%d %d %d\n", sum[i].r, sum[i].c, sum[i].dat);
	puts("\n\n");

	struct MATRIX* transpose = FastTranspose(sum, idx3, &n1, &m1);	


	printf("%d %d %d\n", n1, m1, idx3);
	for (int i = 0; i < idx3; ++i)
		printf("%d %d %d\n", transpose[i].r, transpose[i].c, transpose[i].dat);

}
