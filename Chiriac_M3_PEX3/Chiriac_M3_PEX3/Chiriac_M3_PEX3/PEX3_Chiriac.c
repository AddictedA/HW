#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "TernaryTree.h"

struct line
{
	char* text;
	int number;
	struct line* next;
};

void usage()
{
	printf("Invalid command line parameters");
}

struct line* readInputFile(const char* fileName)
{
	struct line* head = 0;
	struct line* tail = 0;
	FILE* file = fopen(fileName, "r");

	if (file)
	{
		int lineNumber = 1;
		while (!feof(file))
		{
			char buffer[10000];
			memset(buffer, 0, sizeof(10000));
			fgets(buffer, sizeof(buffer), file);

			if (!head)
			{
				head = (struct line*)malloc(sizeof(struct line));
				if (head)
				{
					head->text = (char*)malloc(strlen(buffer) + 1);
					if (head->text)
						strcpy(head->text, buffer);
					head->number = lineNumber;
					head->next = NULL;
					tail = head;
				}
				else {
					return 0;
				}
			}
			else {
				tail->next = (struct line*)malloc(sizeof(struct line));
				if (tail->next)
				{
					tail->next->text = (char*)malloc(strlen(buffer) + 1);
					if (tail->next->text)
						strcpy(tail->next->text, buffer);

					tail->next->number = lineNumber;
					tail->next->next = NULL;
					tail = tail->next;
				}
				else {
					return head;
				}
			}
			lineNumber++;
		}
		fclose(file);
	}
	return head;
}

void writeOutputFile(struct line* content, const char* fileName)
{
	struct line* node = content;
	FILE* file = fopen(fileName, "w");

	if (file)
	{
		while (node)
		{
			if (node->text)
				fprintf(file, node->text);
			node = node->next;
		}
		fclose(file);
	}
}

int isValidWord(const char* word)
{
	int l = strlen(word);
	int i;

	for (i = 0; i<l; i++)
	if (!isalpha(word[i]))
		return 0;

	return l>0;
}

void processLine(struct TernaryTreeNode* root, struct line* node, int* pMisspeledCount)
{
	char* copy = _strdup(node->text);
	if (copy)
	{
		char* beginOfString = copy;
		char* word = strtok(copy, " \t\n");
		while (word)
		{
			if (isValidWord(word))
			{
				int l = strlen(word);
				_strlwr(word);

				if (!lookup(root, word))
				{
					int option = 0;

					printf("\nFound Misspelled word: \'%s\' (line number = %d)\n", word, node->number);
					(*pMisspeledCount)++;
					do
					{
						printf("Choose option: \n");
						printf("1. Replace\n");
						printf("0. Next\n\n");
						printf("Your choice: ");
						scanf("%d", &option);
						if (option == 1)
						{
							char replaced[1000];
							printf("Enter word for replacing: ");
							scanf("%s", replaced);
							if (!lookup(root, replaced))
							{
								printf("\'%s\' word is misspelled too!", replaced);
							}
							else {
								int startPos = (int)(word - beginOfString);
								int afterLen = 0;
								char *replacedLine;
								afterLen = strlen(&word[l + 1]);

								replacedLine = (char*)malloc(startPos + afterLen + strlen(replaced) + 1);

								if (replacedLine)
								{
									char space[2] = { 0, 0 };

									printf("Performing replace...\n");
									strncpy(replacedLine, node->text, startPos);
									replacedLine[startPos] = 0;
									strcat(replacedLine, replaced);
									space[0] = node->text[startPos + l];
									strcat(replacedLine, space);
									strcat(replacedLine, &word[l + 1]);

									free(node->text);
									free(copy);

									node->text = replacedLine;
									copy = _strdup(node->text);
									beginOfString = copy;
									word = strtok(copy, " \t\n");
									option = 0;
								}
							}
						}
						else if (option != 0)
						{
							printf("Invalid option, please try again\n");
						}
					} while (option != 0);
				}
			}
			word = strtok(NULL, " \n");
		}
		free(copy);
	}
}

void process(const char* inputFileName, const char* outputFileName)
{
	struct TernaryTreeNode* root = 0;
	struct line* inputContent;
	struct line* node;
	root = loadDictionary("C:\\Dictionary.txt");

	if (root)
	{
		int misspeledCount = 0;
		inputContent = readInputFile(inputFileName);
		node = inputContent;
		while (node)
		{
			if (node->text)
			{
				processLine(root, node, &misspeledCount);
			}

			node = node->next;
		}
		printf("\nTotal misspelled words: %d\n", misspeledCount);
		writeOutputFile(inputContent, outputFileName);
	}

	free_tree(root);
}

int main(int argc, char* argv[])
{
	char* inputFileName;
	char* outputFileName;

	if (argc >= 2)
	{
		inputFileName = argv[1];
		outputFileName = argv[1];

		if (argc == 4)
		{
			if (!strcmp(argv[2], "-o"))
				outputFileName = argv[3];
		}
		else {
			usage();
			return 1;
		}

		process(inputFileName, outputFileName);

	}
	else {
		usage();
	}
	return 0;
}