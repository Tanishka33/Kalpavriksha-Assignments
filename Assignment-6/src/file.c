#include "../inc/file.h"

void createFile(char* name){
    if(currentFolder->firstChild != NULL)
    {
        FileNode* temp = currentFolder->firstChild;
        do
        {
            if(strcmp(temp->name, name) == 0)
            {
                printf("Name already exists in current directory.\n");
                return;
            }
            temp = temp->next;
        } 
        while (temp != currentFolder->firstChild);
    }

    FileNode* newFile = (FileNode*)malloc(sizeof(FileNode));
    strcpy(newFile->name, name);
    newFile->isFolder = 0;
    newFile->fileSize = 0;
    newFile->numBlocks = 0;
    newFile->parent = currentFolder;
    newFile->firstChild = NULL;

    if(currentFolder->firstChild == NULL)
    {
        currentFolder->firstChild = newFile;
        newFile->next = newFile;
        newFile->prev = newFile;
    }
    else
    {
        FileNode* first = currentFolder->firstChild;
        FileNode* last = first->prev;

        last->next = newFile;
        newFile->prev = last;
        newFile->next = first;
        first->prev = newFile;
    }
    printf("File '%s' created successfully.\n", name);
}

void writeFile(char* name, char* data)
{
    FileNode* file = findChild(name);
    if(file == NULL)
    {
        printf("File not found.\n");
        return;
    }
    if(file->isFolder == 1)
    {
        printf("'%s' is a directory not a file.\n", name);
        return;
    }

    int dataSize = strlen(data);
    int neededBlocks = (dataSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if(neededBlocks>10)
    {
        printf("File too large. Max 10 blocks allowed.\n");
        return;
    }

    int freeBlocks = NUM_BLOCKS - usedBlocks;
    if(freeBlocks < neededBlocks)
    {
        printf("Not enough space on disk\n");
        return;
    }
    if(file->numBlocks > 0)
    {
        clearFileBlocks(file);
    }
    int bytesAvailable = dataSize;
    int writtenBlocks = 0;
    char* ptr = data;

    for(int index = 0; index < neededBlocks; index++)
    {
        int blockNumber = getFreeBlock();
        if(blockNumber == -1)
        {
            printf("Error: no more free blocks!\n");
            break;
        }
        int toWrite = (bytesAvailable > BLOCK_SIZE) ? BLOCK_SIZE : bytesAvailable;
        memcpy(storage[blockNumber], ptr, toWrite);
        file->blockNumbers[index] = blockNumber;

        ptr += toWrite;
        bytesAvailable -= toWrite;
        writtenBlocks++;
    }
    file->numBlocks = writtenBlocks;
    file->fileSize = dataSize;

    printf("Data written successfully (size=%d bytes).\n", dataSize);
}

void readFile(char* name)
{
    FileNode* file = findChild(name);
    if(file == NULL)
    {
        printf("File not found.\n");
        return;
    }
    if(file->isFolder == 1)
    {
        printf("'%s' is a directory, not a file.\n", name);
        return;
    }
    if(file->numBlocks == 0)
    {
        printf("File is empty.\n");
        return;
    }
    printf("File Content:\n");

    char blockData[BLOCK_SIZE + 1];
    int bytesPrinted = 0;
    
    for(int blockIndex = 0; blockIndex < file->numBlocks; blockIndex++)
    {
        int blockNum = file->blockNumbers[blockIndex];
        memcpy(blockData, storage[blockNum], BLOCK_SIZE);

        blockData[BLOCK_SIZE] = '\0';

        int bytesToPrint  = file->fileSize - bytesPrinted;
        if(bytesToPrint > BLOCK_SIZE)
        {
            bytesToPrint = BLOCK_SIZE;
        }
        for(int charIndex = 0; charIndex < bytesToPrint; charIndex++)
        {
            printf("%c", blockData[charIndex]);
        }
        bytesPrinted += bytesToPrint;
    }
    printf("\n");
}

void deleteFile(char* name)
{
    FileNode* fileToDelete = findChild(name);
    if(fileToDelete == NULL)
    {
        printf("File not found.\n");
        return;
    }
    if(fileToDelete->isFolder == 1)
    {
        printf("'%s' is a directory. Use rmdir instead.\n", name);
        return;
    }
    for(int blockIndex = 0; blockIndex < fileToDelete->numBlocks; blockIndex++)
    {
        int blockNum = fileToDelete->blockNumbers[blockIndex];
        returnBlock(blockNum);
        fileToDelete->blockNumbers[blockIndex] = -1;
    }
    fileToDelete->numBlocks = 0;
    fileToDelete->fileSize = 0;

    FileNode* parent = fileToDelete->parent;
    if(fileToDelete->next == fileToDelete && parent->firstChild == fileToDelete)
    {
        parent->firstChild = NULL;
    }
    else
    {
        fileToDelete->prev->next = fileToDelete->next;
        fileToDelete->next->prev = fileToDelete->prev;  
        
        if (parent->firstChild == fileToDelete)
            parent->firstChild = fileToDelete->next;
    }
    free(fileToDelete);
    printf("File '%s' deleted successfully.\n", name);  
}
