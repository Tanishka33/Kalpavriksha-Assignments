#include "file.h"

void createFile(const char *name)
{
    if (currentFolder->firstChild != NULL)
    {
        FileNode *temp = currentFolder->firstChild;
        do
        {
            if (strcmp(temp->name, name) == 0)
            {
                printf("Name already exists in current directory.\n");
            }
            temp = temp->next;
        } while (temp != currentFolder->firstChild);
    }
    FileNode *newFile = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newFile->name, name);
    newFile->isFolder = 0;
    newFile->fileSize = 0;
    newFile->numBlocks = 0;
    newFile->parent = currentFolder;
    newFile->firstChild = NULL;

    if (currentFolder->firstChild == NULL)
    {
        currentFolder->firstChild = newFile;
        newFile->next = newFile;
        newFile->prev = newFile;
    }
    else
    {
        FileNode *first = currentFolder->firstChild;
        FileNode *last = first->prev;
        last->next = newFile;
        newFile->prev = last;
        newFile->next = first;
        first->prev = newFile;
    }
    printf("File '%s' created successfully.\n", name);
}

void writeFile(const char *name, const char *data)
{
    int canWrite = 1;
    FileNode *file = findChild(name);
    if (file == NULL)
    {
        printf("File not found.\n");
        canWrite = 0;
    }
    else if (file->isFolder == 1)
    {
        printf("'%s' is a directory not a file.\n", name);
        canWrite = 0;
    }
    else
    {
        int dataSize = strlen(data);
        int neededBlocks = (dataSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

        if (neededBlocks > 10)
        {
            printf("File too large. Max 10 blocks allowed.\n");
            canWrite = 0;
        }
        else
        {
            int freeBlocks = NUM_BLOCKS - usedBlocks;
            if (freeBlocks < neededBlocks)
            {
                printf("Not enough space on disk\n");
                canWrite = 0;
            }
        }
        if (canWrite && file->numBlocks > 0)
        {
            clearFileBlocks(file);
        }
        if (canWrite)
        {
            int bytesAvailable = dataSize;
            int writtenBlocks = 0;
            char *ptr = data;

            for (int index = 0; index < neededBlocks; index++)
            {
                int blockNumber = getFreeBlock();
                if (blockNumber == -1)
                {
                    printf("Error: no more free blocks!\n");
                    canWrite = 0;
                    break;
                }
                int toWrite = (bytesAvailable > BLOCK_SIZE) ? BLOCK_SIZE : bytesAvailable;
                memcpy(storage[blockNumber], ptr, toWrite);
                file->blockNumbers[index] = blockNumber;

                ptr += toWrite;
                bytesAvailable -= toWrite;
                writtenBlocks++;
            }
            if (canWrite)
            {
                file->numBlocks = writtenBlocks;
                file->fileSize = dataSize;
                printf("Data written successfully (size=%d bytes).\n", dataSize);
            }
        }
    }
}

void readFile(const char *name)
{
    int canRead = 1;
    FileNode *file = findChild(name);
    if (file == NULL)
    {
        printf("File not found.\n");
        canRead = 0;
    }
    else if (file->isFolder == 1)
    {
        printf("'%s' is a directory, not a file.\n", name);
        canRead = 0;
    }
    else if (file->numBlocks == 0)
    {
        printf("File is empty.\n");
        canRead = 0;
    }
    if (canRead)
    {
        printf("File Content:\n");
        char blockData[BLOCK_SIZE + 1];
        int bytesPrinted = 0;
        for (int blockIndex = 0; blockIndex < file->numBlocks; blockIndex++)
        {
            int blockNum = file->blockNumbers[blockIndex];
            memcpy(blockData, storage[blockNum], BLOCK_SIZE);
            blockData[BLOCK_SIZE] = '\0';

            int bytesToPrint = file->fileSize - bytesPrinted;
            if (bytesToPrint > BLOCK_SIZE)
            {
                bytesToPrint = BLOCK_SIZE;
            }
            for (int charIndex = 0; charIndex < bytesToPrint; charIndex++)
            {
                printf("%c", blockData[charIndex]);
            }
            bytesPrinted += bytesToPrint;
        }
        printf("\n");
    }
}

void deleteFile(const char *name)
{
    int canDelete = 1;
    FileNode *fileToDelete = findChild(name);
    if (fileToDelete == NULL)
    {
        printf("File not found.\n");
        canDelete = 0;
    }
    else if (fileToDelete->isFolder == 1)
    {
        printf("'%s' is a directory. Use rmdir instead.\n", name);
        canDelete = 0;
    }
    if (canDelete)
    {
        for (int blockIndex = 0; blockIndex < fileToDelete->numBlocks; blockIndex++)
        {
            int blockNum = fileToDelete->blockNumbers[blockIndex];
            returnBlock(blockNum);
            fileToDelete->blockNumbers[blockIndex] = -1;
        }
        fileToDelete->numBlocks = 0;
        fileToDelete->fileSize = 0;

        FileNode *parent = fileToDelete->parent;
        if (fileToDelete->next == fileToDelete && parent->firstChild == fileToDelete)
        {
            parent->firstChild = NULL;
        }
        else
        {
            fileToDelete->prev->next = fileToDelete->next;
            fileToDelete->next->prev = fileToDelete->prev;

            if (parent->firstChild == fileToDelete)
            {
                parent->firstChild = fileToDelete->next;
            }
        }
        free(fileToDelete);
        printf("File '%s' deleted successfully.\n", name);
    }
}
