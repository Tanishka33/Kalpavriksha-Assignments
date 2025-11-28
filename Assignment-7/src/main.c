#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "Players_data.h"

typedef enum 
{
    ADD_PLAYER = 1,
    DISPLAY_TEAM_PLAYERS,
    DISPLAY_TEAMS_AVG_SR,
    DISPLAY_TOP_K,
    DISPLAY_ROLE_ACROSS_TEAMS,
    EXIT
} MenuOption;

void printMenu(void)
{
        printf("\n==============================================================================\n");
        printf(" ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf(" 1. Add Player to Team\n");
        printf(" 2. Display Players of a Specific Team\n");
        printf(" 3. Display Teams by Average Batting Strike Rate\n");
        printf(" 4. Display Top K Players of a Specific Team by Role\n");
        printf(" 5. Display all Players of specific role Across All Teams by performance index\n");
        printf(" 6. Exit\n");
        printf("==============================================================================\n");
        printf(" Enter your choice: ");   
}

int main(void)
{
    Team *teams = NULL;
    int userChoice = 0;
    int status = 0;
    int exitCode = 0;
    bool runProgram = true;

    teams = malloc(sizeof(Team) * teamCount);
    if(!teams)
    {
        printf("Memory allocation failed for teams.\n");
        return 1;
    }

    status = initializeTeams(teams);
    if (status != 0)
    {
        printf("Error: Could not initialize teams.\n");
        exitCode = 1;
        runProgram = false;
    }

    if (runProgram)
    {
        status = loadInitialPlayers(teams);
        if (status != 0)
        {
            printf("Error: Could not load initial players.\n");
            exitCode = 1;
            runProgram = false;
        }
    }

    while (runProgram && userChoice != EXIT)
    {
        printMenu();

        if (scanf("%d", &userChoice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            userChoice = 0;
            continue;
        }

        switch (userChoice)
        {
            case ADD_PLAYER:
                if (addPlayer(teams) != 0)
                {
                    printf("Error: Could not add player.\n");
                }
                break;

            case DISPLAY_TEAM_PLAYERS:
                if (displayTeamPlayers(teams) != 0)
                {
                    printf("Error in displaying team players.\n");
                }
                break;

            case DISPLAY_TEAMS_AVG_SR:
                if (displayTeamAvgScore(teams) != 0)
                {
                    printf("Error in displaying avg strike rates.\n");
                }
                break;

            case DISPLAY_TOP_K:
                if (displayTopK(teams) != 0)
                {
                    printf("Error in displaying Top K.\n");
                }
                break;

            case DISPLAY_ROLE_ACROSS_TEAMS:
                if (displayRoleOfTeam(teams) != 0)
                {
                    printf("Error in displaying role across teams.\n");
                }
                break;

            case EXIT:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    freeAllMemory(teams);
    free(teams);
    return exitCode;   
}
