#include "commandParser.h"

void handleDirectoryCommands(char* command)
{
    char name[50];

    if(strcmp(command, "mkdir") == 0)
    {
        scanf("%s", name);
        makeDirectory(name);
    }
    else if(strcmp(command, "ls") == 0)
    {
        listFiles();
    }
    else if(strcmp(command, "cd") == 0)
    {
        scanf("%s", name);
        changeDirectory(name);
    }
    else if(strcmp(command, "pwd") == 0)
    {
        showPath();
    }
    else if(strcmp(command, "rmdir") == 0)
    {
        scanf("%s", name);
        removeDirectory(name);
    }
}

void handleFileCommands(char* command)
{
    char name[50];
    char data[512];

    if(strcmp(command, "create") == 0)
    {
        scanf("%s", name);
        createFile(name);
    }
    else if(strcmp(command, "write") == 0)
    {
        scanf("%s", name);
        getchar();  
        fgets(data, sizeof(data), stdin);
        data[strcspn(data, "\n")] = '\0';
        writeFile(name, data);
    }
    else if(strcmp(command, "read") == 0)
    {
        scanf("%s", name);
        readFile(name);
    }
    else if(strcmp(command, "delete") == 0)
    {
        scanf("%s", name);
        deleteFile(name);
    }
}

int handleUtilityCommands(char* command)
{
    if(strcmp(command, "df") == 0)
    {
        showDiskInfo();
    }
    else if(strcmp(command, "exit") == 0)
    {
        cleanupFileSystem();
        return 0;
    }
    else
    {
        printf("Unknown command: %s\n", command);
    }
    return 1;
}
