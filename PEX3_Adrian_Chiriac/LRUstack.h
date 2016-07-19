//
// Created by adrianchiri on 4/30/16.
//

#ifndef PEX3_CHIRIAC_ADRIAN_LRUSTACK_H
#define PEX3_CHIRIAC_ADRIAN_LRUSTACK_H

#include <stdio.h>
#include <malloc.h>

/*  a node for use in an LRU stack, which is a doubly-linked list */
typedef struct s_node {
    struct s_node *prev;
    struct s_node *next;
    unsigned long pagenum;
} node;

/*  an LRU stack consisting of head and tail pointers as well as
 *  a current size and a max size. Keep the stack limited to the
 *  max size or you will run out of memory trying to run the simulation*/
typedef struct s_lrus{
    node* head;
    node* tail;
    unsigned int size;
    unsigned int maxsize;
} lrustack;

/*  initialize the LRU stack */
void initialize(lrustack* lrus, unsigned int maxsize);

/*  use pagenum when creating a new nod, which will be pushed onto
 *  the LRU stack; make sure to keep track of the LRU stack's size
 *  and free and reset the tail as necessary to limit it to max size */
void push(lrustack* lrus, unsigned long pagenum);

/*  seek pagenum in lrus and remove it if found; return the depth
 *  at whcih pagenum was found or -1 if not */
int seek_and_remove(lrustack* lrus, unsigned long pagenum);

#endif //PEX3_CHIRIAC_ADRIAN_LRUSTACK_H
