#define _CRT_SECURE_NO_WARNINGS
#ifndef TERNARTY_TREE
#define TERNARTY_TREE

struct TernaryTreeNode
{
	char c;

	struct TernaryTreeNode* left;
	struct TernaryTreeNode* right;
	struct TernaryTreeNode* down;
};


struct TernaryTreeNode* loadDictionary(const char* filename);
struct TernaryTreeNode* insert(struct TernaryTreeNode* rootNode, const char* word);
int lookup(struct TernaryTreeNode* rootNode, const char* word);
void free_tree(struct TernaryTreeNode* rootNode);


#endif