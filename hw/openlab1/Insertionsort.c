#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int Swap(int* a, int* b)
{	
	int c = *a;	
	*a = *b;	
	*b = c;		
}// 바꿔주기

void Sort(int* arr, int size)
{
	for (int i = 1; i < size; ++i) 
	{	
		int cur = i;	
		for (int j = i - 1; j >= 0; --j) 
		{	
											
			if (arr[cur] < arr[j]) 
			{	
				Swap(&arr[cur], &arr[j]);
				--cur;
			}
			else {	
				break;
			}
		}
	}
}//삽입정렬


int* ArSet(int size)
{
	int* temp = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; ++i)
		temp[i] = size - i;
	return temp;
}// 경우의 수 설정


int main()
{
	FILE* fp = fopen("./lab1.data", "r");	
	if (fp == NULL) 
	{
		printf("파일없음.");
		return 0;
	}
	int n;		//file 받을거
	int id = 0;	//입력으로 받은 숫자의 개수 count + 배열의 끝 부분 check
	int size = 10;	//배열 크기저장
	int* ar = (int*)malloc(sizeof(int) * size);	//ar에 size 크기로 배열 설정
	printf("Input: ");	
	while (fscanf(fp, "%d", &n) != -1) 
	{
		printf("%d ", n);
		if (id >= size) 
		{	//배열의 끝이 size보다 크거나 같아졌다면 배열을 늘려줘야 함
			size *= 2;	//배열크기2배로
			int* temp = (int*)malloc(sizeof(int) * size);	//해당 크기 새로운 배열
			for (int i = 0; i < id; ++i)	//이전 배열 복사
				temp[i] = ar[i];
			free(ar);	//이전 배열 삭제
			ar = temp;	//arr이 새배열의 시작 부분을 가리키게
		}
		ar[id++] = n;	//입력값 저장
	}
	puts("");
	Sort(ar, id);
	printf("Sorted: ");
	for (int i = 0; i < id; ++i)
		printf("%d ", ar[i]);
	puts("\n");
	free(ar);
	printf("%4s %11s %-8s\n", "n", "repetitions", "time");
	for (int i = 0; i <= 100; i += 10) 
	{	//배열의 길이 0부터 100까지 10단위
		printf("%4d %11d ", i, 1000 - (i * 5));
		long sum = 0;	//시간의 합
		for (int j = 0; j < 1000 - (i * 5); ++j) 
		{
			ar = ArSet(i);		//크기별 새로운 배열을 할당
			clock_t start = clock();	//시작 시간 확인
			Sort(ar, i);		
			clock_t end = clock();		//끝 시간 확인
			sum += (end - start);		//sum 변수에 끝 시간 - 시작 시간++,
		}
		printf("%-8lf\n", sum / (1000.0 - (i * 5)));	//출력
	}
	return 0;
}