//
// Created by adrianchiri on 4/30/16.
//
#include "LRUstack.h"

void initialize(lrustack* lrus, unsigned int maxsize){
    lrus->head = NULL;
    lrus->tail = NULL;
    lrus->maxsize = maxsize;
    lrus->size = 0;
}

void push(lrustack* lrus, unsigned long pagenum){
    node* newNode = (node*)malloc(sizeof(node));
    newNode->pagenum = pagenum;

    if(lrus->size == 0){
        newNode->prev = NULL;
        newNode->next = NULL;
        lrus->head = newNode;
        lrus->tail = newNode;
        lrus->size++;
    } else {

        if (lrus->size > lrus->maxsize) {
            node* newTail = lrus->tail->prev;
            newTail->next = NULL;
            free(lrus->tail);
            lrus->tail = newTail;
            lrus->size--;
        }

        newNode->prev = NULL;
        newNode->next = lrus->head;
        lrus->head->prev = newNode;
        lrus->head = newNode;
        lrus->size++;
    }


}

int seek_and_remove(lrustack* lrsu, unsigned long pagenum){

    node* currentNode = lrsu->head;

    int depth = 0;

    while (currentNode){
        if(currentNode->pagenum == pagenum){
            //  tail
            if(!currentNode->next){
                lrsu->tail = currentNode->prev;
                lrsu->tail->next = NULL;
                free(currentNode);
            }
                //head
            else if(!currentNode->prev){
                lrsu->head = currentNode->next;
                lrsu->head->prev = NULL;
                free(currentNode);
            }
            else{
                currentNode->prev->next = currentNode->next;
                currentNode->next->prev = currentNode->prev;
                free(currentNode);
            }
            lrsu->size--;
            return depth;
        }
        currentNode = currentNode->next;
        depth++;
    }
    return -1;
}