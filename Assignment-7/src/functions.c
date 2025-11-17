#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "Players_data.h"

extern const char* teamNames[];
extern const Player players[];
extern int teamCount;
extern int playerCount;

static const int teamIdToTeamNamesIndex[TEAM_COUNT] = {
    4, 1, 3, 6, 5, 7, 2, 8, 0, 9 // mapping accordint to players_data.h
};

int initializeTeams(Team teams[])
{
    int status = 0;
    for(int index = 0; index < TEAM_COUNT; index++)
    {
        teams[index].teamId = index + 1;

        int mappedIndex = teamIdToTeamNamesIndex[index];
        strcpy(teams[index].teamName, teamNames[mappedIndex]);

        teams[index].totalPlayers = 0;
        teams[index].avgBattingStrikeRate = 0.0f;

        teams[index].roleHead[0] = NULL;
        teams[index].roleHead[1] = NULL;
        teams[index].roleHead[2] = NULL;
    }
cleanup: 
    return status;
}

int computePerformanceIndex(PlayerNode *p)
{
    int status = 0;
    if(p == NULL)
    {
        status = 1;
    }
    else if(p->role == ROLE_BATSMAN)
    {
        p->performanceIndex = (p->battingAverage * p->strikeRate) / 100.0f;
    }
    else if(p->role == ROLE_BOWLER)
    {
        p->performanceIndex = (p->wickets * 2) + (100.0f - p->economyRate);
    }
    else if(p->role == ROLE_ALLROUNDER)
    {
        p->performanceIndex = (p->battingAverage * p->strikeRate) / 100.0f + (p->wickets * 2);
    }
    else 
    {
        status = 2;
    }
    return status;
}

int insertPlayerSorted(PlayerNode **head, PlayerNode *p)
{
    int status = 0;
    if (head == NULL || p == NULL)
    {
        status = 1;
    }
    else
    {
        p->next = NULL;

        PlayerNode *scanner = *head;
        while (scanner != NULL)
        {
            if (scanner->playerId == p->playerId)
            {
                status = 2;
                break;
            }

            scanner = scanner->next;
        }
        if (status == 0)
        {
            if (*head == NULL)
            {
                *head = p;
            }
            else
            {
                if (p->performanceIndex > (*head)->performanceIndex)
                {
                    p->next = *head;
                    *head = p;
                }
                else
                {
                    PlayerNode *prev = *head;
                    PlayerNode *curr = prev->next;

                    while (curr != NULL && curr->performanceIndex >= p->performanceIndex)
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                    prev->next = p;
                    p->next = curr;
                }
            }
        }
    }
    return status;
}

int updateTeamStats(Team *team)
{
    int status = 0;
    if(team ==NULL)
    {
        status = 1;
        return status;
    }
    int total = 0;
    float strikeRateSum = 0.0f;
    int strikeRateCount = 0;

    for(int role = 0; role < 3; role++)
    {
        PlayerNode *curr = team->roleHead[role];

        while (curr != NULL)
        {
            total++;
            if(curr->role == ROLE_BATSMAN || curr->role == ROLE_ALLROUNDER)
            {
                strikeRateSum += curr->strikeRate;
                strikeRateCount++;
            }
            curr = curr->next;
        }
    }
    team->totalPlayers = total;
    if(strikeRateCount > 0)
    {
        team -> avgBattingStrikeRate = strikeRateSum / strikeRateCount;
    }
    else
    {
        team->avgBattingStrikeRate = 0.0f;
    }
    return status;
}

int findTeamIndexByName(const char *name, Team teams[])
{
    if(name == NULL || teams == NULL)
        return -1;

    for(int index = 0; index < TEAM_COUNT; index++)
    {
        if(strcmp(teams[index].teamName, name) == 0)
            return index;   
    }
    return -1;
}

int loadInitialPlayers(Team teams[])
{
    int status = 0;
    for(int index = 0; index < playerCount; index++)
    {
        PlayerNode *p = (PlayerNode *)malloc(sizeof(PlayerNode));
        if(p == NULL)
        {
            status = 1;
            break;
        }
        p->playerId = players[index].id;
        strncpy(p->name, players[index].name, MAX_NAME_LENGTH);
        p->name[MAX_NAME_LENGTH] = '\0';

        if(strcmp(players[index].role, "Batsman") == 0)
        {
            p->role = ROLE_BATSMAN;
        }
        else if(strcmp(players[index].role, "Bowler") == 0)
        {
            p->role = ROLE_BOWLER;
        }
        else if(strcmp(players[index].role, "All-rounder") == 0)
        {
            p->role = ROLE_ALLROUNDER;
        }
        else 
        {
            p->role = ROLE_UNKNOWN;
        }

        p->totalRuns = players[index].totalRuns;
        p->battingAverage = players[index].battingAverage;
        p->strikeRate = players[index].strikeRate;
        p->wickets = players[index].wickets;
        p->economyRate = players[index].economyRate;

        computePerformanceIndex(p);
        p->next = NULL;
        int teamIndex = findTeamIndexByName(players[index].team, teams);
        if(teamIndex == -1)
        {
            free(p);
            continue;
        }
        insertPlayerSorted(&teams[teamIndex].roleHead[p->role], p);
    }
    for(int team = 0; team < TEAM_COUNT; team++)
    {
        updateTeamStats(&teams[team]);
    }
    return status;   
}

// PlayerNode *createPlayerFromInput(void)
// {
//     PlayerNode *newPlayer = NULL;
//     int status = 0;
//     int roleChoice = 0;
//     char nameInput[MAX_NAME_LENGTH];

//     newPlayer = (PlayerNode *)malloc(sizeof(PlayerNode));
//     if(newPlayer == NULL)
//     {
//         status = 1;
//     }
//     if (status == 0)
//     {
//         printf("Enter Playes ID: ");
//         scanf("%d", &newPlayer->playerId);

//         printf("Enter PLayer name : ");
//         scanf(" %[^\n]s", nameInput);
//         strncpy(newPlayer->name, nameInput, MAX_NAME_LENGTH);
//         newPlayer->name[MAX_NAME_LENGTH - 1] = '\0';

//         printf("Enter role (Batsman-1 / Bowler-2 / All-rounder-3): ");
//         scanf(" %d", &roleChoice);

//         if (roleChoice == 1)
//             newPlayer->role = ROLE_BATSMAN;
//         else if (roleChoice == 2)
//             newPlayer->role = ROLE_BOWLER;
//         else
//             newPlayer->role = ROLE_ALLROUNDER;

//         printf("Enter Total Runs: ");
//         scanf("%d", &newPlayer->totalRuns);
//         printf("Enter Batting Average: ");
//         scanf("%f", &newPlayer->battingAverage);
//         printf("Enter Strike Rate: ");
//         scanf("%f", &newPlayer->strikeRate);
//         printf("Enter Wickets: ");
//         scanf("%d", &newPlayer->wickets);
//         printf("Enter Econony Rate: ");
//         scanf("%f", &newPlayer->economyRate);

//         newPlayer->next = NULL;
//         computePerformanceIndex(newPlayer);
//     }
//     return newPlayer;
// }

int addPlayer(Team teams[])
{
    int teamIdInput = 0;
    printf(" Enter Team ID to add player: ");
    scanf("%d", &teamIdInput);

    if (teamIdInput < 1 || teamIdInput > TEAM_COUNT)
    {
        printf(" Invalid Team ID.\n");
        return 1;
    }
    int teamIndex = teamIdInput - 1;

    PlayerNode *newPlayer = malloc(sizeof(PlayerNode));
    if(newPlayer == NULL)
    {
        printf(" Memory allocation failed.\n");
        return 2;
    }
    printf(" Player ID: ");
    scanf("%d", &newPlayer->playerId);

    printf(" Name: ");
    scanf(" %[^\n]", newPlayer->name);

    int roleChoice = 0;
    printf(" Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);

    if (roleChoice == 1)
        newPlayer->role = ROLE_BATSMAN;
    else if (roleChoice == 2)
        newPlayer->role = ROLE_BOWLER;
    else
        newPlayer->role = ROLE_ALLROUNDER;

    printf(" Total Runs: ");
    scanf("%d", &newPlayer->totalRuns);

    printf(" Batting Average: ");
    scanf("%f", &newPlayer->battingAverage);

    printf(" Strike Rate: ");
    scanf("%f", &newPlayer->strikeRate);

    printf(" Wickets: ");
    scanf("%d", &newPlayer->wickets);

    printf(" Economy Rate: ");
    scanf("%f", &newPlayer->economyRate);

    newPlayer->next = NULL;
    computePerformanceIndex(newPlayer);

    int insertResult = insertPlayerSorted(
        &teams[teamIndex].roleHead[newPlayer->role],
        newPlayer
    );

    if (insertResult != 0)
    {
        printf(" Duplicate Player ID. Player not added.\n");
        free(newPlayer);
        return 3;
    }

    updateTeamStats(&teams[teamIndex]);
    printf(" Player added successfully to Team %s!\n",
           teams[teamIndex].teamName);
    return 0;
}

void printPlayerRow(const PlayerNode *P)
{
    if(P == NULL)
    {
        return;
    }
    printf("%-5d %-20s %-12d %-10.2f %-10.2f %-8d %-10.2f %-10.2f\n",
    P->playerId, P->name, P->totalRuns, P->battingAverage, 
    P->strikeRate, P->wickets, P->economyRate, P->performanceIndex);
}

void printPlayersList(const PlayerNode *head, int limit)
{
    int count = 0;
    const PlayerNode *curr = head;
    while(curr != NULL)
    {
        printPlayerRow(curr);
        curr = curr->next;
        count++;
        if(limit > 0 && count >= limit)
        {
            break;
        }
    }
}

int displayTeamPlayers(Team teams[])
{
    int teamIdInput = 0;

    printf(" Enter Team ID: ");
    scanf("%d", &teamIdInput);

    if (teamIdInput < 1 || teamIdInput > TEAM_COUNT)
    {
        printf(" Invalid Team ID.\n");
        return 1;
    }
    int teamIndex = teamIdInput - 1;
    printf(" Players of Team %s:\n", teams[teamIndex].teamName);
    printf(" ====================================================================================\n");
    printf(" ID   Name                 Role        Runs   Avg     SR      Wkts   ER     Perf.Index\n");
    printf(" ====================================================================================\n");

    int total = 0;
    PlayerNode *curr = teams[teamIndex].roleHead[ROLE_BATSMAN];
    while (curr != NULL)
    {
        printf(" %-4d %-20s %-11s %-6d %-7.2f %-7.2f %-6d %-7.2f %-10.2f\n",
               curr->playerId,
               curr->name,
               "Batsman",
               curr->totalRuns,
               curr->battingAverage,
               curr->strikeRate,
               curr->wickets,
               curr->economyRate,
               curr->performanceIndex);
        total++;
        curr = curr->next;
    }
    curr = teams[teamIndex].roleHead[ROLE_BOWLER];
    while (curr != NULL)
    {
        printf(" %-4d %-20s %-11s %-6d %-7.2f %-7.2f %-6d %-7.2f %-10.2f\n",
               curr->playerId,
               curr->name,
               "Bowler",
               curr->totalRuns,
               curr->battingAverage,
               curr->strikeRate,
               curr->wickets,
               curr->economyRate,
               curr->performanceIndex);
        total++;
        curr = curr->next;
    }
    curr = teams[teamIndex].roleHead[ROLE_ALLROUNDER];
    while (curr != NULL)
    {
        printf(" %-4d %-20s %-11s %-6d %-7.2f %-7.2f %-6d %-7.2f %-10.2f\n",
               curr->playerId,
               curr->name,
               "All-rounder",
               curr->totalRuns,
               curr->battingAverage,
               curr->strikeRate,
               curr->wickets,
               curr->economyRate,
               curr->performanceIndex);
        total++;
        curr = curr->next;
    }
    printf(" ====================================================================================\n");
    printf(" Total Players: %d\n", total);
    printf(" Average Batting Strike Rate: %.2f\n", teams[teamIndex].avgBattingStrikeRate);

    return 0; 
}

int displayTeamAvgScore(Team teams[])
{
    Team tempTeams[TEAM_COUNT];
    for(int index = 0; index < TEAM_COUNT; index++)
    {
        tempTeams[index] = teams[index];
    }
    for(int currentTeamindex = 0; currentTeamindex < TEAM_COUNT - 1; currentTeamindex++)
    {
        for(int comparingTeamIndex = currentTeamindex + 1; comparingTeamIndex < TEAM_COUNT; comparingTeamIndex++)
        {
            if(tempTeams[comparingTeamIndex].avgBattingStrikeRate > tempTeams[currentTeamindex].avgBattingStrikeRate)
            {
                Team temp = tempTeams[currentTeamindex];
                tempTeams[currentTeamindex] = tempTeams[comparingTeamIndex];
                tempTeams[comparingTeamIndex] = temp;
            }
        }
    }
    printf("\n========================================\n");
    printf("Teams sorted by average batting strike rate\n");
    printf("=========================================\n\n");

    printf("%-5s %-20s %-10s\n", "ID", "Team Name", "Avg SR");

    for(int index = 0; index < TEAM_COUNT; index++)
    {
        printf("%-5d %-20s %-10.2f\n", 
            tempTeams[index].teamId, 
            tempTeams[index].teamName, 
            tempTeams[index].avgBattingStrikeRate);
    }
    return 0;
}

int displayTopK(Team teams[])
{
    int teamIdInput = 0;
    int roleChoice = 0;
    int kValue = 0;
    int roleIndex = 0;
    printf(" Enter Team ID: ");
    scanf("%d", &teamIdInput);
    if (teamIdInput < 1 || teamIdInput > TEAM_COUNT)
    {
        printf(" Invalid Team ID.\n");
        return 1;
    }

    int teamIndex = teamIdInput - 1;

    printf(" Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);
    if (roleChoice == 1)
    {
        roleIndex = ROLE_BATSMAN;
    }
    else if (roleChoice == 2)
    {
        roleIndex = ROLE_BOWLER;
    }
    else if (roleChoice == 3)
    {
        roleIndex = ROLE_ALLROUNDER;
    }
    else 
    {
        printf(" Invalid role choice.\n");
        return 2;
    }
    printf(" Enter K: ");
    scanf("%d", &kValue);

    if (kValue <= 0)
    {
        printf(" Invalid K value.\n");
        return 2;
    }
    printf(" ====================================================================================\n");
    printf(" ID   Name                 Runs   Avg     SR      Wkts   ER     Perf.Index\n");
    printf(" ====================================================================================\n");

    PlayerNode *curr = teams[teamIndex].roleHead[roleIndex];

    printf(" Top %d %s of Team %s:\n",
           kValue,
           (roleIndex == ROLE_BATSMAN) ? "Batsmen" :
           (roleIndex == ROLE_BOWLER) ? "Bowlers" : "All-rounders",
           teams[teamIndex].teamName);

    printf(" ====================================================================================\n");
    printf(" ID   Name                 Role        Runs   Avg     SR      Wkts   ER     Perf.Index\n");
    printf(" ====================================================================================\n");

    printPlayersList(curr, kValue);

    return 0;
}

int displayRoleOfTeam(Team teams[])
{
    int status = 0;
    int roleChoice = 0;
    int roleIndex = 0;
    printf(" Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);

    if (roleChoice == 1)
    {
        roleIndex = ROLE_BATSMAN;
    }
    else if (roleChoice == 2)
    {
        roleIndex = ROLE_BOWLER;
    }
    else if (roleChoice == 3)
    {
        roleIndex = ROLE_ALLROUNDER;
    }
    else
    {
        printf(" Invalid role choice.\n");
        return 1;
    }

    int totalPlayersOfRole = 0;
    for(int teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++)
    {
        PlayerNode *curr = teams[teamIndex].roleHead[roleIndex];
        while (curr != NULL)
        {
            totalPlayersOfRole++;
            curr = curr->next;
        }
    }
    PlayerNode* playerArray[600];
    int fillIndex = 0;
    for(int teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++)
    {
        PlayerNode *curr = teams[teamIndex].roleHead[roleIndex];
        while(curr != NULL)
        {
            playerArray[fillIndex++] = curr;
            curr = curr->next;
        }
    }
    for(int currentPlayerIndex = 0; currentPlayerIndex < totalPlayersOfRole - 1; currentPlayerIndex++)
    {
        for(int comparingPlayerIndex = currentPlayerIndex + 1; comparingPlayerIndex < totalPlayersOfRole; comparingPlayerIndex++){
        if (playerArray[comparingPlayerIndex]->performanceIndex > playerArray[currentPlayerIndex]->performanceIndex)
            {
                PlayerNode *temp = playerArray[currentPlayerIndex];
                playerArray[currentPlayerIndex] = playerArray[comparingPlayerIndex];
                playerArray[comparingPlayerIndex] = temp;
            }
        }
    }
    printf(" ====================================================================================\n");
    printf(" ID   Name                 Role        Runs   Avg     SR      Wkts   ER     Perf.Index\n");
    printf(" ====================================================================================\n");
    for (int i = 0; i < totalPlayersOfRole; i++)
    {
        printPlayerRow(playerArray[i]);
    }
    return 0;
}

void freeAllMemory(Team teams[])
{
    int teamIndex = 0;
    for(teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++)
    {
        for(int roleIndex = 0; roleIndex < 3; roleIndex++)
        {
            PlayerNode *currentNode = teams[teamIndex].roleHead[roleIndex];
            while (currentNode != NULL)
            {
                PlayerNode *nextNode = currentNode->next;
                free(currentNode);
                currentNode = nextNode;
            }
            teams[teamIndex].roleHead[roleIndex] = NULL;
        }
    }
}