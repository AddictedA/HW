#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processQueue.h"
#include "CPUs.h"

void *genProcess(sharedVars *shared, int *time_to_next_process, int *nextPID, int *randSeed, int currTime);

void init(int num_processes, sharedVars *svars, int quantum);

void usage(char **argv);

int main(int argc, char *argv[]) {

    int simTime = 0;
    int numCPUs = 0;
    int CPUtype = 0;
    int quantum = 0;
    bool debugOutput = 0;

    char input[128];

    //get anything from the command line that we can
    if (argc > 1) {
        if (!strcmp(argv[1], "-h")) {
            usage(argv);
        }
        debugOutput = (bool) atoi(argv[1]);
        if (argc > 2) {
            simTime = atoi(argv[2]);
            if (argc > 3) {
                numCPUs = atoi(argv[3]);
                if (argc > 4) {
                    CPUtype = atoi(argv[4]);
                    if (argc > 5) {
                        quantum = atoi(argv[5]);
                        if (argc > 6) {
                            usage(argv);
                        }
                    }
                }
            }
        }
    }
    //get anything else we need from the user
    //get time
    while (simTime <= 0) {
        fprintf(stderr, "Enter simulation time: ");
        fgets(input, 128, stdin);
        simTime = atoi(input);
    }
    //get num CPUs
    while (numCPUs <= 0) {
        fprintf(stderr, "Enter number of CPUs: ");
        fgets(input, 128, stdin);
        numCPUs = atoi(input);
    }
    //get CPU type
    while (CPUtype < 1 || CPUtype > 6) {
        fprintf(stderr,
                "Select a CPU Type:\n\t1: First Come First Served\n\t2: Shortest Job First\n\t3: Priority (non-preemptive)\n\t4: Round Robbin\n\t5: Shortest Remaining Time First\n\t6: Priority (preemptive)\n");
        fgets(input, 128, stdin);
        CPUtype = atoi(input);
    }
    switch (CPUtype) {
        case 1:
            fprintf(stderr, "Using First Come First Served CPU\n");
            break;
        case 2:
            fprintf(stderr, "Using Shortest Job First CPU\n");
            break;
        case 3:
            fprintf(stderr, "Using Priority (non-preemptive) CPU\n");
            break;
        case 4:
            fprintf(stderr, "Using Round Robin CPU\n");
            break;
        case 5:
            fprintf(stderr, "Using Shortest Remaining Time First CPU\n");
            break;
        case 6:
            fprintf(stderr, "Using Priority (preemptive) CPU\n");
            break;
        default:
            break;
    }
    //get quantum, if necessary
    while (CPUtype == 4 && quantum <= 0) {//only if using RR
        fprintf(stderr, "Enter quantum length: ");
        fgets(input, 128, stdin);
        quantum = atoi(input);
    }

    //declare and initilize shared variables
    sharedVars vars;
    init(numCPUs, &vars, quantum);
    int processID = 0;
    int time_to_next_process = 0;//we want one as soon as we start
    int seed = 0;

    //create threads
    pthread_t *CPUs = malloc(sizeof(pthread_t) * numCPUs);
    cpuParams *params = malloc(sizeof(cpuParams) * numCPUs);
    int i = 0;
    for (i = 0; i < numCPUs; i++) {
        params[i].threadNumber = i;
        params[i].svars = &vars;
        switch (CPUtype) {
            case 1:
                pthread_create(&(CPUs[i]), NULL, &FCFScpu, &(params[i]));
                break;
            case 2:
                pthread_create(&(CPUs[i]), NULL, &SJFcpu, &(params[i]));
                break;
            case 3:
                pthread_create(&(CPUs[i]), NULL, &NPPcpu, &(params[i]));
                break;
            case 4:
                pthread_create(&(CPUs[i]), NULL, &RRcpu, &(params[i]));
                break;
            case 5:
                pthread_create(&(CPUs[i]), NULL, &SRTFcpu, &(params[i]));
                break;
            case 6:
                pthread_create(&(CPUs[i]), NULL, &PPcpu, &(params[i]));
                break;
        }
    }

    //run the simulation
    fprintf(stderr, "Beginning simulation...\n");
    int time;
    for (time = 0; time < simTime; time++) {

        // create a new process
        genProcess(&vars, &time_to_next_process, &processID, &seed, time);

        //print states of each queue
        if (debugOutput) {
            printf("\nstart of time %d\n", time);
            printf("ready:\n");
            Qprint(vars.readyQ);
            printf("\nfinished:\n");
            Qprint(vars.finishedQ);
            printf("\n");
        }

        //signal each CPU
        int i;
        for (i = 0; i < numCPUs; i++) {
            sem_post(&(vars.cpuSems[i]));
        }
        //wait for all CPUs
        for (i = 0; i < numCPUs; i++) {
            sem_wait(&(vars.mainSem));
        }
        //go slow so people can see what's going on
        //sleep(1);
        //increment wait times
        incrementWaitTimes(&(vars.readyQ));
    }
    fprintf(stderr, "Simulation finished!\n");
    //print state of queues
    printf("end of simulation\n");
    printf("ready:\n");
    Qprint(vars.readyQ);
    printf("\nfinished:\n");
    Qsort(&(vars.finishedQ));
    Qprint(vars.finishedQ);

    return 0;

}

void init(int num_processes, sharedVars *svars, int quantum) {
    initQueue(&(svars->readyQ));//given
    initQueue(&(svars->finishedQ));//given

    svars->cpuSems = malloc(sizeof(sem_t) * num_processes);
    int i;
    for (i = 0; i < num_processes; i++) {
        sem_init(&(svars->cpuSems[i]), 0, 0);
    }
    sem_init(&(svars->mainSem), 0, 0);
    pthread_mutex_init(&(svars->readyQLock), NULL);
    pthread_mutex_init(&(svars->finishedQLock), NULL);
    svars->quantum = quantum;
}

void usage(char **argv) {
    fprintf(stderr, "\nUsage: %s [debugOutput (0/1) [simulation time [number of CPUs [type of CPU [quantum]]]]]",
            argv[0]);
    fprintf(stderr,
            "\n\nWhere CPU types is a digit corresponidng to the following menu:\n\t1: First Come First Served\n\t2: Shortest Job First\n\t3: Priority (non-preemptive)\n\t4: Round Robbin\n\t5: Shortest Remaining Time First\n\t6: Priority (preemptive)\n");
    exit(0);
}

const int max_time_between = (5 + 2);
const int max_burst = 10;
const int max_priority = 6;

void *genProcess(sharedVars *shared, int *time_to_next_process, int *nextPID, int *randSeed, int currTime) {

    //decide to generate a process or not
    if ((*time_to_next_process) <= 0) {
        //create new process
        process *proc = malloc(sizeof(process));
        proc->PID = *nextPID;
        proc->arrivalTime = currTime;
        proc->priority = rand_r((u_int *) randSeed) % max_priority;
        proc->burstTotal = (rand_r((u_int *) randSeed) % max_burst) + 1;
        proc->burstRemaining = proc->burstTotal;
        proc->initialWait = 0;
        proc->totalWait = 0;
        proc->requeued = false;

        //insert into queue
        Qinsert(&(shared->readyQ), proc);

        //set up variables for next process
        (*nextPID)++;
        (*time_to_next_process) = rand_r((u_int *) randSeed) % max_time_between;
    }
    //decrement timer
    (*time_to_next_process)--;

    return 0;
}

