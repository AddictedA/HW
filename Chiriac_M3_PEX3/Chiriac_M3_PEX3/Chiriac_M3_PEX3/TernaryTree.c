#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TernaryTree.h"

struct TernaryTreeNode* insert_helper(struct TernaryTreeNode* root,
	char** wordArray,
	int leftIndex,
	int rightIndex)
{
	if (leftIndex <= rightIndex)
	{
		int middle = (leftIndex + rightIndex) / 2;
		root = insert(root, wordArray[middle]);
		root = insert_helper(root, wordArray, leftIndex, middle - 1);
		root = insert_helper(root, wordArray, middle + 1, rightIndex);
	}
	return root;
}

struct TernaryTreeNode* loadDictionary(const char* fileName)
{
	FILE* file;
	char word[1000];
	char** wordArray;
	int i, l, wordCount;
	struct TernaryTreeNode* root = 0;

	wordCount = 0;
	file = fopen(fileName, "r");

	if (file)
	{
		while (!feof(file))
		{
			word[0] = 0;
			fgets(word, sizeof(word), file);
			wordCount++;
		}

		fseek(file, 0, SEEK_SET);
		wordArray = (char**)malloc(sizeof(char*)*wordCount);

		i = 0;
		while (!feof(file))
		{
			word[0] = 0;
			fgets(word, sizeof(word), file);
			l = strlen(word);

			if (word[l - 1] == '\n')
				word[l - 1] = 0;

			wordArray[i] = _strdup(word);
			i++;
		}

		root = insert_helper(root, wordArray, 0, wordCount - 1);

		for (i = 0; i<wordCount; i++)
			free(wordArray[i]);

		free(wordArray);
		fclose(file);
	}

	return root;
}

void insert_down(struct TernaryTreeNode* node, const char* word)
{
	int index = 0;
	do
	{
		index++;
		node->down = (struct TernaryTreeNode*)malloc(sizeof(struct TernaryTreeNode));
		node->down->down = 0;
		node->down->left = 0;
		node->down->right = 0;
		node->down->c = word[index];
		node = node->down;
	} while (word[index]);
}

struct TernaryTreeNode* insert(struct TernaryTreeNode* rootNode, const char* word)
{
	if (!rootNode)
	{
		rootNode = (struct TernaryTreeNode*)malloc(sizeof(struct TernaryTreeNode));
		rootNode->down = 0;
		rootNode->left = 0;
		rootNode->right = 0;
		rootNode->c = word[0];
		insert_down(rootNode, word);
		return rootNode;
	}
	else {
		int index = 0;
		int length = strlen(word);
		struct TernaryTreeNode* node = rootNode;

		while (node && index <= length)
		{
			if (node->c > word[index])
			{
				if (node->left)
					node = node->left;
				else
				{
					node->left =
						(struct TernaryTreeNode*)malloc(sizeof(struct TernaryTreeNode));
					node->left->left = 0;
					node->left->right = 0;
					node->left->down = 0;
					node->left->c = word[index];

					insert_down(node->left, &word[index]);
				}
			}
			else if (node->c < word[index])
			{
				if (node->right)
					node = node->right;
				else
				{
					node->right =
						(struct TernaryTreeNode*)malloc(sizeof(struct TernaryTreeNode));
					node->right->left = 0;
					node->right->right = 0;
					node->right->down = 0;
					node->right->c = word[index];

					insert_down(node->right, &word[index]);
				}
			}
			else {
				node = node->down;
				index++;
			}
		}
		return rootNode;
	}
}

int lookup(struct TernaryTreeNode* rootNode, const char* word)
{
	if (rootNode)
	{
		int index = 0;

		struct TernaryTreeNode* node = rootNode;

		while (node && word[index])
		{
			if (node->c > word[index])
			{
				node = node->left;
			}
			else if (node->c < word[index])
			{
				node = node->right;
			}
			else {
				node = node->down;
				index++;
			}
		}

		if (!word[index])
			return 1;
	}

	return 0;
}

void free_tree(struct TernaryTreeNode* rootNode)
{
	if (rootNode)
	{
		free_tree(rootNode->left);
		free_tree(rootNode->right);
		free_tree(rootNode->down);
		free(rootNode);
	}
}
