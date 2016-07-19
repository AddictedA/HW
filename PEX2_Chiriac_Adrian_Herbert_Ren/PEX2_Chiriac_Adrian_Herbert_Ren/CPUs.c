#include <stdio.h>
#include "CPUs.h"
#include "processQueue.h"
/*=============================================================================
 |   Assignment:  PEX2 - CPU Scheduling
 |       Author:  Adrian Chiriac, Ren Herbert
 |        Class:  CS483
 +-----------------------------------------------------------------------------
 |   Description:  Implements a simulation of different common scheduling
 |      algorithms used by Operating Systems.
 |
 |   Required Features Not Included:  None
 |
 |
 |   Known Bugs:  None that we are aware of.
 |
 |   RESULTS:
 |   FCFS -> N/A
 |   SJF -> Matches except for one process which is a completely different process
 |      (different arrival time, priority and burst time), follows algorithm
 |      correctly though.
 |   RR -> Works in the same order, but the wait times are slighty different. This
 |      due to an arrival v reschedule discrepancy
 |   NPP -> Runs slightly different based off wether you look at arrivals on the time
 |      time that they appear or on the next go around
 |   SRTF -> Runs slightly different, I think it has something to do with when you
 |      count a piece of the burstRemaining off.
 |   PP -> The programs run in the right order but the wait times are different becuase
 |      we increased them at a different time than the solution.
 +-----------------------------------------------------------------------------
 |   Documentation Statement:  Stated above each algorithm
 *===========================================================================*/
// Provided solution to FCFC algorithm
void *FCFScpu(void *param) {
    int myID = ((cpuParams *) param)->threadNumber;
    sharedVars *shared = ((cpuParams *) param)->svars;
    process *myProcess = NULL;
    while (1) {
        //wait for "uptick" from main
        sem_wait(&(shared->cpuSems[myID]));
        //FCFS only schedules when the last process has completed
        if (!myProcess) {
            //pull first process from queue
            pthread_mutex_lock(&(shared->readyQLock));
            myProcess = Qremove(&(shared->readyQ), 0);
            if (myProcess) {
                printf("CPU %d - Scheduling PID %d\n", myID,myProcess->PID);
                processPrint(*myProcess);
            } else {
                printf("CPU %d - No process to schedule\n", myID);
            }
            pthread_mutex_unlock(&(shared->readyQLock));
        }
        if (myProcess) {
            //execute that process for one timestep
            (myProcess->burstRemaining)--;
            //if process is done processing, move to finish
            if (myProcess->burstRemaining <= 0) {
                pthread_mutex_lock(&(shared->finishedQLock));
                Qinsert(&(shared->finishedQ), myProcess);
                pthread_mutex_unlock(&(shared->finishedQLock));
                myProcess = NULL;
            }
        } //else do nothing if no process to run
        //send "downtick" to main once complete
        sem_post(&(shared->mainSem));
    }
}


// Traverse Ready Queue and find minimal Burst time
// Input: Q - Queue pointer
// Output: min_pos - integer position in Queue
int findMin_burst(queue* Q){
    int i = 0;
    int min_pos = 0;

    if(Q->head) {
        int min = Q->head->data->burstTotal;
        node *next = Q->head;
        //iterate through list
        while (next != Q->tail && next != NULL) {
            if (next->data->burstTotal < min) { //check if new lowest
                min = next->data->burstTotal;
                min_pos = i;
            }
            i++;
            next = next->next;
        }
    }
    return min_pos;
}

// You will implement the remaining algorithms below

//  Determine which is the shortest job and execute to completion
//  Documentation: https://www.youtube.com/watch?v=LNsHRY5Nk5U
void *SJFcpu(void *param) {
    int myID = ((cpuParams *) param)->threadNumber;
    sharedVars *shared = ((cpuParams *) param)->svars;
    process *myProcess = NULL;
    while (1) {
        //wait for "uptick" from main
        sem_wait(&(shared->cpuSems[myID]));
        //NPP only schedules when the last process has completed
        if (!myProcess) {
            //pull first process from queue
            pthread_mutex_lock(&(shared->readyQLock));
            myProcess = Qremove(&(shared->readyQ), findMin_burst(&(shared->readyQ))); //Choose next proc based on shortest burst time;
            if (myProcess) {
                printf("CPU %d - Scheduling PID %d\n", myID, myProcess->PID);
                processPrint(*myProcess);
            } else {
                printf("CPU %d - No process to schedule\n", myID);
            }
            pthread_mutex_unlock(&(shared->readyQLock));
        }
        if (myProcess) {
            //execute that process for one timestep
            (myProcess->burstRemaining)--;
            //if process is done processing, move to finish
            if (myProcess->burstRemaining <= 0) {
                pthread_mutex_lock(&(shared->finishedQLock));
                Qinsert(&(shared->finishedQ), myProcess);
                pthread_mutex_unlock(&(shared->finishedQLock));
                myProcess = NULL;
            }
        } //else do nothing if no process to run
        //send "downtick" to main once complete
        sem_post(&(shared->mainSem));
    }
}

//Return pos of node in list with lowest priority
// INPUT: queue* Q, pointer to queue object that will be traversed to find lowest priority
// OUTPUT: int min_pos, integer position of node with lowest priority (0 = head node)

int findMin_priority(queue* Q){
    int i = 0;
    int min_pos = 0;

    if(Q->head) {
        int min = Q->head->data->priority;
        node *next = Q->head;
        //iterate through list, keep track of lowest priority
        while (next != Q->tail && next != NULL) {
            if (next->data->priority < min) {
                min = next->data->priority;
                min_pos = i;
            }
            i++;
            next = next->next;
        }
    }
    return min_pos;
}

//  Schedule programs based on priority (lowest is most important), non preemptive
//  https://www.youtube.com/watch?v=i4PQucowf1c
void *NPPcpu(void *param) {
    int myID = ((cpuParams *) param)->threadNumber;
    sharedVars *shared = ((cpuParams *) param)->svars;
    process *myProcess = NULL;
    while (1) {
        //wait for "uptick" from main
        sem_wait(&(shared->cpuSems[myID]));
        //NPP only schedules when the last process has completed
        if (!myProcess) {
            //pull first process from queue
            pthread_mutex_lock(&(shared->readyQLock));
            myProcess = Qremove(&(shared->readyQ), findMin_priority(&(shared->readyQ))); //Choose next proc with lowest priority
            if (myProcess) {
                printf("CPU %d - Scheduling PID %d\n", myID, myProcess->PID);
                processPrint(*myProcess);
            } else {
                printf("CPU %d - No process to schedule\n", myID);
            }
            pthread_mutex_unlock(&(shared->readyQLock));
        }
        if (myProcess) {
            //execute that process for one timestep
            (myProcess->burstRemaining)--;
            //if process is done processing, move to finish
            if (myProcess->burstRemaining <= 0) {
                pthread_mutex_lock(&(shared->finishedQLock));
                Qinsert(&(shared->finishedQ), myProcess);
                pthread_mutex_unlock(&(shared->finishedQLock));
                myProcess = NULL;
            }
        } //else do nothing if no process to run
        //send "downtick" to main once complete
        sem_post(&(shared->mainSem));
    }
}

//  Round Robin scheduling, processes go FIFO and are given a limited amount of CPU time(quantum).
//  Documentation: https://www.youtube.com/watch?v=3N2t9_6Co3U
void *RRcpu(void *param) {
    int myID = ((cpuParams *) param)->threadNumber;
    sharedVars *shared = ((cpuParams *) param)->svars;
    process *myProcess = NULL;
    //
    int quantum = ((cpuParams*)param)->svars->quantum;
    int steps = 0;

    while(1){
        //wait for "uptick" from main
        sem_wait(&(shared->cpuSems[myID]));

        if(!myProcess){
            //pull first process from queue
            pthread_mutex_lock(&(shared->readyQLock));

            if((shared->readyQ).head) {

                myProcess = Qremove(&(shared->readyQ),0);
                printf("CPU %d - Scheduling PID %d\n", myID,myProcess->PID);
                processPrint(*myProcess);
            }
            else
                printf("CPU %d - No process to schedule\n", myID);

            pthread_mutex_unlock(&(shared->readyQLock));
        }

        if(myProcess){
            //execute that process for one timestep
            steps++;
            myProcess->burstRemaining = myProcess->burstRemaining -1;

            //if process is done processing, move to finish
            if(myProcess->burstRemaining ==0){
                pthread_mutex_lock(&(shared->finishedQLock));
                Qinsert(&(shared->finishedQ),myProcess);
                pthread_mutex_unlock(&(shared->finishedQLock));

                myProcess = NULL;
                steps = 0;
            }

//            If process not finished requeue
            if(steps == quantum){
                myProcess->requeued = true;
                pthread_mutex_lock(&(shared->readyQLock));
                Qinsert(&(shared->readyQ),myProcess);
                pthread_mutex_unlock(&(shared->readyQLock));

                myProcess = NULL;
                steps = 0;
            }

        }

        //send "downtick" to main once complete
        sem_post(&(shared->mainSem));
    }
}

//  Documentation: https://www.youtube.com/watch?v=h-e7QtjfmkI
void *SRTFcpu(void *param) {
    int myID = ((cpuParams *) param)->threadNumber;
    sharedVars *shared = ((cpuParams *) param)->svars;
    process *myProcess = NULL;


    while(1){

        sem_wait(&(shared->cpuSems[myID]));

        if(!myProcess){
            pthread_mutex_lock(&(shared->readyQLock));

            if((shared->readyQ).head) {

                myProcess = Qremove(&(shared->readyQ),findMin_burst(&(shared->readyQ)));
                printf("CPU %d - Scheduling PID %d\n", myID,myProcess->PID);
                processPrint(*myProcess);
            }
            else
                printf("CPU %d - No process to schedule\n", myID);

            pthread_mutex_unlock(&(shared->readyQLock));
        }

        if(myProcess){
            myProcess->burstRemaining = myProcess->burstRemaining-1;
            if(myProcess->burstRemaining==0) {
                pthread_mutex_lock(&(shared->finishedQLock));
                Qinsert(&(shared->finishedQ),myProcess);
                pthread_mutex_unlock(&(shared->finishedQLock));
                myProcess=NULL;
            }
        }
        pthread_mutex_lock(&(shared->readyQLock));

        if((shared->readyQ).head && myProcess){
            if((shared->readyQ).head->data->burstRemaining < myProcess->burstRemaining){
                myProcess->requeued = true;
                Qinsert(&(shared->readyQ),myProcess);
                myProcess = NULL;
            }

        }
        pthread_mutex_unlock(&(shared->readyQLock));
        //send "downtick" to main once complete
        sem_post(&(shared->mainSem));
    }
}

//  Documentation: https://www.youtube.com/watch?v=hDn4hM148V8
void *PPcpu(void *param) {
    int myID = ((cpuParams *) param)->threadNumber;
    sharedVars *shared = ((cpuParams *) param)->svars;
    process *myProcess = NULL;

    while(1){

        sem_wait(&(shared->cpuSems[myID]));

        if(!myProcess){

            pthread_mutex_lock(&(shared->readyQLock));

            if((shared->readyQ).head){

                myProcess = Qremove((&(shared->readyQ)),findMin_priority(&(shared->readyQ)));
                printf("CPU %d - Scheduling PID %d\n", myID,myProcess->PID);
                processPrint(*myProcess);
            } else
                printf("CPU %d - No process to schedule\n", myID);

            pthread_mutex_unlock(&(shared->readyQLock));

        }

        if(myProcess){

            myProcess->burstRemaining = myProcess->burstRemaining - 1;

            if(myProcess->burstRemaining==0){

                pthread_mutex_lock(&(shared->finishedQLock));

                Qinsert(&(shared->finishedQ),myProcess);
                pthread_mutex_unlock(&(shared->finishedQLock));

                myProcess = NULL;

            }
        }

        pthread_mutex_lock(&(shared->readyQLock));

        if((shared->readyQ).head && myProcess){

            if(((shared->readyQ).head->data->priority)  <   (myProcess->priority)){

                myProcess->requeued = true;
                Qinsert(&(shared->readyQ),myProcess);

                myProcess = NULL;

            }
        }
        pthread_mutex_unlock(&(shared->readyQLock));

        sem_post(&(shared->mainSem));

    }
}