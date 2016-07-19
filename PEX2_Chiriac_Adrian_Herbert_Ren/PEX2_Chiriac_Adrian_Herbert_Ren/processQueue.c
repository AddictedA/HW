#include <stdio.h>
#include <stdlib.h>
#include "processQueue.h"

void initQueue(queue *Q) {
    Q->head = NULL;
    Q->tail = NULL;
}

void Qinsert(queue *Q, process *proc) {
    if (!(Q->head)) {//first entry
        Q->head = malloc(sizeof(node));
        Q->tail = Q->head;
        Q->head->data = proc;
        Q->head->next = NULL;
        Q->head->prev = NULL;
    } else {//insert at back
        node *oldTail = Q->tail;
        node *newTail = malloc(sizeof(node));
        newTail->next = NULL;
        newTail->prev = oldTail;
        newTail->data = proc;
        oldTail->next = newTail;
        Q->tail = newTail;
    }
}

//should free node but not process struct
process *Qremove(queue *Q, int which) {
    node *curr = Q->head;
    while (curr && which > 0) {
        curr = curr->next;
        which--;
    }
    if (!curr) {
        return NULL;
    } else if (curr == Q->head && curr == Q->tail) {
        Q->head = NULL;
        Q->tail = NULL;
    } else if (curr == Q->head) {
        Q->head = curr->next;
        Q->head->prev = NULL;
    } else if (curr == Q->tail) {
        Q->tail = curr->prev;
        Q->tail->next = NULL;
    } else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }
    process *rtn = curr->data;
    free(curr);
    return rtn;
}

void Qprint(queue Q) {
    printf("\t%s: %s, %s, %s, %s, %s, %s, %s\n", "PID", "arrivalTime", "priority", "burstTotal", "burstRemaining",
           "initialWait", "totalWait", "requeued");
    node *curr = Q.head;
    while (curr) {
        processPrint(*(curr->data));
        curr = curr->next;
    }
}

void incrementWaitTimes(queue *Q) {
    node *curr = Q->head;
    while (curr) {
        curr->data->totalWait++;
        if (!(curr->data->requeued)) {
            curr->data->initialWait++;
        }
        curr = curr->next;
    }
}

void processPrint(process proc) {
    printf("\t%d: %d, %d, %d, %d, %d, %d, ", proc.PID, proc.arrivalTime, proc.priority, proc.burstTotal,
           proc.burstRemaining, proc.initialWait, proc.totalWait);
    if (proc.requeued) {
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }
}

void Qsort(queue *Q) {
//base case
    if (Q->head == Q->tail) {//either only 1 element or empty (shouldn't get empty unless user's list was empty)
        if (Q->head) {
            Q->head->next = NULL;
            Q->head->prev = NULL;
        }
        /*printf("base case process %d\n",Q->head->data->PID);*/return;
    }
//split
    queue left;//two new queues to hold left and right halves
    queue right;
    left.head = Q->head;//left starts all the way left
    left.tail = Q->head;
    right.head = Q->tail;//right starts all the way right
    right.tail = Q->tail;
    //step both in towards center until they cross or meet
    while (left.tail != right.head && left.tail != right.head->next) {
        left.tail = left.tail->next;
        right.head = right.head->prev;
    }//printf("sort line 100\n");
    //if they met, move one
    if (left.tail == right.head) {
        right.head = right.head->next;
    } else {
        //if they crossed, switch them
        node *tmp = left.tail;
        left.tail = right.head;
        right.head = tmp;
    }
//recurse
    Qsort(&left);
    Qsort(&right);
//merge
    //reset queue (all data is in either left or right)
    Q->head = NULL;
    Q->tail = NULL;
    node *Lcurr = left.head;
    node *Rcurr = right.head;
    //add first one
    if (Lcurr->data->PID < Rcurr->data->PID) {
        Q->head = Lcurr;
        Q->tail = Lcurr;
        Lcurr = Lcurr->next;
    } else {
        Q->head = Rcurr;
        Q->tail = Rcurr;
        Rcurr = Rcurr->next;
    }
    Q->head->prev = NULL;
    Q->head->next = NULL;
    //loop until we exhaust one queue
    /*printf("*****line 132:\nleft:");Qprint(left);printf("\nright:");Qprint(right);*/while (Lcurr && Rcurr) {
        if (Lcurr->data->PID < Rcurr->data->PID) {
            Q->tail->next = Lcurr;
            Lcurr->prev = Q->tail;
            Q->tail = Lcurr;
            Lcurr = Lcurr->next;
        } else {
            Q->tail->next = Rcurr;
            Rcurr->prev = Q->tail;
            Q->tail = Rcurr;
            Rcurr = Rcurr->next;
        }
    }//printf("*****line 144:\nleft:");Qprint(left);printf("\nright:");Qprint(right);
    //add in the rest of the other queue
    if (Lcurr) {
        Q->tail->next = Lcurr;
        Lcurr->prev = Q->tail;
        Q->tail = left.tail;
    } else {
        Q->tail->next = Rcurr;
        Rcurr->prev = Q->tail;
        Q->tail = right.tail;
    }
}

