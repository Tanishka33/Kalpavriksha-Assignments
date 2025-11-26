#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/LRUcache.h"

int main()
{
    LRUCache *cacheObject = NULL;
    char command[50];
    int key = 0;
    int capacity = 0;
    char value[200];
    char *returnedValue = NULL;
    int shouldContinue = 1;

    while (shouldContinue)
    {
        scanf("%s", command);

        if(strcmp(command, "createCache") == 0)
        {
            scanf("%d", &capacity);
            cacheObject = createCache(capacity);
        }
        else if(strcmp(command, "put") == 0)
        {
            scanf("%d %s", &key, value);
            if(cacheObject != NULL)
            {
                putValue(cacheObject, key, value);
            }
        }
        else if(strcmp(command, "get") == 0)
        {
            scanf("%d", &key);
            if(cacheObject != NULL)
            {
                returnedValue = getValue(cacheObject, key);
                printf("%s\n", returnedValue);
            }
        }
        else if(strcmp(command, "exit") == 0)
        {
            shouldContinue = 0;
        }
    }
    if(cacheObject != NULL)
    {
        freeCache(cacheObject);
    }
    return 0;
}