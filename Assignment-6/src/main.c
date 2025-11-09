#include "../inc/virtualFileSystem.h"

void handleDirectoryCommands(char* command)
    {
        char name[50];

        if(strcmp(command, "mkdir") == 0)
        {
            scanf("%s", name);
            makeDirectory(name);
        }
        else if(strcmp(command,"ls") == 0)
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

int main()
{
    setupFileSystem();

    char command[50];
    int running = 1;
    while (running)
    {
        printf("/ > ");
        scanf("%s", command);

        if(
            strcmp(command, "mkdir") == 0 || 
            strcmp(command, "ls") == 0 || 
            strcmp(command, "cd") == 0 || 
            strcmp(command, "pwd") == 0 || 
            strcmp(command, "rmdir") == 0
        )
        {
            handleDirectoryCommands(command);
        }
        else if(
            strcmp(command, "create") == 0 ||
            strcmp(command, "write") == 0 ||
            strcmp(command, "read") == 0 ||
            strcmp(command, "delete") == 0
        )
        {
            handleFileCommands(command);
        }
        else
        {
            running = handleUtilityCommands(command);
        }
    }
    return 0;
}

