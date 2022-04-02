#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int Swap(int* a, int* b)
{	
	int c = *a;	
	*a = *b;	
	*b = c;		
}// �ٲ��ֱ�

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
}//��������


int* ArSet(int size)
{
	int* temp = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; ++i)
		temp[i] = size - i;
	return temp;
}// ����� �� ����


int main()
{
	FILE* fp = fopen("./lab1.data", "r");	
	if (fp == NULL) 
	{
		printf("���Ͼ���.");
		return 0;
	}
	int n;		//file ������
	int id = 0;	//�Է����� ���� ������ ���� count + �迭�� �� �κ� check
	int size = 10;	//�迭 ũ������
	int* ar = (int*)malloc(sizeof(int) * size);	//ar�� size ũ��� �迭 ����
	printf("Input: ");	
	while (fscanf(fp, "%d", &n) != -1) 
	{
		printf("%d ", n);
		if (id >= size) 
		{	//�迭�� ���� size���� ũ�ų� �������ٸ� �迭�� �÷���� ��
			size *= 2;	//�迭ũ��2���
			int* temp = (int*)malloc(sizeof(int) * size);	//�ش� ũ�� ���ο� �迭
			for (int i = 0; i < id; ++i)	//���� �迭 ����
				temp[i] = ar[i];
			free(ar);	//���� �迭 ����
			ar = temp;	//arr�� ���迭�� ���� �κ��� ����Ű��
		}
		ar[id++] = n;	//�Է°� ����
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
	{	//�迭�� ���� 0���� 100���� 10����
		printf("%4d %11d ", i, 1000 - (i * 5));
		long sum = 0;	//�ð��� ��
		for (int j = 0; j < 1000 - (i * 5); ++j) 
		{
			ar = ArSet(i);		//ũ�⺰ ���ο� �迭�� �Ҵ�
			clock_t start = clock();	//���� �ð� Ȯ��
			Sort(ar, i);		
			clock_t end = clock();		//�� �ð� Ȯ��
			sum += (end - start);		//sum ������ �� �ð� - ���� �ð�++,
		}
		printf("%-8lf\n", sum / (1000.0 - (i * 5)));	//���
	}
	return 0;
}