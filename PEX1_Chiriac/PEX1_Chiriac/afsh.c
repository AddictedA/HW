#include <unistd.h>
#include <sys/wait.h>
#include "afsh.h"

/* parseCommand: Breaks input down into tokens
   Parameters
   - char*: a pointer to the user input
   - char**: array of pointer to individual tokens
   Return: a pointer to the head of the commands */
char** parseCommand(char *input, char** commands){
    char* token;

    //  Initial memory allocation
    commands = malloc(sizeof(char*));

    //  Remove the "Enter" ("/n")
    input = strtok(input, "\n");

    int i = 0;
    token = strtok(input, " \t");

    while(token != NULL){
        commands[i] = strdup(token);
        token = strtok(NULL, " \t");
        i++;
    }
    commands[i] = NULL;
    return commands;
//
//    //  Infinite loop until NULL breaks it
//    while (1){
//
//        if(i == 0)
//            //  Take tokens from input
//            token = strtok(input, " ");
//        else
//            token = strtok(NULL, " ");
//
//        if(token == NULL)
//            break;
//        else {
//            commands[i] = token;
//            i++;
//            //  Expand memory size
//            commands = realloc(commands, i * sizeof(char*));
//        }
//    }
//    //  Return head pointer for commands
//    return commands;
}

/* recall: travers history linked list and retrieve command
   Parameters
   - node*:  a pointer to the head of history linked list
   - int:    command number to be retrieved
   - char**: array of pointers to commands
   Return: a pointer to the head of the commands */
char** recall(node* history, int number, char** commands){
    //  Retrieve the specified command from history
    return parseCommand(list_get(history, list_get_size(history) + 1 - number), commands);
}

/* addHistory: create a new node at the end of the history linked list
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   Return: a pointer to the head of the linked list */
node* addHistory(char* input, node* history) {
    //  If first 6 chars are "recall"
    if (strncmp(input, "recall", 6) == 0)
        //  Return history
        return history;

    //  Remove "Enter"
    input = strtok(input, "\n");

    //  Remove duplicates
//    if (history != NULL) {
    //This should be already tested in my remove function
    // That's the way I designed it
        history = list_remove(history, input);
//    }

    //  Insert new node and return point to head
    history = list_insert_tail(history, input);
    return history;
}

/* cd: executes cd command
   Parameters
   - char*: a pointer to the head of new directory
   - char*: a pointer to the head of CWD
   Return: a pointer to the head of  CWD */
void cd(char *newDirectory, char *currentDirectory) {
    //  if no arguments passed to cd or "~" is passed
    if (!strcmp(newDirectory, "~") || !strcmp(newDirectory, ""))
        //  We end up HOME
        newDirectory = strdup(getenv("HOME"));

    if (chdir(newDirectory) !=0)
        printf("We could not change directory");
    else {
        //  Update current working directory
        currentDirectory = (char *) (long) getcwd(currentDirectory, PATH_MAX);
        //  Inform user about his location
        printf("%s ", currentDirectory);
    }
}

int runCommand(char** args) {
    pid_t ret_val;
    int x = 0;
    ret_val = fork();

    if (ret_val != 0) {
        pid_t pid_term = wait(&x);
        printf("Terminated process %d\n", pid_term);
    }
    else {
        execvp(args[0], args);
        printf("command not found\n");
        exit(0);
    }

    return 0;
}

//    pid_t pid;
//
//    pid = fork();
//
//    if (pid < 0)
//    else
//    if (pid == 0 )
//        execvp(args[0], args);
//    else{
//        wait(NULL);
//        }
//    return 0;
//}