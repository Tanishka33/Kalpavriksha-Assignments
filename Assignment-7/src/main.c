#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "Players_data.h"

typedef enum {
    ADD_PLAYER = 1,
    DISPLAY_TEAM_PLAYERS,
    DISPLAY_TEAMS_AVG_SR,
    DISPLAY_TOP_K,
    DISPLAY_ROLE_ACROSS_TEAMS,
    EXIT
} MenuOption;

int main(void)
{
    Team teams[TEAM_COUNT];
    int userChoice = 0;
    int status = 0;

    status = initializeTeams(teams);
    if (status != 0)
    {
        printf("Error: Could not initialize teams.\n");
        return 1;
    }

    status = loadInitialPlayers(teams);
    if (status != 0)
    {
        printf("Error: Could not load initial players.\n");
        return 1;
    }

    while (userChoice != EXIT)
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

        scanf("%d", &userChoice);

        switch (userChoice)
        {
            case ADD_PLAYER:
                status = addPlayer(teams);
                if (status != 0)
                {
                    printf("Error: Could not add player.\n");
                }
                break;

            case DISPLAY_TEAM_PLAYERS:
                status = displayTeamPlayers(teams);
                if (status != 0)
                {
                    printf("Error in displaying team players.\n");
                }
                break;

            case DISPLAY_TEAMS_AVG_SR:
                status = displayTeamAvgScore(teams);
                if (status != 0)
                {
                    printf("Error in displaying avg strike rates.\n");
                }
                break;

            case DISPLAY_TOP_K:
                status = displayTopK(teams);
                if (status != 0)
                {
                    printf("Error in displaying Top K.\n");
                }
                break;

            case DISPLAY_ROLE_ACROSS_TEAMS:
                status = displayRoleOfTeam(teams);
                if (status != 0)
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

    return 0;   
}
