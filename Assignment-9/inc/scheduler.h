#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>

#define PROCESS_NAME_MAX_LENGTH 32
#define HASH_MAP_DEFAULT_CAPACITY 64

typedef enum
{
    PROCESS_NEW,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_WAITING,
    PROCESS_TERMINATED,
    PROCESS_KILLED
} ProcessState;

typedef struct processControlBlock
{
    int processId;
    char processName[PROCESS_NAME_MAX_LENGTH];

    int totalCpuBurst;
    int ioStartTime;
    int ioDuration;

    int remainingCpuTime;
    int remainingIoTime;

    int arrivalTime;
    int completionTime;

    int totalIoTime;
    int totalWaitingTime;

    ProcessState currentState;

    int isMovedToTerminated;
} ProcessControlBlock;

typedef struct QueueNode
{
    ProcessControlBlock* processPointer;
    struct QueueNode* nextNodePointer;
} QueueNode;

typedef struct ProcessQueue
{
    QueueNode* frontNodePointer;
    QueueNode* rearNodePointer;
} ProcessQueue;

typedef struct PcbHashEntry
{
    int isOccupied;
    int storedProcessId;
    ProcessControlBlock* processPointer;
} PcbHashEntry;

typedef struct PcbHashMap
{
    size_t capacity;
    PcbHashEntry *entryArrayPointer;
} PcbHashMap;

typedef struct KillEvent
{
    int processId;
    int killTime;
} KillEvent;

void initializeHashMap(PcbHashMap* pcbHashMapPointer, size_t capacity);
ProcessControlBlock *createProcess(PcbHashMap *pcbHashMapPointer, 
                                    const char*processName,                         
                                    int processId, 
                                    int totalCpuBurst, 
                                    int ioStartTime, 
                                    int ioDuration);
ProcessControlBlock *findProcessById(const PcbHashMap *pcbHashMapPointer, int processId);
void destroyHashMap(PcbHashMap* pcbHashMapPointer);

void initializeQueue(ProcessQueue* processQueuePointer);
void enqueueProcess(ProcessQueue* processQueuePointer, ProcessControlBlock* processPointer);
ProcessControlBlock* dequeueProcess(ProcessQueue* processQueuePointer);
int isQueueEmpty(const ProcessQueue* processQueuePointer);
void printResults(ProcessQueue *terminatedQueuePointer);
void runScheduler(ProcessQueue *readyQueuePointer,
                  ProcessQueue *waitingQueuePointer,
                  ProcessQueue *terminatedQueuePointer,
                  PcbHashMap *pcbHashMapPointer,
                  KillEvent *killEventsArrayPointer,
                  size_t killEventCount);
size_t readUserInput(PcbHashMap *pcbTablePointer, 
                     ProcessQueue *readyQueuePointer,
                     KillEvent *KillEventsPointer,
                     size_t maxKillEvents);

#endif
