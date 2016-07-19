#include <malloc.h>
#include <string.h>
#include "list.h"

/* list_insert_tail: create a new node at the end of the linked list
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   Return: a pointer to the head of the linked list */
node* list_insert_tail(node* list, char* input){

    // Best case:
    // List is empty and we just add the node
    if(list == NULL){
        //  Allocate memory base on the size of the data structure
        node *list = malloc(sizeof(struct s_node));

        list->data = strdup(input);

        list->next = NULL;

        return list;
    }

    // But we could be at the last node
    if(list->next == NULL){

        // Allocate memory for the new node to be attached to the list
        node *newNode = malloc(sizeof(struct s_node));

        // Link the new node to the tail of the list
        list->next = newNode;

        newNode->data = strdup(input);

        return list;
    }

    // If we are at a random node advance until the end
    list->next = list_insert_tail(list->next, input);
    return list;
}

/* list_remove: remove the node containing specific data; if multiple nodes
    contain the data, remove the node nearest to the head
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data that, if found in one of the nodes in the list, indicates
      the node to remove (in the case of multiple nodes containing the data,
	  remove the node nearest to the head)
   Return: a pointer to the head of the linked list */

node* list_remove(node* list, char* input){
    //  If empty list
    if(list == NULL)
        return NULL;
    node* current = list;

    //  If head of list
    if(strcmp(current->data, input) == 0){
        node *NewHead = current->next;
        free(current->data);
        free(current);
        return NewHead;
    }

    //  Anywhere in list
    while(current->next != NULL){
                // OMF
        // THIS MF WASTED HOURS FROM MY TIME BECAUSE I DID NOT POINT TO THE DATA IN NEXT
        if(strcmp(current->next->data, input) == 0){
            node* NewNextNode = current->next->next;
            free(current->next->data);
            free(current->next);
            current->next = NewNextNode;
            return list;
        }
        current = current->next;
    }
    return list;
}
//node* list_remove(node* list, char* input){
//    //As long as we have elements
//    if(list != NULL){
//        // We check to see if they have the same data
////        if(list->data == input){
//        if(strcmp(list->data, input) == 0){
//            // We remove the link
//            node *newNextNode = list->next;
//
//            // Remove the data
//            free(list->data);
//
//            // An remove the node itself when done
//            free(list);
//            return newNextNode;
//        }
//            //If it's not the correct data we move forward through the list
//        else{
//            // And check following nodes recursively
//            list->next = list_remove(list->next, input);
//            return list;
//        }
//    }
//        // But if we reach the end of the list
//    else{
//        // We print a warning message
//        printf("Specified data does not exist in any of the nodes");
//        return list;
//    }
//}

/* list_get: retrieve the data stored in the <n>th node in the list
   Parameters:
   - node*: a pointer to the head of a linked list
   - int: the position of the node with the data to retrieve, where the head is
       at position 1
   Return: the data */
char* list_get(node* list, int n){
    if(list != NULL){   // As long as the list is not empty
        if(n <= 1){     // And I have reached my node
            return list->data;  // Give me my data
        }

        else{   // But if I'm not there just keep going
            return list_get(list->next, n-1);   // Until you are done
        }
    }

    else{   // And if you reached the end and the nth node does not exist
            // Or the list is empty print warning.
        printf("Slow down buddy! That element is outside of list length");
        return NULL;
    }
}

int list_get_size(node* list){
    int i = 0;
    node* current = list;
    while(current != NULL){
        i++;
        current = current->next;
    }
    return i;
}

/* list_print: print the data contained in each node in the linked list,
    starting with the head, where each node's data is printed on a separate
    line
   Parameter: node*: a pointer to the head of a linked list */
//void list_print(node* list) {
//    if (list != NULL) {   // As long as the list is not empty
//        printf("%s\n", list->data); // Print the data.
//
//        if (list->next != NULL) {  // While you have not reached the end
//            list_print(list->next); // Keep moving forward
//        }
//    }
//
//    else {   // Terminate if you reach end of the list (AKA list is empty)
//        printf("You have reached the end of your list");
//    }
//}

/* listPrintHistory: formatted print of linked list required
   for reverse and number printing
   Parameters:
   - node*: a pointer to the head of a linked list */
void listPrintHistory(node* list, int count) {
    node* current = list;
    int size = list_get_size(list);
    while(current != NULL) {
        if(size <= count){
            printf("%d:\t%s\n", size, current->data);
        }
        size--;
        current = current->next;
    }
}

