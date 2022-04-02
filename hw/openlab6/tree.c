#include <stdio.h>
#include <stdlib.h>
typedef struct threadedTree *threadedPointer;
typedef struct threadedTree {
	short int leftThread;
	threadedPointer leftChild;
	char data;
	threadedPointer rightChild;
	short int rightThread;
} threadedTree;
threadedPointer insucc(threadedPointer tree) {
	threadedPointer temp = tree->rightChild;

	if (!tree->rightThread) {
		while (!temp->leftThread) {
			temp = temp->leftChild;
		}
	}
	return temp;
}
void tinorder(threadedPointer tree) {
	threadedPointer temp = tree;
	
	for (;;) {
		temp = insucc(temp);
		if (temp == tree) {
			break;
		}
		printf("%c ", temp->data);
	}

}
void insertRight(threadedPointer s, threadedPointer r) {
	r->rightChild = s->rightChild;
	r->rightThread = s->rightThread;
	r->leftChild = s;
	r->leftThread = 1;
	s->rightChild = r;
	s->rightThread = 0;
	if (!r->rightThread) {
		threadedPointer temp = insucc(r);
		temp->leftChild = r;
	}
	return;
}
void insertLeft(threadedPointer s, threadedPointer l) {
	l->leftChild = s->leftChild;
	l->leftThread = s->leftThread;
	l->rightChild = s;
	l->rightThread = 1;
	s->leftChild = l;
	s->leftThread = 0;
	if (!l->leftThread) {
		threadedPointer temp = l->leftChild;

		while (!temp->rightThread) {
			temp = temp->rightChild;
		}
		temp->rightChild = l;
	}
	return;
}
threadedPointer getNewNode(char data) {
	threadedPointer newNode = (threadedPointer) malloc(sizeof(threadedTree));
	
	newNode->leftThread = 1;
	newNode->leftChild = newNode;
	newNode->data = data;
	newNode->rightChild = newNode;
	newNode->rightThread = 0;
	
	return newNode;
}
int main() {
	/* image 5.23 */
	threadedPointer root = getNewNode('-');
	insertLeft(root, getNewNode('A'));
	insertLeft(root->leftChild, getNewNode('B'));
	insertLeft(root->leftChild->leftChild, getNewNode('D'));
	insertLeft(root->leftChild->leftChild->leftChild,
		getNewNode('H'));
	insertRight(root->leftChild->leftChild->leftChild,
		getNewNode('I'));
	insertRight(root->leftChild->leftChild, getNewNode('E'));
	insertRight(root->leftChild, getNewNode('C'));
	insertLeft(root->leftChild->rightChild, getNewNode('F'));
	insertRight(root->leftChild->rightChild, getNewNode('G'));
	tinorder(root);
	return 0;
}









