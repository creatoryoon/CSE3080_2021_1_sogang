#include<stdio.h>
#include<stdlib.h>

/*
*	Sparse matrix의 정보를 저장할 구조체입니다.
*/
struct MATRIX {
	int r, c, dat;
};
/*
*	Push 함수는 매개변수로 받는 MATRIX 배열에, r, c, dat 데이터를 추가합니다.
*	idx는 MATRIX 배열의 저장 위치, 데이터가 들어있는 맨 뒤의 위치를 나타내며
*	capacity는 실제 저장 가능한 공간을
*	r, c, dat는 삽입할 데이터입니다.
*/
void Push(struct MATRIX** mat, int* idx, int* capacity, int r, int c, int dat) {
	if (*idx >= *capacity) {	//만약 현재 저장하고자 하는 위치가 저장 가능한 공간보다 크다면
		(*capacity) *= 2;	//저장 가능한 공간을 2배로 늘리고
		struct MATRIX* tmp = (struct MATRIX*)malloc(sizeof(struct MATRIX) * (*capacity));	//해당 데이터를 저장하기 위한 새로운 공간을 할당한 뒤
		for (int k = 0; k < *idx; ++k)	//원래 배열의 데이터를 새로운 배열에 복사해준 뒤
			tmp[k] = (*mat)[k];
		free((*mat));	//기존의 배열 메모리를 해제하고
		(*mat) = tmp;	//새로 만든 배열을 가리키도록 바꿈
	}
	(*mat)[(*idx)].r = r;	//배열의 맨 뒤에 데이터를 추가
	(*mat)[(*idx)].c = c;
	(*mat)[(*idx)++].dat = dat;
}
/*
*	LoadMatrix는 fileName으로부터 행렬 데이터를 불러옵니다.
*	idx는 실제 저장한 데이터의 개수를 나타냅니다.
*	capacity는 해당 배열의 실제 저장 가능한 공간의 크기를 나타냅니다.
*	n과 m은 행렬의 크기를 나타냅니다.
*/
struct MATRIX* LoadMatrix(const char* fileName, int * idx, int * capacity, int * n, int * m) {
	FILE* fp = fopen(fileName, "r");	//FILE 포인터를 이용하여 fileName을 read only로 읽습니다.
	if (fp == NULL) {	//만약 해당 파일이 비어있다면 함수를 즉시 종료합니다.
		printf("Can't find file");
		return 0;
	}
	*idx = 0;	//idx를 0으로
	*capacity = 20;	//저장 가능한 공간을 20으로 초기화합니다.
	struct MATRIX* res = (struct MATRIX*)malloc(sizeof(struct MATRIX) * (*capacity));	//읽어온 행렬을 저장할 배열을 만들어줍니다.
	fscanf(fp, "%d %d", n, m);	//파일로부터 행렬의 크기를 읽어옵니다.
	for (int i = 0; i < *n; ++i) {
		for (int j = 0; j <* m; ++j) {
			int num; fscanf(fp, "%d", &num);	//파일로부터 행렬의 원소를 읽어옵니다.
			if (num != 0)	//만약 원소가 0이 아니라면
				Push(&res, idx, capacity, i, j, num);	//Sparse matrix에 해당 원소를 삽입합니다.
		}
	}
	return res;
}
/*
*	Sum 함수는 두 sparse matrix(A, B)를 매개변수로 받아 A+B의 결과를 sparse matrix로 만들어 반환합니다.
*	idx와 capacity는 모두 위에서 사용한 것과 같은 의미를 갖습니다.
*/
struct MATRIX* Sum(struct MATRIX* A, struct MATRIX* B, int idx1, int idx2, int * idx, int * capacity) {
	int i1 = 0, i2 = 0;	//i2, i2는 각각 A, B 배열의 인덱스를 관리할 변수입니다.

	*idx = 0;	//idx와 capacity는 A+B의 결과를 저장할 배열의 인덱스와 저장공간을 관리할 변수입니다.
	*capacity = 20;
	struct MATRIX* res = (struct MATRIX*)malloc(sizeof(struct MATRIX) * (*capacity));	//A+B의 결과를 저장할 배열의 공간을 새롭게 할당합니다.

	while (i1 < idx1 && i2 < idx2) {	//두 배열의 인덱스 범위 내에서만 반복할 수 있도록 조건식을 만들어줍니다.
		if (A[i1].r == B[i2].r && A[i1].c == B[i2].c) {	//만약 두 원소의 행/열 값이 같다면 더할 수 있다는 뜻이므로,
			Push(&res, idx, capacity, A[i1].r, A[i1].c, A[i1].dat + B[i2].dat);	//더한 결과를 새로 만든 배열에 넣어주고
			++i1;	//두 인덱스를 하나씩 증가시켜줍니다.
			++i2;
		}
		else {
			int pos1 = A[i1].r * 10000 + A[i1].c;	//행과 열의 값을 간단하게 비교할 수 있도록 0000011111(0: r, 1 : c)가 되도록 하나의 정수로 만들어줍니다)
			int pos2 = B[i2].r * 10000 + B[i2].c;
			if (pos1 > pos2) {	//pos1 > pos2라면, B배열의 현재 위치에 있는 원소가 더 앞에 있다는 의미이므로
				Push(&res, idx, capacity, B[i2].r, B[i2].c, B[i2].dat);	//B배열의 현재 원소를 저장하고
				++i2;
			}
			else {	//아니라면
				Push(&res, idx, capacity, A[i1].r, A[i1].c, A[i1].dat);	//A배열의 현재 원소를 저장합니다.
				++i1;
			}

		}
	}
	/*
	*	밑의 두 반복문은 항상 둘 중 하나만 실행됩니다.
	*	위의 while문에서 탈출 조건이 (i1 >= idx1 || i2 >= idx2)이기 때문입니다.
	*	따라서 아직 남아있는 원소가 있는 경우 새로 만들어진 배열에 저장될 수 있도록 추가합니다.
	*/
	for (; i1 < idx1; ++i1)
		Push(&res, idx, capacity, A[i1].r, A[i1].c, A[i1].dat);
	for (; i2 < idx2; ++i2)
		Push(&res, idx, capacity, B[i2].r, B[i2].c, B[i2].dat);
	return res;
}
/*
*	FastTranspose 함수는 sparse matrix 배열을 매개변수로 받아 transpose한 결과를 반환합니다.
*	이때 n과 m은 swap해줍니다.
*/
struct MATRIX* FastTranspose(struct MATRIX* A, int idx, int * n, int * m) {
	struct MATRIX* res = (struct MATRIX*)malloc(sizeof(struct MATRIX) * idx);	//새로운 결과를 저장할 배열을 만들어줍니다.
	int cols = (*m) + 1;	//rowTerms를 만들어주기 위한 변수
	int* rowTerms = (int*)malloc(sizeof(int) * cols);	//rowTerms에 새로운 메모리 공간을 할당해줍니다.
	int tmp = *n;	//n과 m을 교체하기 위한 임시 변수
	*n = *m;
	*m = tmp;

	for (int i = 0; i < cols; ++i)	//rowTerms를 0으로 초기화해줍니다.
		rowTerms[i] = 0;

	for (int i = 0; i < idx; ++i)	//각 column의 개수가 몇개인지 세어줍니다.
		++rowTerms[A[i].c + 1];

	for (int i = 1; i < cols; ++i)	//start_pos와 비슷한 역할을 할 수 있도록 rowTerms에 누적합을 계산합니다.
		rowTerms[i] += rowTerms[i - 1];

	for (int i = 0; i < idx; ++i) {
		int pos = rowTerms[A[i].c];	//해당 배열의 현재 원소를 전치시키기 위해 저장되야 할 위치를 찾습니다.
									//이때 rowTerms에 저장되어 있는 값는 문제의 설명에서 나오는 start_pos와 같은 값입니다.
		res[pos].c = A[i].r;	//새로운 데이터를 추가합니다.
		res[pos].r = A[i].c;
		res[pos].dat = A[i].dat;
		++rowTerms[A[i].c];	//start_pos를 하나 증가시키는 것과 같은 효과를 냅니다.
	}
	return res;
}

/* Test용 코드 시작 */
int AA[10][10];
int BB[10][10];
void test(const char * file1, const char * file2) {
	freopen(file1, "r", stdin);
	int n, m; scanf("%d %d", &n, &m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			scanf("%d", &AA[i][j]);
	freopen(file2, "r", stdin);
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%d", &BB[i][j]);
			AA[i][j] += BB[i][j];
		}
	}
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (AA[j][i])
				printf("%d %d %d\n", i, j, AA[j][i]);
		}
	}
	puts("\n\n");
}
/* Test용 코드 끝 */
int main() {
	/*
	*	test 함수는 TEST용 코드 입니다.
	*	이 함수는 주어진 입력 파일(A.txt, B.txt)을 2차원 배열로 입력받아 더한 후, transpose한 결과를 sparse matrix 형식으로 출력합니다
	*	밑의 FastTranspose 함수가 제대로 동작하는지 확인하기 위해 사용한 함수이므로 과제 제출시에는 제거하셔도 무관합니다.
	*/
	const char* file1 = "./Crebugs/SparseMatrix/A.txt";
	const char* file2 = "./Crebugs/SparseMatrix/B.txt";
	test(file1, file2);	//제거 가능

	int idx1, cap1, idx2, cap2;	//Sparse matrix와 관련된 정보를 저장하는 변수들입니다. idx는 배열의 크기, cap은 실제 저장가능한 공간을 저장합니다.
	int n1, n2, m1, m2;	//Sparse matrix와 관련된 정보를 저장하는 변수입니다. n과 m은 행렬의 크기를 뜻합니다.

	struct MATRIX* A = LoadMatrix(file1, &idx1, &cap1, &n1, &m1);	//A.txt 파일을 읽어와 A에 저장합니다.
	struct MATRIX* B = LoadMatrix(file2, &idx2, &cap2, &n2, &m2);	//B.txt 파일을 읽어와 B에 저장합니다.
	
	int idx3, cap3;	//A+B의 결과를 저장할 행렬을 저장하는 sparse matrix의 정보를 저장하는 변수입니다.
	struct MATRIX* sum = Sum(A, B, idx1, idx2, &idx3, &cap3);	//sum에 A+B를 저장합니다.
	
	/*결과 출력*/
	printf("%d %d %d\n", n1, m1, idx3);
	for (int i = 0; i < idx3; ++i)
		printf("%d %d %d\n", sum[i].r, sum[i].c, sum[i].dat);
	puts("\n\n");

	struct MATRIX* transpose = FastTranspose(sum, idx3, &n1, &m1);	//sum의 transpose 결과를 저장합니다.

	/*결과 출력*/
	printf("%d %d %d\n", n1, m1, idx3);
	for (int i = 0; i < idx3; ++i)
		printf("%d %d %d\n", transpose[i].r, transpose[i].c, transpose[i].dat);

}