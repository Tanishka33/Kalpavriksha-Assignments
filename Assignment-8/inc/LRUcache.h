#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10000

typedef struct Node
{
    int key;
    char *data;
    struct Node *previousNode;
    struct Node *nextNode;
} Node;

typedef struct LRUCache 
{
    int capacity;
    int currentSize;
    Node *head;
    Node *tail;
    Node *hashMap[HASH_SIZE];
} LRUCache;

LRUCache* createCache(int capacity);
char* getValue(LRUCache *cacheObject, int key);
void putValue(LRUCache *cacheObject, int key, const char *value);
void freeCache(LRUCache *cacheObject);

#endif