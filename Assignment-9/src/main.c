#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

int main()
{
    int processCount = 0;
    int killCount = 0;

    PcbHashMap pcbTable;
    ProcessQueue readyQueue;
    ProcessQueue waitingQueue;      
    ProcessQueue terminatedQueue;

    initializeHashMap(&pcbTable, HASH_MAP_DEFAULT_CAPACITY);
    initializeQueue(&readyQueue);
    initializeQueue(&waitingQueue);
    initializeQueue(&terminatedQueue);

    printf("Enter total number of processes : ");
    scanf("%d", &processCount);

    for(int index = 0; index < processCount; index++)
    {
        char processName[PROCESS_NAME_MAX_LENGTH];
        int pid, burstTime, ioStartTime, ioDuration;

        printf("Enter details for Process %d\n", index + 1);

        printf("Process Name: ");
        scanf("%s", processName);
        printf("Process ID: ");
        scanf("%d", &pid);
        printf("CPU bust time: ");
        scanf("%d", &burstTime);
        printf("I/O start time: ");
        scanf("%d", &ioStartTime);
        printf("I/O duration: ");
        scanf("%d", &ioDuration);

        ProcessControlBlock *pcbPointer = createProcess(&pcbTable, processName, pid, burstTime, ioStartTime, ioDuration);

        enqueueProcess(&readyQueue, pcbPointer);
    }
    printf("Enter total number of kill events : ");
    scanf("%d", &killCount);

    KillEvent *killEventsPointer = malloc(sizeof(KillEvent) * killCount);

    for(int index = 0; index < killCount; index++)
    {
        printf("\nKill Event %d\n", index + 1);
        printf("PID to kill: ");
        scanf("%d", &killEventsPointer[index].processId);

        printf("Kill time: ");
        scanf("%d", &killEventsPointer[index].killTime);
    }
    printf("\nRunning FCFS Scheduler..\n");
    runScheduler(&readyQueue, &waitingQueue, &terminatedQueue, &pcbTable, killEventsPointer, killCount);
    printResults(&terminatedQueue);

    destroyHashMap(&pcbTable);
    free(killEventsPointer);

    printf("simulation completed.\n");
    return 0;

}
