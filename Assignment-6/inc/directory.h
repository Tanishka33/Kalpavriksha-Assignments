#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

typedef struct FileNode
{
    char name[51];
    int isFolder;
    int fileSize;
    int numBlocks;
    int blockNumbers[10];

    struct FileNode* next;
    struct FileNode* prev;
    struct FileNode* parent;
    struct FileNode* firstChild;
} FileNode;

typedef struct FreeBlock
{
    int number;
    struct FreeBlock* next;
    struct FreeBlock* prev;
} FreeBlock;

extern char storage[NUM_BLOCKS][BLOCK_SIZE];
extern FreeBlock* freeHead;
extern FreeBlock* freeTail;
extern FileNode* rootFolder;
extern FileNode* currentFolder;
extern int usedBlocks;

void setupFileSystem();
void makeDirectory(const char* name);
void listFiles();
void changeDirectory(const char* name);
void showPath();
void removeDirectory(const char* name);
void showDiskInfo();

FileNode* findChild(const char* name);
int getFreeBlock();
void returnBlock(int number);
void clearFileBlocks(FileNode* file);

void cleanupFileSystem();

#endif
