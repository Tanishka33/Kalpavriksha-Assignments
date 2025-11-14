#include "../inc/commandParser.h"

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
