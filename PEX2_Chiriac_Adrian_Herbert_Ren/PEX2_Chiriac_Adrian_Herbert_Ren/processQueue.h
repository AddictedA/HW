#include <stdbool.h>

#ifndef processQueueDotHStructs
#define processQueueDotHStructs
// This struct tracks various attributes for a process
typedef struct {
    int PID;    // process ID
    int arrivalTime;    // timestep this process arrived in the queue
    int priority;    // lower priority == better
    int burstTotal;    // total # of timesteps this process requires to run
    int burstRemaining;    // # of timesteps remaining for this process
    int initialWait;    // # timesteps it took to be scheduled for first time
    int totalWait;        // total # timesteps until process completed
    bool requeued;    // Has this process been requeued (e.g., for Round Robin)
} process;

// A single node in our linked list of processes
typedef struct s_node {
    process *data;    // pointer to a process struct (see above)
    struct s_node *prev;    // pointer to previous node in list
    struct s_node *next;    // pointer to next node in list
} node;

// queue contains pointers to both the head and tail of a linked list of node*
typedef struct {
    node *head;
    node *tail;
} queue;

#endif

// private functions in main (no concern to you)
void initQueue(queue *Q);

void incrementWaitTimes(queue *Q);

void Qsort(queue *Q);

// functions that may be helpful to you for debugging purposes
void Qprint(queue Q);

void processPrint(process proc);

// functions you will definitely be using
void Qinsert(queue *Q, process *proc);

process *Qremove(queue *Q, int which);


