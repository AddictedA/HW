#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "TernaryTree.h"

int main(void)
{
	struct TernaryTreeNode* root = 0;
	root = loadDictionary("C:\\Dictionary.txt");

	puts("");
	puts("");
	puts("");

	if (lookup(root, "ape"))
		printf("\"ape\" word is found, test passed\n");
	else
		printf("\"ape\" word is not found, test failed\n");

	if (lookup(root, "carrot"))
		printf("\"carrot\" word is found, test passed\n");
	else
		printf("\"carrot\" word is not found, test failed\n");

	if (!lookup(root, "carrrrrrrot"))
		printf("\"carrrrrrrot\" word is not found, test passed\n");
	else
		printf("\"carrrrrrrot\" word is found, test failed\n");

	free_tree(root);

	return 0;
}