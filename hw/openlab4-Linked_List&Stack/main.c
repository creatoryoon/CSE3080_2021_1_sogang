#define _CRT_SECURE_NO_WARNINGS
#define INF 99999999 //언더플로우 방지
#include<stdio.h>
#include<stdlib.h>
#include<string.h> //문자열 이용시 사용

//------------------------노드 생성
typedef struct {
	int data;
	struct Node *next;
} Node;
//----------------------------- 큐 정의

typedef struct {
	Node *front;
	Node *rear;
	int count;
} Queue;

void Qpush(Queue *queue, int data) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->data = data;
	node->next = NULL;
	if (queue->count == 0) {
		queue->front = node;
	}
	else {
		queue->rear->next = node;
	}
	queue->rear = node;
	queue->count++;
}
int Qpop(Queue *queue) {
	if (queue->count == 0) {
		printf("Queue underflow");
		return -INF;
	}
	Node *node = queue->front;
	int data = node->data;
	queue->front = node->next;
	free(node);
	queue->count--;
	return data;


}

void Qshow(Queue *queue) {
	Node *cur = queue->front;
	while (cur != NULL) {
		printf("%d ", cur->data);
		cur = cur->next;
	}
}


// --------------------------------스택 선언
typedef struct {
	Node *top;
} Stack;
// 스택 삽입
void Stackpush(Stack *stack, int data) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->data = data;
	node->next = stack->top;
	stack->top = node;
}
// 스택 팝
int Stackpop(Stack *stack) {
	if (stack->top == NULL) {
		printf("Stack underflow");
		return -INF;
	}
	Node *node = stack->top;
	int data = node->data;
	stack->top = node->next;
	free(node);
	return data;
}
// 스택 출력
void Stackshow(Stack *stack) {
	Node *cur = stack->top;
	while (cur != NULL) {
		printf("%d ", cur->data);
		cur = cur->next;
	}
}





int main() {
	Queue queue;
	queue.front = queue.rear = NULL;
	queue.count = 0;

	Stack stack;
	stack.top = NULL;




	int f;
	FILE* fp = fopen("./input.txt", "r");	//----------------------------------------파일 경로!--------------------
	if (fp == NULL) {	//파일 없으면 바로 종료.
		printf("Can't find file");
		return 0;
	}

	//--------------문자열 이용/-----------------------------
	//char *pState;
	//char buffer[10];
	//while (1)

	//{

	//	pState = fgets(buffer, sizeof(buffer), fp);
	//	char a[1] = "0";
	//	if (pState == NULL)

	//		break;
	//	char *c = buffer[0];
	//	char *st = buffer[0];
	//	int ret = strcmp(&st, "0");
	//	int *n, *m;
	//	if (ret == 0) {
	//		int len = strlen(buffer);
	//		//printf("%d", len);
	//		//printf(buffer);
	//		char del[] = " "; char* token;
	//		token = strtok(buffer, del);
	//		token = strtok(NULL, del);
	//		token = atoi(token);
	//		Qpush(&queue, token);
	//		Stackpush(&stack, token);
	//	}
	//	else {
	//		Qpop(&queue);
	//		Stackpop(&stack);
	//	}
	//}
	//------------------------------------------------
	while (!feof(fp)) {
		fscanf(fp, "%d", &f);
		if (f == 0) {
			fscanf(fp, "%d", &f);
			Qpush(&queue, f);
			Stackpush(&stack, f);
		}
		else if (f == 1) {
			Qpop(&queue);
			Stackpop(&stack);

		}

	}
	fclose(fp);


	Stackshow(&stack);
	printf("\n");
	Qshow(&queue);
	return 0;
}