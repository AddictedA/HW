#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#define COMMAND_MAX_SIZE 128
#define HISTORY 10

/* parseCommand: Breaks input down into tokens
   Parameters
   - char*: a pointer to the user input
   - char**: array of pointer to individual tokens
   Return: a pointer to the head of the commands */
char** parseCommand(char *input, char** commands);

/* recall: travers history linked list and retrieve command
   Parameters
   - node*:  a pointer to the head of history linked list
   - int:    command number to be retrieved
   - char**: array of pointers to commands
   Return: a pointer to the head of the commands */
char** recall(node* history, int number, char** commands);

/* addHistory: create a new node at the end of the history linked list
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   Return: a pointer to the head of the linked list */
node* addHistory(char* input, node* history);

/* cd: executes cd command
   Parameters
   - char*: a pointer to the head of new directory
   - char*: a pointer to the head of CWD
   Return: a pointer to the head of  CWD */
void cd(char *newDirectory, char *currentDirectory);

/* runCommand: Executes commands
   Parameters
   - args**: arguments to be run
   Return: 0 if complete execution*/
int runCommand(char** args);
