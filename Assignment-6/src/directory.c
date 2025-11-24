#include "directory.h"

char storage[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock* freeHead = NULL;
FreeBlock* freeTail = NULL;
FileNode* rootFolder = NULL;
FileNode* currentFolder = NULL;
int usedBlocks = 0;

void addFreeBlock(int number)
{
    FreeBlock* newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    newBlock->number = number;
    newBlock->next = NULL;
    newBlock->prev = NULL;

    if (freeHead == NULL)
    {
        freeHead = freeTail = newBlock;
    }
    else
    {
        freeTail->next = newBlock;
        newBlock->prev = freeTail;
        freeTail = newBlock;
    }
}

void setupFileSystem()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        addFreeBlock(i);
    }

    rootFolder = (FileNode*)malloc(sizeof(FileNode));
    strcpy(rootFolder->name, "/");
    rootFolder->isFolder = 1;
    rootFolder->fileSize = 0;
    rootFolder->numBlocks = 0;
    rootFolder->firstChild = NULL;
    rootFolder->next = rootFolder->prev = rootFolder;
    rootFolder->parent = NULL;

    currentFolder = rootFolder;

    printf("Virtual File Sysstm initialized. Root folder '/' created.\n");
}

void makeDirectory(char* name)
{
    FileNode* newFolder = (FileNode*)malloc(sizeof(FileNode));
    strcpy(newFolder->name, name);
    newFolder->isFolder = 1;
    newFolder->numBlocks = 0;
    newFolder->fileSize = 0;
    newFolder->firstChild = NULL;
    newFolder->parent = currentFolder;

    if (currentFolder->firstChild == NULL)
    {
        currentFolder->firstChild = newFolder;
        newFolder->next = newFolder;
        newFolder->prev = newFolder;
    }
    else
    {
        FileNode* first = currentFolder->firstChild;
        FileNode* last = first->prev;

        last->next = newFolder;
        newFolder->prev = last;
        newFolder->next = first;
        first->prev = newFolder;
    }
    printf("Directory '%s' created successfully.\n", name);
}


void listFiles()
{
    if (currentFolder->firstChild == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode* temp = currentFolder->firstChild;

    do
    {
        if (temp->isFolder)
            printf("%s/\n", temp->name);
        else
            printf("%s\n", temp->name);

        temp = temp->next;
    }
    while (temp != currentFolder->firstChild);
}


void changeDirectory(char* name)
{
    if (strcmp(name, "..") == 0)
    {
        if (currentFolder->parent != NULL)
        {
            currentFolder = currentFolder->parent;
            printf("Moved to ");
            showPath();
        }
        else
        {
            printf("Already at root directory.\n");
        }
        return;
    }

    if (currentFolder->firstChild == NULL)
    {
        printf("Directory not found.\n");
        return;
    }

    FileNode* temp = currentFolder->firstChild;
    do
    {
        if (temp->isFolder && strcmp(temp->name, name) == 0)
        {
            currentFolder = temp;
            printf("Moved to ");
            showPath();
            return;
        }
        temp = temp->next;
    }
    while (temp != currentFolder->firstChild);

    printf("Directory not found.\n");
}

void showPath()
{
    FileNode* temp = currentFolder;
    char fullPath[500] = "";

    while (temp != NULL)
    {
        char tempPath[500];

        if (strcmp(temp->name, "/") == 0)
        {
            if (strlen(fullPath) == 0)
                strcpy(fullPath, "/");
            else
            {
                sprintf(tempPath, "/%s", fullPath);
                strcpy(fullPath, tempPath);
            }
            break;
        }
        else
        {
            if (strlen(fullPath) == 0)
                sprintf(tempPath, "%s", temp->name);
            else
                sprintf(tempPath, "%s/%s", temp->name, fullPath);

            strcpy(fullPath, tempPath);
        }

        temp = temp->parent;
    }

    int len = strlen(fullPath);
    if (len > 1 && fullPath[len - 1] == '/')
        fullPath[len - 1] = '\0';

    printf("%s\n", fullPath);
}


void removeDirectory(char* name)
{
    int status = 1;
    FileNode* folderToRemove = findChild(name);
    if (folderToRemove == NULL)
    {
        printf("Directory not found.\n");
        status = 0;
    }

    else if (folderToRemove->isFolder == 0)
    {
        printf("'%s' is a file, not directory.\n", name);
        status = 0;
    }

    else if (folderToRemove->firstChild != NULL)
    {
        printf("Cannot rremove '%s': Directory is not empty.\n", name);
        status = 0;
    }
    if(!status)
    {
        return;
    }

    FileNode* parent = folderToRemove->parent;

    if (folderToRemove->next == folderToRemove && parent->firstChild == folderToRemove)
    {
        parent->firstChild = NULL;
    }
    else
    {
        folderToRemove->prev->next = folderToRemove->next;
        folderToRemove->next->prev = folderToRemove->prev;

        if (parent->firstChild == folderToRemove)
            parent->firstChild = folderToRemove->next;
    }

    free(folderToRemove);
    printf("Directory '%s' removed successfully.\n", name);
}

FileNode *findChild(char *name)
{
    FileNode *result = NULL;
    if (currentFolder->firstChild == NULL)
    {

        FileNode *temp = currentFolder->firstChild;

        do
        {
            if (strcmp(temp->name, name) == 0)
            {
                return temp;
                break;
            }
            temp = temp->next;
        } while (temp != currentFolder->firstChild);
    }
    return result;
}

int getFreeBlock()
{
    int blockNum = -1;
    if (freeHead == NULL)
    {
        FreeBlock *temp = freeHead;
        blockNum = temp->number;
        if (freeHead == freeTail)
        {
            freeHead = freeTail = NULL;
        }
        else
        {
            freeHead = freeHead->next;
            freeHead->prev = NULL;
        }
        free(temp);
        usedBlocks++;
    }
    return blockNum;
}

void returnBlock(int number)
{
    FreeBlock* newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    newBlock->number = number;
    newBlock->next = NULL;
    newBlock->prev = NULL;

    if (freeTail == NULL)
    {
        freeHead = freeTail = newBlock;
    }
    else
    {
        freeTail->next = newBlock;
        newBlock->prev = freeTail;
        freeTail = newBlock;
    }

    usedBlocks--;
}

void clearFileBlocks(FileNode* file)
{
    for (int index = 0; index < file->numBlocks; index++)
    {
        returnBlock(file->blockNumbers[index]);
        file->blockNumbers[index] = -1;
    }

    file->numBlocks = 0;
    file->fileSize = 0;
}

void freeFileNodes(FileNode* node)
{
    if (node == NULL)
        return;

    if (node->firstChild != NULL)
    {
        FileNode* child = node->firstChild;
        FileNode* start = child;

        do
        {
            FileNode* nextChild = child->next;
            freeFileNodes(child);
            child = nextChild;
        }
        while (child != start);
    }

    free(node);
}


void freeFreeBlockList()
{
    FreeBlock* temp = freeHead;

    while (temp != NULL)
    {
        FreeBlock* next = temp->next;
        free(temp);
        temp = next;
    }

    freeHead = NULL;
    freeTail = NULL;
    usedBlocks = 0;
}


void cleanupFileSystem()
{
    printf("memory released, exiting program... \n");

    freeFileNodes(rootFolder);
    freeFreeBlockList();

    rootFolder = NULL;
    currentFolder = NULL;

    printf("Exiting Virtual File System...\n");
}

void showDiskInfo()
{
    int totalBlocks = NUM_BLOCKS;
    int used = usedBlocks;
    int free = totalBlocks - used;
    double usagePercent = (used * 100.0) / totalBlocks;

    printf("Total Blocks: %d\n", totalBlocks);
    printf("Used Blocks: %d\n", used);
    printf("Free Blocks: %d\n", free);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

