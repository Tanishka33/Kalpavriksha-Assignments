#include "LRUcache.h"

static int computeHashIndex(const int key);

static Node* createNode(const int key, const char *data);

static void insertAtHead(LRUCache *cacheObject, Node *newNode);
static void removeNode(LRUCache *cacheObject, Node *nodeToRemove);
static void moveToHead(LRUCache *cacheObject, Node *nodeToMove);
static Node* removeTailNode(LRUCache *cacheObject);

LRUCache* createCache(const int capacity);
char* getValue(LRUCache *cacheObject, const int key);
void putValue(LRUCache *cacheObject, const int key, const char *value);
void freeCache(LRUCache *cacheObject);

static int computeHashIndex(const int key)
{
    int hashIndex = key % HASH_SIZE;
    if(hashIndex < 0)
    {
        hashIndex = -hashIndex;
    }
    return hashIndex;
}

LRUCache* createCache(const int capacity)
{
    LRUCache *cacheObject = NULL;
    int indexPosition = 0;
    cacheObject = (LRUCache*)malloc(sizeof(LRUCache));
    if(cacheObject != NULL)
    {
        cacheObject->capacity = capacity;
        cacheObject->currentSize = 0;
        cacheObject->head = NULL;
        cacheObject->tail = NULL; 
        for(indexPosition = 0; indexPosition < HASH_SIZE; indexPosition++)
        {
            cacheObject->hashMap[indexPosition] = NULL;
        }
    }
    return cacheObject;
}

static Node* createNode(const int key, const char *data)
{
    Node *newNode = NULL;
    int dataLength = 0;
    newNode = (Node*)malloc(sizeof(Node));
    if(newNode != NULL)
    {
        newNode->key = key;
        newNode->previousNode = NULL;
        newNode->nextNode = NULL;
        if(data != NULL)
        {
            dataLength = strlen(data);
            newNode->data = (char*)malloc(dataLength + 1);
            if(newNode->data != NULL)
            {
                strcpy(newNode->data, data);
            }
        }
    }
    return newNode;
}

static void insertAtHead(LRUCache *cacheObject, Node *newNode)
{
    int isCacheEmpty = 0;
    if(cacheObject != NULL && newNode != NULL)
    {
        isCacheEmpty = (cacheObject->head == NULL);
        if(isCacheEmpty)
        {
            cacheObject->head = newNode;
            cacheObject->tail = newNode;
        }
        else
        {
            newNode->nextNode = cacheObject->head;
            cacheObject->head->previousNode = newNode;
            cacheObject->head = newNode;
        }
    }
}

static void removeNode(LRUCache *cacheObject, Node *nodeToRemove)
{
    int isHeadNode = 0;
    int isTailNode = 0;
    if(cacheObject != NULL && nodeToRemove != NULL)
    {
        isHeadNode = (nodeToRemove == cacheObject->head);
        isTailNode = (nodeToRemove == cacheObject->tail);
        if(isHeadNode)
        {
            cacheObject->head = nodeToRemove->nextNode;
            if(cacheObject->head != NULL)
            {
                cacheObject->head->previousNode = NULL;
            }
        }
        else if(isTailNode)
        {
            cacheObject->tail = nodeToRemove->previousNode;
            if(cacheObject->tail != NULL)
            {
                cacheObject->tail->nextNode = NULL;
            }
        }
        else 
        {
            if(nodeToRemove->previousNode != NULL)
            {
                nodeToRemove->previousNode->nextNode = nodeToRemove->nextNode;
            }
            if(nodeToRemove->nextNode != NULL)
            {
                nodeToRemove->nextNode->previousNode = nodeToRemove->previousNode;
            }
        }
        nodeToRemove->previousNode = NULL;
        nodeToRemove->nextNode = NULL;
    }
}

static void moveToHead(LRUCache *cacheObject, Node *nodeToMove)
{
    int isAlreadyHead = 0;
    if (cacheObject != NULL && nodeToMove != NULL)
    {
        isAlreadyHead = (cacheObject->head == nodeToMove);

        if (!isAlreadyHead)
        {
            removeNode(cacheObject, nodeToMove);
            insertAtHead(cacheObject, nodeToMove);
        }
    }
}

static Node* removeTailNode(LRUCache *cacheObject)
{
    Node *removeNode = NULL;
    int isSingleNode = 0;
    if(cacheObject != NULL && cacheObject->tail != NULL)
    {
        removeNode = cacheObject->tail;
        isSingleNode = (cacheObject->head == cacheObject->tail);
        if(isSingleNode)
        {
            cacheObject->head = NULL;
            cacheObject->tail = NULL;
        }
        else
        {
            cacheObject->tail = removeNode->previousNode;
            cacheObject->tail->nextNode = NULL;
        }
        removeNode->previousNode = NULL;
        removeNode->nextNode = NULL;
    }
    return removeNode;
}

void putValue(LRUCache *cacheObject, const int key, const char *value)
{
    int hashIndex = 0;
    Node *existingNode = NULL;
    Node *newNode = NULL;
    Node *removeNode = NULL;
    int isUpdatedOperation = 0;
    int isCacheFull = 0;

    if(cacheObject != NULL && value != NULL)
    {
        hashIndex = computeHashIndex(key);
        existingNode = cacheObject->hashMap[hashIndex];
        isUpdatedOperation = (existingNode != NULL);
        if(isUpdatedOperation)
        {
            free(existingNode->data);
            existingNode->data = (char*)malloc(strlen(value) + 1);
            if(existingNode->data != NULL)
            {
                strcpy(existingNode->data, value);
            }
            moveToHead(cacheObject, existingNode);
        }
        else 
        {
            newNode = createNode(key, value);
            isCacheFull = (cacheObject->currentSize == cacheObject->capacity);
            if(isCacheFull)
            {
                removeNode = removeTailNode(cacheObject);
                if(removeNode != NULL)
                {
                    int removedIndex = computeHashIndex(removeNode->key);
                    cacheObject->hashMap[removedIndex] = NULL;
                    free(removeNode->data);
                    free(removeNode);
                    cacheObject->currentSize--;
                }
            }
            insertAtHead(cacheObject, newNode);
            hashIndex = computeHashIndex(key);
            cacheObject->hashMap[hashIndex] = newNode;
            cacheObject->currentSize++;
        }
    }
}

char* getValue(LRUCache *cacheObject, const int key)
{
    int hashIndex = 0;
    Node *foundNode = NULL;
    char *resultValue = NULL;
    if(cacheObject != NULL)
    {
        hashIndex = computeHashIndex(key);
        foundNode = cacheObject->hashMap[hashIndex];
        if(foundNode != NULL)
        {
            moveToHead(cacheObject, foundNode);
            resultValue = foundNode->data;
        }
        else
        {
            resultValue = "NULL";
        }
    }
    return resultValue;
}

void freeCache(LRUCache *cacheObject)
{
    Node *currentNode = NULL;
    Node *nextNode = NULL;
    if(cacheObject != NULL)
    {
        currentNode = cacheObject->head;
        while (currentNode != NULL)
        {
            nextNode = currentNode->nextNode;
            if(currentNode->data != NULL)
            {
                free(currentNode->data);
            }
            free(currentNode);
            currentNode = nextNode;
        }
        free(cacheObject);
    }
}