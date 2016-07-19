#include <pthread.h>
#include <semaphore.h>
#include "processQueue.h"

#ifndef CPUsDotHStructs
#define CPUsDotHStructs
typedef struct {
    pthread_mutex_t readyQLock;    // use a mutex to ensure singular access
    pthread_mutex_t finishedQLock;    // use a mutex to ensure singular access
    sem_t *cpuSems;    // array of CPU semaphores, indexed using CPU/thread ID
    sem_t mainSem;    // single semaphore to signal main we're ready for next timestep
    queue readyQ;    // head/tail of linked list of processes in Ready Queue
    queue finishedQ;    // head/tail of linked list of processes in Finished Queue
    int quantum;    // Used for Round Robin, etc.
} sharedVars;        // All scheduling threads will share this data struct!

typedef struct {
    int threadNumber;    // which CPU are we (used for our cpuSems array, e.g.)
    sharedVars *svars;    // reference to the shared data struct
} cpuParams;    // this gets passed in to your scheduling thread upon creation
#endif

// You must implement the following in CPUs.c
void *FCFScpu(void *param);

void *SJFcpu(void *param);

void *NPPcpu(void *param);

void *RRcpu(void *param);

void *SRTFcpu(void *param);

void *PPcpu(void *param);
