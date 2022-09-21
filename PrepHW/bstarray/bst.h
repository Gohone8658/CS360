#ifndef BST_H
#define BST_H

typedef struct _bst{
	int size;
	int *tree;
} BST;

BST* createEmptyTree(int size);
void growTree(BST* bst);
int insertValue(BST* bst, int value);
int findValue(BST* bst, int value);
int deleteValue(BST* bst, int value);

#endif
