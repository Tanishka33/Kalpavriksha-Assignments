#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "Players_data.h"

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

    while (userChoice != 6)
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
            case 1:
                status = addPlayer(teams);
                if (status != 0)
                {
                    printf("Error: Could not add player.\n");
                }
                break;

            case 2:
                status = displayTeamPlayers(teams);
                if (status != 0)
                {
                    printf("Error in displaying team players.\n");
                }
                break;

            case 3:
                status = displayTeamAvgScore(teams);
                if (status != 0)
                {
                    printf("Error in displaying avg strike rates.\n");
                }
                break;

            case 4:
                status = displayTopK(teams);
                if (status != 0)
                {
                    printf("Error in displaying Top K.\n");
                }
                break;

            case 5:
                status = displayRoleOfTeam(teams);
                if (status != 0)
                {
                    printf("Error in displaying role across teams.\n");
                }
                break;

            case 6:
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
