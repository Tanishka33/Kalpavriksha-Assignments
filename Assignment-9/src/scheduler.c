#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void initializeQueue(ProcessQueue* processQueuePointer)
{
    if(processQueuePointer != NULL)
    {
        processQueuePointer->frontNodePointer = NULL;
        processQueuePointer->rearNodePointer = NULL;
    }
}

int isQueueEmpty(const ProcessQueue* processQueuePointer)
{
    int isEmpty = 1;
    if(processQueuePointer != NULL)
    {
        if(processQueuePointer->frontNodePointer != NULL)
        {
            isEmpty = 0;
        }
    }
    return isEmpty;
}

void enqueueProcess(ProcessQueue* processQueuePointer, ProcessControlBlock* processPointer)
{
    QueueNode *newNodePointer = NULL;
    if(processQueuePointer != NULL && processPointer != NULL)
    {
        newNodePointer = (QueueNode*)malloc(sizeof(QueueNode));
        if(newNodePointer != NULL)
        {
            newNodePointer->processPointer = processPointer;
            newNodePointer->nextNodePointer = NULL;

            if(processQueuePointer->rearNodePointer == NULL)
            {
                processQueuePointer->frontNodePointer = newNodePointer;
                processQueuePointer->rearNodePointer = newNodePointer;
            }
            else
            {
                processQueuePointer->rearNodePointer->nextNodePointer = newNodePointer;
                processQueuePointer->rearNodePointer = newNodePointer;
            }
        }
        else
        {
            printf("Failed to allocate memory for new queue node.\n");
        }
    }  
}

ProcessControlBlock *dequeueProcess(ProcessQueue* processQueuePointer)
{
    ProcessControlBlock *removedProcessPointer = NULL;
    if(processQueuePointer != NULL && processQueuePointer->frontNodePointer != NULL)
    {
        QueueNode *nodeToRemovePointer = processQueuePointer->frontNodePointer;
        removedProcessPointer = nodeToRemovePointer->processPointer;

        processQueuePointer->frontNodePointer = nodeToRemovePointer->nextNodePointer;
        if(processQueuePointer->frontNodePointer == NULL)
        {
            processQueuePointer->rearNodePointer = NULL;
        }
        free(nodeToRemovePointer);
        nodeToRemovePointer = NULL;
    }
    return removedProcessPointer;
}

void initializeHashMap(PcbHashMap *pcbHashMapPointer, size_t capacity)
{
    if(pcbHashMapPointer != NULL)
    {
        pcbHashMapPointer->capacity = capacity;
        pcbHashMapPointer->entryArrayPointer = (PcbHashEntry*)malloc(sizeof(PcbHashEntry) * capacity);

        if(pcbHashMapPointer->entryArrayPointer != NULL)
        {
            size_t index = 0;
            while (index < capacity)
            {
                pcbHashMapPointer->entryArrayPointer[index].isOccupied = 0;
                pcbHashMapPointer->entryArrayPointer[index].storedProcessId = -1;
                pcbHashMapPointer->entryArrayPointer[index].processPointer = NULL;
                index++;
            }  
        }
        else
        {
            printf("Failed to allocate memory for PCB Hash Map entries.\n");
        }        
    }
}

ProcessControlBlock *findProcessById(const PcbHashMap *pcbHashMapPointer,const int processId)
{
    ProcessControlBlock *foundProcessPointer = NULL;
    if(pcbHashMapPointer != NULL && pcbHashMapPointer->entryArrayPointer != NULL)
    {
        size_t index = (size_t)(processId % (int)pcbHashMapPointer->capacity);
        size_t searchStepCount = 0;
        while(searchStepCount < pcbHashMapPointer->capacity)
        {
            if(pcbHashMapPointer->entryArrayPointer[index].isOccupied == 1 && pcbHashMapPointer->entryArrayPointer[index].storedProcessId == processId)
            {
                foundProcessPointer = pcbHashMapPointer->entryArrayPointer[index].processPointer;
                break; 
            }
            index = (index + 1) % pcbHashMapPointer->capacity;
            searchStepCount++;
        }
    }
    return foundProcessPointer;
}

ProcessControlBlock *createProcess(PcbHashMap *pcbHashMapPointer, 
                                    const char* processName,                         
                                    int processId, 
                                    int totalCpuBurst, 
                                    int ioStartTime, 
                                    int ioDuration)
{
    ProcessControlBlock *newProcessPointer = NULL;
    if(pcbHashMapPointer != NULL && pcbHashMapPointer->entryArrayPointer != NULL)
    {
        newProcessPointer = (ProcessControlBlock*)malloc(sizeof(ProcessControlBlock));
        if(newProcessPointer != NULL)
        {
            size_t index = (size_t)(processId % (int)pcbHashMapPointer->capacity);
            
            newProcessPointer->processId = processId;
            newProcessPointer->processName = (char*)malloc(strlen(processName) + 1);
            if(newProcessPointer->processName != NULL)
            {
                strcpy(newProcessPointer->processName, processName);
            }

            newProcessPointer->totalCpuBurst = totalCpuBurst;
            newProcessPointer->ioStartTime = ioStartTime;
            newProcessPointer->ioDuration = ioDuration;

            newProcessPointer->remainingCpuTime = totalCpuBurst;
            newProcessPointer->remainingIoTime = ioDuration;

            newProcessPointer->arrivalTime = 0;
            newProcessPointer->completionTime = -1;

            newProcessPointer->totalIoTime = 0;
            newProcessPointer->totalWaitingTime = 0;

            newProcessPointer->currentState = PROCESS_NEW;
            newProcessPointer->isMovedToTerminated = 0;

            size_t searchStepCount = 0;
            while(searchStepCount < pcbHashMapPointer->capacity)
            {
                if(pcbHashMapPointer->entryArrayPointer[index].isOccupied == 0)
                {
                    pcbHashMapPointer->entryArrayPointer[index].isOccupied = 1;
                    pcbHashMapPointer->entryArrayPointer[index].storedProcessId = processId;
                    pcbHashMapPointer->entryArrayPointer[index].processPointer = newProcessPointer;
                    break; 
                }
                index = (index + 1) % pcbHashMapPointer->capacity;
                searchStepCount++;
            }
        }
        else
        {
            printf("Failed to allocate memory for new Process Control Block.\n");
        }
    }
    return newProcessPointer;
}

void destroyHashMap(PcbHashMap *pcbHashMapPointer)
{
    if(pcbHashMapPointer != NULL && pcbHashMapPointer->entryArrayPointer != NULL)
    {
        size_t index = 0;
        while(index < pcbHashMapPointer->capacity)
        {
            if(pcbHashMapPointer->entryArrayPointer[index].isOccupied == 1 && 
               pcbHashMapPointer->entryArrayPointer[index].processPointer != NULL)
            {
                    free(pcbHashMapPointer->entryArrayPointer[index].processPointer);
                    pcbHashMapPointer->entryArrayPointer[index].processPointer = NULL;
            }
            index++;
        }
        free(pcbHashMapPointer->entryArrayPointer);
        pcbHashMapPointer->entryArrayPointer = NULL;
    }
}

void simulateCpuTick(ProcessControlBlock *runningProcessPointer, int currentTime)
{
    if(runningProcessPointer != NULL)
    {
        runningProcessPointer->remainingCpuTime--;

        if(runningProcessPointer->remainingCpuTime == runningProcessPointer->totalCpuBurst - runningProcessPointer->ioStartTime &&
           runningProcessPointer->ioDuration > 0)
        {
            runningProcessPointer->currentState = PROCESS_WAITING;
        }
        else if(runningProcessPointer->remainingCpuTime <= 0 && runningProcessPointer->currentState != PROCESS_KILLED)
        {
            runningProcessPointer->currentState = PROCESS_TERMINATED;
            runningProcessPointer->completionTime = currentTime;
        }
        else
        {
            runningProcessPointer->currentState = PROCESS_RUNNING;
        }
    }
}

void handleIoQueue(ProcessQueue *waitingQueuePointer, ProcessQueue *readyQueuePointer)
{
    if(waitingQueuePointer != NULL && readyQueuePointer != NULL)
    {
        size_t size = 0;
        QueueNode *node = waitingQueuePointer->frontNodePointer;
        while(node != NULL)
        {
            size++;
            node = node->nextNodePointer;
        }
        while (size > 0)
        {
            ProcessControlBlock *processPointer = dequeueProcess(waitingQueuePointer);
            if(processPointer != NULL)
            {
                if(processPointer->currentState == PROCESS_KILLED)
                {
                    size--;
                    continue;
                }
                if(processPointer->remainingIoTime > 0)
                {
                    processPointer->remainingIoTime--;
                    processPointer->totalIoTime++;
                }
                if(processPointer->remainingIoTime == 0)
                {
                    processPointer->currentState = PROCESS_READY;
                    enqueueProcess(readyQueuePointer, processPointer);
                }
                else
                {
                    enqueueProcess(waitingQueuePointer, processPointer);
                }
            }
            size--;
        }   
    }
}

void moveProcessToTerminated(ProcessControlBlock *processPointer, ProcessQueue *terminatedQueuePointer)
{
    if(processPointer != NULL && terminatedQueuePointer != NULL)
    {
        if(processPointer->isMovedToTerminated == 0)
        {
            processPointer->currentState = PROCESS_TERMINATED;
            enqueueProcess(terminatedQueuePointer, processPointer);
            processPointer->isMovedToTerminated = 1;
        }
    }
}

void applyKillEvent(KillEvent *killEventsArrayPointer, size_t killEventCount, 
                    int currentTime, PcbHashMap *pcbHashMapPointer, 
                    ProcessQueue *terminatedQueuePointer)
{
    if(killEventsArrayPointer != NULL && pcbHashMapPointer != NULL && terminatedQueuePointer != NULL)
    {
        size_t eventIndex = 0;
        while(eventIndex < killEventCount)
        {
            if(killEventsArrayPointer[eventIndex].killTime == currentTime)
            {
                ProcessControlBlock *targetProcessPointer = findProcessById(pcbHashMapPointer, killEventsArrayPointer[eventIndex].processId);
                if(targetProcessPointer != NULL)
                {
                    targetProcessPointer->currentState = PROCESS_KILLED;
                    moveProcessToTerminated(targetProcessPointer, terminatedQueuePointer);
                }
            }
            eventIndex++;
        }
    }
}

void runScheduler(ProcessQueue *readyQueuePointer,
                  ProcessQueue *waitingQueuePointer,
                  ProcessQueue *terminatedQueuePointer,
                  PcbHashMap *pcbHashMapPointer,
                  KillEvent *killEventsArrayPointer,
                  size_t killEventCount)
{
    ProcessControlBlock *currentProcessPointer = NULL;
    int currentTime = 0;

    while (!(isQueueEmpty(readyQueuePointer) && isQueueEmpty(waitingQueuePointer) && currentProcessPointer == NULL))
    {
        applyKillEvent(killEventsArrayPointer, killEventCount, currentTime, pcbHashMapPointer, terminatedQueuePointer);
        if (currentProcessPointer == NULL && !isQueueEmpty(readyQueuePointer))
        {
            currentProcessPointer = dequeueProcess(readyQueuePointer);
            if (currentProcessPointer != NULL)
            {
                currentProcessPointer->currentState = PROCESS_RUNNING;
            }
        }
        simulateCpuTick(currentProcessPointer, currentTime);

        if (currentProcessPointer != NULL)
        {
            if(currentProcessPointer->currentState == PROCESS_WAITING)
            {
                enqueueProcess(waitingQueuePointer, currentProcessPointer);
                currentProcessPointer = NULL;
            }
            else if(currentProcessPointer->currentState == PROCESS_TERMINATED)
            {
                moveProcessToTerminated(currentProcessPointer, terminatedQueuePointer);
                currentProcessPointer = NULL;
            }
            else if(currentProcessPointer->currentState == PROCESS_KILLED)
            {
                currentProcessPointer->remainingCpuTime = 0;
                moveProcessToTerminated(currentProcessPointer, terminatedQueuePointer);
                currentProcessPointer = NULL;
            }
        }
        handleIoQueue(waitingQueuePointer, readyQueuePointer);
        currentTime++;
        #ifdef _WIN32
        Sleep(1000);
        #else
        sleep(1);
        #endif
    }
}

size_t readUserInput(PcbHashMap *pcbTablePointer, 
                     ProcessQueue *readyQueuePointer,
                     KillEvent *KillEventsPointer,
                     size_t maxKillEvents)
{
    size_t killEventCount = 0;
    int continueInput = 1;
    if(pcbTablePointer != NULL && readyQueuePointer != NULL && KillEventsPointer != NULL)
    {
        while(continueInput == 1)
        {
            char firstWord[32];
            printf("Enter process or kill command (or END) : ");
            scanf("%31s", firstWord);

            if(strcmp(firstWord, "END") == 0)
            {
                continueInput = 0;
            }
            else if(strcmp(firstWord, "KILL") == 0)
            {
                if(killEventCount < maxKillEvents)
                {
                    int processId = 0;
                    int killTime = 0;

                    scanf("%d %d", &processId, &killTime);

                    KillEventsPointer[killEventCount].processId = processId;
                    KillEventsPointer[killEventCount].killTime = killTime;
                    killEventCount++;
                }
            }
            else  
            {
                char processName[PROCESS_NAME_MAX_LENGTH];
                int processId = 0, burstTime = 0, ioStart = 0, ioDuration = 0;
                
                snprintf(processName, PROCESS_NAME_MAX_LENGTH, "%s", firstWord);
                scanf("%d %d %d %d", &processId, &burstTime, &ioStart, &ioDuration);

                ProcessControlBlock *newProcessPointer = createProcess(pcbTablePointer, 
                                                                      processName, 
                                                                      processId, 
                                                                      burstTime, 
                                                                      ioStart, 
                                                                      ioDuration);
                if(newProcessPointer != NULL)
                {
                    enqueueProcess(readyQueuePointer, newProcessPointer);
                }
            }
        }
    }
    return killEventCount;
}

void printResults(ProcessQueue *terminatedQueuePointer)
{
    if(terminatedQueuePointer != NULL)
    {
        printf("\n\n============Final Output============\n");
        printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\n");

        while(!isQueueEmpty(terminatedQueuePointer))
        {
            ProcessControlBlock *processPointer = dequeueProcess(terminatedQueuePointer);
            if(processPointer == NULL)
            {
                continue;
            }
            if(processPointer->currentState == PROCESS_KILLED)
            {
                printf("%d\t%s\t%d\t%d\t-\t\t-\n",
                       processPointer->processId,
                       processPointer->processName,
                       processPointer->totalCpuBurst,
                       processPointer->totalIoTime);

                continue;
            }
            int turnaroundTime = processPointer->completionTime - processPointer->arrivalTime;
            int waitingTime = turnaroundTime - processPointer->totalCpuBurst - processPointer->totalIoTime;

            if(waitingTime < 0)
            {
                waitingTime = 0;
            }
            printf("%d\t%s\t%d\t%d\t%d\t\t%d\n",
            processPointer->processId,
            processPointer->processName,
            processPointer->totalCpuBurst,
            processPointer->totalIoTime,
            turnaroundTime,
            waitingTime);
        }
    }
}