#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
// 연결 리스트를 위한 노드 정의
typedef struct {
	int data;
	struct NODE *next;
}NODE;
// 원형 리스트가 몇개의 노드인지 저장하기 위한 index, 기준 노드를 정하기 위한 tail 선언
typedef struct list {
	int countIndex;
	NODE* tail;
}LIST;




LIST* createList() {
	LIST* list = (LIST*)malloc(sizeof(LIST));

	if (list == NULL)
	{
		printf("error\n");
	}
	else
	{
		list->tail = NULL;
		list->countIndex = 0;
	}

	return list;
}

void addNode(LIST* list, NODE element) {
	NODE *preNode = list->tail;
	NODE *newNode = (NODE*)malloc(sizeof(NODE));

	//데이터 삽입
	newNode->data = element.data;

	if (list->countIndex == 0)
	{
		newNode->next = newNode;
		list->tail = newNode;
	}
	else
	{
		newNode->next = preNode->next;
		preNode->next = newNode;

		list->tail = newNode;
	}

	list->countIndex++;
}

void delNode(LIST* list, int position) {
	NODE *preNode = list->tail;
	NODE *delNode = list->tail;
	position = position - 1;
	if (list->countIndex == 0)
	{
		printf("노드가 없습니다.");
	}
	else
	{

		for (int i = 0; i < position; i++)
		{
			preNode = preNode->next;
		}

		delNode = preNode->next;

		preNode->next = delNode->next;
		free(delNode);

		if (position == list->countIndex - 1)
		{
			list->tail = preNode;
		}

		list->countIndex--;
	}
}

void display(LIST* list) {
	int i;
	NODE* node;

	for (node = list->tail->next, i = 0; i < list->countIndex; i++, node = node->next) {
		printf("%d ", node->data);
	}
}

void merge(LIST*A, LIST*B) {	//merge 후에는 B는 삭제후 A에 합침
	NODE *prenode = A->tail->next;
	NODE *pronode = B->tail->next;
	A->countIndex += B->countIndex;
	A->tail->next = pronode;
	B->tail->next = prenode;
	A->tail = B->tail;
	prenode = A->tail->next;
	free(B);

}


void findDel(LIST *list, int n)//값비교로 삭제(모두삭제)
{
	NODE *delNode = list->tail;
	NODE *preNode = list->tail;
	int i = 0;
	while (i != list->countIndex)
	{
		delNode = preNode->next;
		if (delNode->data == n)
		{
			preNode->next = delNode->next;
			if (i == list->countIndex - 1)
			{
				list->tail = preNode; // 마지막꺼인경우 테일 수정
			}
			free(delNode);
			list->countIndex--;
			return findDel(list, n);			// 삭제할게 있는경우 더있나 다시 확인.
		}	
		preNode = preNode->next;
		i++;
	}
}




int main() {
	LIST* A;
	LIST* B;
	LIST* C;
	NODE node;
	A = createList();
	B = createList();

	int f;
	// 파일 A,B 불러다 리스트 만들고
	FILE* fp1 = fopen("./A.txt", "r");	//----------------------------------------파일 경로!--------------------
	FILE* fp2 = fopen("./B.txt", "r");
	if (fp1 == NULL) {	//파일 없으면 바로 종료.
		printf("Can't find file1");
		return 0;
	}
	if (fp2 == NULL) {	//파일 없으면 바로 종료.
		printf("Can't find file2");
		return 0;
	}
	while (EOF != fscanf(fp1, "%d", &f)){
		node.data = f;
		addNode(A, node);
	}
	while (EOF != fscanf(fp2, "%d", &f)) {
		node.data = f;
		addNode(B, node);
	}
	fclose(fp1);
	fclose(fp2);

	merge(A, B); //합치고

	display(A); //보여주고
	printf("\n");

	FILE* fp3 = fopen("./C.txt", "r");
	while (EOF != fscanf(fp3, "%d", &f)) {
		int d = f;
		findDel(A, d); // C.txt의 숫자를 비교로 찾는경우

		//delNode(A, d); // C.txt가 포지션인경우
	}
	fclose(fp3);

	display(A);

	return 0;
}

