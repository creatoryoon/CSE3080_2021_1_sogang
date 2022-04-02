#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct _student {
	char id[10];
	char name[40];
	char course[40];
	int score;
} STUDENT;

typedef struct _treenode {
	STUDENT key;
	struct _treenode *left, *right;
} TreeNode;

int InsertNode(TreeNode* root, STUDENT key);
TreeNode* NewNode(STUDENT key);
int DeleteNode(TreeNode* root, char *id);
void SearchCourse(TreeNode* node, char *course);
void SearchCourseRecursive(TreeNode* node, char *course, int *cnt, int *sum);
void SearchName(TreeNode* node, char *name);
int SearchNameRecursive(TreeNode *node, char *name);
int SearchID(TreeNode* node, char *id);
int SearchIDRecursive(TreeNode* node, char *id);
void printStudent(STUDENT key);

int main() {
	TreeNode *root = NULL;
	STUDENT tmp;
	int i, n;

	FILE *fp = NULL;
	fp = fopen("input.txt", "r");

	if (fp == NULL) {
		printf("파일 못 읽음.");
	}
	else {
		for (i = 0; i < 15; i++) {
			fscanf(fp, "%s %s %s %d", tmp.id, tmp.name, tmp.course, &tmp.score);
			if (root == NULL) {
				root = NewNode(tmp);
			}
			else {
				InsertNode(root, tmp);
			}
		}
	}

	int menu = 0, x;
	char value[128];

	while (menu != -1) {
		printf("-- 메뉴 --\n");
		printf("-1. 종료\n");
		printf("1. 학생 삽입\n");
		printf("2. 학생 삭제\n");
		printf("3. Course 검색\n");
		printf("4. Name 검색\n");
		printf("5. id 검색\n");
		printf("입력 : ");
		scanf("%d", &menu);

		if (menu == 1) {
			printf("삽입할 학생 입력 : (id name course score)");
			scanf("%s %s %s %d", tmp.id, tmp.name, tmp.course, &tmp.score);
			x = InsertNode(root, tmp);
			if (x == 1) {
				printf("성공입니다.\n");
			}
		}

		else if (menu == 2) {
			printf("삭제할 학생 id 입력 : ");
			scanf("%s", value);
			DeleteNode(root, value);
		}

		else if (menu == 3) {
			printf("Course 입력 : ");
			scanf("%s", value);
			SearchCourse(root, value);
		}
		else if (menu == 4) {
			printf("Name 입력 : ");
			scanf("%s", value);
			SearchName(root, value);
		}
		else if (menu == 5) {
			printf("ID 입력 : ");
			scanf("%s", value);
			SearchID(root, value);
		}
	}

	return 0;
}

int InsertNode(TreeNode* root, STUDENT key) {
	// a = 부모 노드, n = 현재 노드
	TreeNode *a, *n;

	a = NULL;

	//탐색해서 삽입 가능한지 알아보기
	n = root;
	while (n != NULL) {
		if (strcmp(n->key.id, key.id) == 0) {
			printf("id 중복입니다.(삽입 불가능합니다.)\n");
			return 0;
		}

		a = n;
		if (strcmp(key.id, n->key.id) > 0) {
			n = n->left;
		}
		else {
			n = n->right;
		}
	}

	TreeNode *b = NewNode(key); //b = 새로운 노드 

	if (strcmp(key.id, a->key.id) > 0) {
		a->left = b;
	}

	else {
		a->right = b;
	}

	return 1;
}


TreeNode* NewNode(STUDENT key) {
	TreeNode *b = (TreeNode *)malloc(sizeof(TreeNode));
	if (b == NULL) return 0;

	//복사  
	memcpy(&(b->key), &key, sizeof(STUDENT));
	b->left = b->right = NULL;
	return b;
}


int DeleteNode(TreeNode* root, char *id) {

	TreeNode *a, *c, *n, *s, *s_;

	a = NULL;
	n = root;

	while (n != NULL && strcmp(n->key.id, id) != 0) {
		a = n;
		if (strcmp(id, n->key.id) > 0) {
			n = n->left;
		}
		else {
			n = n->right;
		}
	}

	if (n == NULL) {
		printf("없어서 삭제가 불가능합니다.");
		return 0;
	}

	//단일노드 
	if (n->left == NULL && n->right == NULL) {

		if (a != NULL) {
			if (a->left == n) {
				a->left = NULL;
			}
			else {
				a->right = NULL;
			}
		}

		else {
			root->left = NULL;
			root->right = NULL;
		}
	}
	//자식 하나 
	else if ((n->left == NULL) || (n->right == NULL)) {

		if (n->left != NULL) {
			c = n->left;
		}

		else {
			c = n->right;
		}


		if (a != NULL) {
			if (a->left == n) {
				a->left = c;
			}
			else {
				a->right = c;
			}
		}

		else {
			root->key = c->key;
			root->left = c->left;
			root->right = c->right;
		}
	}
	//자식 둘 
	else {
		s_ = n;
		s = n->right;

		while (s->left != NULL) {
			s_ = s;
			s = s->left;
		}

		if (s_->left == s) {
			s_->left = s->right;
		}

		else {
			s_->right = s->right;
		}

		n->key = s->key;
		n = s;
	}
	printf("성공입니다.");
	return 1;
}


void SearchCourse(TreeNode* node, char *course) {
	printf("%s 과목을 찾습니다.\n", course);
	int cnt = 0;
	int sum = 0;

	SearchCourseRecursive(node, course, &cnt, &sum);
	if (cnt == 0) {
		printf("해당 과목을 듣는 학생이 없습니다.\n");
	}
	else {
		printf("수강 인원 : %d명\n", cnt);
		printf("평균 점수 : %.2f점\n", (float)sum / cnt);
	}
}

void SearchCourseRecursive(TreeNode* node, char *course, int *cnt, int *sum) {
	if (node == NULL) {
		return;
	}
	if (strcmp(node->key.course, course) == 0) {
		printStudent(node->key);
		*cnt += 1;
		*sum += node->key.score;
	}
	SearchCourseRecursive(node->left, course, cnt, sum);
	SearchCourseRecursive(node->right, course, cnt, sum);
}


void SearchName(TreeNode* node, char *name) {
	SearchNameRecursive(node, name);
}

int SearchNameRecursive(TreeNode *node, char *name) {
	if (node == NULL) {
		return 0;
	}
	if (strcmp(node->key.name, name) == 0) {
		printStudent(node->key);
		//return 1;
	}
	//else if (SearchNameRecursive(node->left, name) == 1) {
	//	return 1;
	//}
	//else if (SearchNameRecursive(node->right, name) == 1) {
	//	return 1;
	//}
	//수정
	SearchNameRecursive(node->left, name);
	SearchNameRecursive(node->right, name);
}


int SearchID(TreeNode* node, char *id) {
	if (SearchIDRecursive(node, id)) {
		printf("학생을 찾았습니다.\n");
		return 1;
	}
	else {
		printf("학생을 찾지 못했습니다.\n");
		return 0;
	}
}
int SearchIDRecursive(TreeNode* node, char *id) {
	if (node == NULL) {
		return 0;
	}
	if (strcmp(node->key.id, id) == 0) {
		printStudent(node->key);
		return 1;
	}
	else if (SearchIDRecursive(node->left, id) == 1) {
		return 1;
	}
	else if (SearchIDRecursive(node->right, id) == 1) {
		return 1;
	}
	return 0;
}


void printStudent(STUDENT key) {
	printf("ID: %s, Name: %s, score: %d\n", key.id, key.name, key.score);
}