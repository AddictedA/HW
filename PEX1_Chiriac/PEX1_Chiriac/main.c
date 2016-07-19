/*=============================================================================
 |   Assignment:  PEX1
 |       Author:  Adrian Chiriac
 |        Class:  CS483
 +-----------------------------------------------------------------------------
 |   Description:  Create a shell
 |
 |   Required Features Not Included:  DESCRIBE HERE ANY REQUIREMENTS OF
 |      THE ASSIGNMENT THAT THE PROGRAM DOES NOT ATTEMPT TO SOLVE.
 |
 |
 |   Known Bugs:
 |
 |  This is very moody and unstable. Sometimes it works and sometimes it doesn't
 |
 |  The are no issues with input. Everything is parsed correctly.
 |
 |  The program also terminates correctly when exit is requested.
 |
 |  Recall works correctly and so does history
 |  One issue that I had with history is that for duplicate commands sometimes is replacing
 |      things as it should sometimes it doesn't. When it doesn't the program freezes.
 |
 |  External commands are running properly even with several arguments
 |
 |  The cd command is not cooperating at all
 |      Surprisingly it does go home when ~ is passed.
 |      But when I just pass cd it breaks with code 139
 |      There are times when it crashes directly with code 139
 |      and there are other times when it decides to sort of work.
 |      The only consistent failures are error code 139 and program freezes and does not accept any other input.
 |
 |
 |
 +-----------------------------------------------------------------------------
 |   Documentation Statement:
 |I used the following resources to learn how to build my linked list.
 |  http://www.cprogramming.com/tutorial/c/lesson15.html
 |  http://www.geeksforgeeks.org/generic-linked-list-in-c-2/
 |  http://www.learn-c.org/en/Linked_lists
 |
 |There is a long list of other resources that I have been checking (lost track of some):
 |  http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/
 |  http://www.tutorialspoint.com/cprogramming   (Various functions)
 |  http://man7.org/linux/man-pages/index.html   (Various calls)
 |  http://stackoverflow.com/questions/28502305/writing-a-simple-shell-in-c-using-fork-execvp
 |  http://codereview.stackexchange.com/questions/67746/simple-shell-in-c
 |  http://cboard.cprogramming.com/c-programming/141949-writing-c-shell.html
 |  http://www.dreamincode.net/forums/topic/146084-creating-a-simple-unix-shell-using-c/
 |  http://www.linuxquestions.org/questions/programming-9/creating-a-linux-shell-with-c-31683/
 |  http://www.linuxquestions.org/questions/programming-9/wanna-write-a-shell-27382/
 |
 |I also had to ask for help on StacksOverflow  for debugging.
 |  http://stackoverflow.com/questions/34959847/debugging-a-linked-list
 |
 *===========================================================================*/

#include "afsh.h"

int main(){
    char* currentDirectory = malloc(PATH_MAX);
    char* input = malloc(COMMAND_MAX_SIZE);
    char* inputCopy = malloc(COMMAND_MAX_SIZE);
    char** commands = NULL;
    node* history = NULL;

    //  Keep listening for commands
    while(1){

        //  Take command line input and store it
        printf("prompt> ");
        fgets(input, COMMAND_MAX_SIZE, stdin);

        //  Create deep copy of input for processing
        inputCopy = strcpy(inputCopy, input);

        //  Break down input and store the tokens
        commands = parseCommand(input, commands);

        // If the first token is recall process it
        if(strcmp(commands[0], "recall") == 0){

            //  If the second token is not given
            if( !commands[1])
                //  Warn the user he did not specify what to recall
                printf("Recall value not specified");

                // But if specified
            else
                //  Recall that command from history
                commands = recall(history, atoi(commands[1]), commands);
        }

        //  If no command do NOTHING
        if(commands[0] == NULL);

        //  But if something is a first token
        else

            //  Test first token to see if "exit"
        if (strcmp(commands[0], "exit") == 0){

            //  Inform about exiting
            printf("Exiting terminal");

            //  Clear
            fflush(stdout);

            //  And exit program
            exit(EXIT_SUCCESS);
        }
            // If first token not "exit"
        else

            //  Check to see if it's "history"
        if(strcmp(commands[0], "history") == 0){
            //  Then check second token for number of commands
            if(!commands[1])
                //  If none provided use default
                listPrintHistory(history, HISTORY);
            else
                //  But if token specified print that many
                listPrintHistory(history, atoi(commands[1]));
        }
            //  If first token not "exit" or "history"
        else
            //  Check to see if it's "cd"
        if(strcmp(commands[0], "cd") == 0){
            //  No matter of validity add history
            history = addHistory(inputCopy, history);
            //  and go to specified path updating currentDirectory
            cd(commands[1], currentDirectory);
        }
            //  But if first token none of the previous
        else{
            //  Run the command
            runCommand(commands);
            //  And store a copy in the history
            history = addHistory(inputCopy, history);
        }
    }
    return 0;
}



