#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "Players_data.h"

int initializeTeams(Team teamsArr[])
{
    for(int index = 0; index < TEAM_COUNT; index++)
    {
        teamsArr[index].teamId = index + 1;
        strcpy(teamsArr[index].teamName, teams[index]);

        teamsArr[index].totalPlayers = 0;
        teamsArr[index].avgBattingStrikeRate = 0.0f;

        teamsArr[index].roleHead[0] = NULL;
        teamsArr[index].roleHead[1] = NULL;
        teamsArr[index].roleHead[2] = NULL;
    }
    return 0;
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
    if (team == NULL)
    {
        status = 1;
    }
    else
    {
        int total = 0;
        float strikeRateSum = 0.0f;
        int strikeRateCount = 0;

        for (int role = 0; role < 3; role++)
        {
            PlayerNode *curr = team->roleHead[role];

            while (curr != NULL)
            {
                total++;
                if (curr->role == ROLE_BATSMAN || curr->role == ROLE_ALLROUNDER)
                {
                    strikeRateSum += curr->strikeRate;
                    strikeRateCount++;
                }
                curr = curr->next;
            }
        }
        team->totalPlayers = total;
        if (strikeRateCount > 0)
        {
            team->avgBattingStrikeRate = strikeRateSum / strikeRateCount;
        }
        else
        {
            team->avgBattingStrikeRate = 0.0f;
        }
    }
    return status;
}

int findTeamIndexByName(const char *name, Team teamsArr[])
{
    int result = -1;

    if (name && teamsArr)
    {

        int left = 0;
        int right = TEAM_COUNT - 1;
        while (left <= right)
        {
            int mid = (left + right) / 2;
            int compare = strcmp(name, teamsArr[mid].teamName);
            if (compare == 0)
            {
                result = mid;
                break;
            }
            else if (compare > 0)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
    return result;
}

int loadInitialPlayers(Team teamsArr[])
{
    int status = 0;
    for (int index = 0; index < playerCount && status == 0; index++)
    {
        PlayerNode *player = malloc(sizeof(PlayerNode));
        if (!player)
        {
            printf("Memory allocation failed.\n");
            status = 1;
        }
        else
        {
            player->playerId = players[index].id;
            strncpy(player->name, players[index].name, MAX_NAME_LENGTH - 1);
            player->name[MAX_NAME_LENGTH - 1] = '\0';

            if (strcmp(players[index].role, "Batsman") == 0)
            {
                player->role = ROLE_BATSMAN;
            }
            else if (strcmp(players[index].role, "Bowler") == 0)
            {
                player->role = ROLE_BOWLER;
            }
            else if (strcmp(players[index].role, "All-rounder") == 0)
            {
                player->role = ROLE_ALLROUNDER;
            }
            else
            {
                player->role = ROLE_UNKNOWN;
            }

            player->totalRuns = players[index].totalRuns;
            player->battingAverage = players[index].battingAverage;
            player->strikeRate = players[index].strikeRate;
            player->wickets = players[index].wickets;
            player->economyRate = players[index].economyRate;

            computePerformanceIndex(player);
            player->next = NULL;
            int teamIndex = findTeamIndexByName(players[index].team, teamsArr);
            if (teamIndex == -1)
            {
                free(player);
            }
            else
            {
                insertPlayerSorted(&teamsArr[teamIndex].roleHead[player->role], player);
            }
        }
    }
    if (status == 0)
    {
        for (int team = 0; team < TEAM_COUNT; team++)
        {
            updateTeamStats(&teamsArr[team]);
        }
    }
    return status;
}

int addPlayer(Team teamsArr[])
{
    int status = 0;
    int teamIdInput = 0;
    printf(" Enter Team ID to add player: ");
    scanf("%d", &teamIdInput);

    if (teamIdInput < 1 || teamIdInput > TEAM_COUNT)
    {
        printf(" Invalid Team ID.\n");
        status = 1;
    }
    PlayerNode *newPlayer = NULL;
    int teamIndex = 0;
    if (status == 0)
    {
        teamIndex = teamIdInput - 1;

        newPlayer = malloc(sizeof(PlayerNode));
        if (!newPlayer)
        {
            printf(" Memory allocation failed.\n");
            status = 2;
        }
    }
    if (status == 0)
    {
        printf(" Player ID: ");
        scanf("%d", &newPlayer->playerId);

        for (int check = 0; check < 3; check++)
        {
            PlayerNode *scan = teamsArr[teamIndex].roleHead[check];
            while (scan)
            {
                if (scan->playerId == newPlayer->playerId)
                {
                    printf(" Duplicate player ID. Player not added.\n");
                    status = 3;
                    break;
                }
                scan = scan->next;
            }
            if (status != 0)
            {
                break;
            }
        }
    }
    if (status == 0)
    {
        printf(" Name: ");
        scanf(" %63[^\n]", newPlayer->name);

        int roleChoice = 0;
        printf(" Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
        scanf("%d", &roleChoice);

        if (roleChoice == 1)
        {
            newPlayer->role = ROLE_BATSMAN;
        }
        else if (roleChoice == 2)
        {
            newPlayer->role = ROLE_BOWLER;
        }
        else if (roleChoice == 3)
        {
            newPlayer->role = ROLE_ALLROUNDER;
        }
        else
        {
            printf(" Invalid role.\n");
            status = 4;
        }
    }
    if (status == 0)
    {
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

        insertPlayerSorted(&teamsArr[teamIndex].roleHead[newPlayer->role], newPlayer);
        updateTeamStats(&teamsArr[teamIndex]);
        printf(" Player added successfully to Team %s!\n", teamsArr[teamIndex].teamName);
    }
    if (status != 0 && newPlayer != NULL)
    {
        free(newPlayer);
    }
    return status;
}

void printPlayerRow(const PlayerNode *Player)
{
    if(!Player)
    {
        return;
    }
    const char *roleStr = 
        (Player->role == ROLE_BATSMAN) ? "Batsman" :
        (Player->role == ROLE_BOWLER) ? "Bowler" :
        (Player->role == ROLE_ALLROUNDER) ? "All-rounder" : "Unknown";

    printf(" %-5d %-20s %-12s %-7d %-8.2f %-8.2f %-7d %-8.2f %-10.2f\n",
        Player->playerId,
        Player->name,
        roleStr,
        Player->totalRuns,
        Player->battingAverage,
        Player->strikeRate,
        Player->wickets,
        Player->economyRate,
        Player->performanceIndex
    );
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

int displayTeamPlayers(Team teamsArr[])
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
    printf(" Players of Team %s:\n", teamsArr[teamIndex].teamName);
    printf(" ====================================================================================\n");
    printf(" ID   Name                 Role        Runs   Avg     SR      Wkts   ER     Perf.Index\n");
    printf(" ====================================================================================\n");

    int printedCount = 0;
    for(int roleOrder = 0; roleOrder < 3; roleOrder++)
    {
        PlayerNode *curr = teamsArr[teamIndex].roleHead[roleOrder];
        while (curr)
        {
            printPlayerRow(curr);
            printedCount++;
            curr = curr->next;
        }
    }
    printf(" ===================================================================================\n");
    printf(" Total Players: %d\n", teamsArr[teamIndex].totalPlayers);
    printf(" Average Batting Strike Rate: %.2f\n", teamsArr[teamIndex].avgBattingStrikeRate);
    return 0;
}

int displayTeamAvgScore(Team teamsArr[])
{
    Team tempTeams[TEAM_COUNT];
    for(int index = 0; index < TEAM_COUNT; index++)
    {
        tempTeams[index] = teamsArr[index];
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
    printf(" %-5s %-20s %-12s %-15s\n",
           "ID", "Team Name", "Avg Bat SR", "Total Players");

    for(int index = 0; index < TEAM_COUNT; index++)
    {
        printf(" %-5d %-20s %-12.2f %-15d\n", 
            tempTeams[index].teamId, 
            tempTeams[index].teamName, 
            tempTeams[index].avgBattingStrikeRate,
            tempTeams[index].totalPlayers);
    }
    return 0;
}

int displayTopK(Team teamsArr[])
{
    int status = 0;
    int teamIdInput = 0;
    int teamIndex = 0;
    int roleChoice = 0;
    int roleIndex = -1;
    int kValue = 0;
    printf(" Enter Team ID: ");
    scanf("%d", &teamIdInput);
    if (teamIdInput < 1 || teamIdInput > TEAM_COUNT)
    {
        printf(" Invalid Team ID.\n");
        status = 1;
    }
    else
    {
    teamIndex = teamIdInput - 1;
    }
    if(status == 0)
    {
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
            status = 2;
        }
    }
    if(status == 0)
    {
        printf(" Enter K: ");
        scanf("%d", &kValue);

        if (kValue <= 0)
        {
            printf(" Invalid K value.\n");
            status = 3;
        }
    }
    if(status == 0)
    {
        printf("\n Top %d %s of Team %s:\n",
                kValue,
                (roleIndex == ROLE_BATSMAN) ? "Batsmen" :
                (roleIndex == ROLE_BOWLER)  ? "Bowlers" : "All-rounders",
                teamsArr[teamIndex].teamName);
        printf(" ==============================================================================================\n");
        printf(" ID    Name                 Role         Runs    Avg      SR       Wkts    ER       Perf.Index\n");
        printf(" ==============================================================================================\n");
    
        PlayerNode *curr = teamsArr[teamIndex].roleHead[roleIndex];
        int count = 0;
        while(curr && count < kValue)
        {
            printPlayerRow(curr);
            curr = curr->next;
            count++;
        }
        if(count == 0)
        {
            printf(" No players available for this role in this team.\n");
        }
    }
    return status;
}

int displayRoleOfTeam(Team teamsArr[])
{
    int status = 0;
    int roleChoice = 0;
    int roleIndex = -1;
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
        status = 1;
    }

    int totalPlayersOfRole = 0;
    if(status == 0)
    {
        for(int teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++)
        {
            PlayerNode *curr = teamsArr[teamIndex].roleHead[roleIndex];
            while (curr)
            {
                totalPlayersOfRole++;
                curr = curr->next;
            }
        }
        if(totalPlayersOfRole == 0)
        {
            printf(" No players found for this role.\n");
            status = 2;
        }
    }
    PlayerNode **arr = NULL;
    if(status == 0)
    {
        arr = malloc(sizeof(PlayerNode*) * totalPlayersOfRole);
        if(!arr)
        {
        printf(" Memory allocation failed.\n");
        status = 3;
        }
    }
    int fillIndex = 0;
    if(status == 0)
    {
        for (int teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++)
        {
            PlayerNode *curr = teamsArr[teamIndex].roleHead[roleIndex];
            while (curr)
            {
                arr[fillIndex++] = curr;
                curr = curr->next;
            }
        }
        for (int currentIndex = 0; currentIndex < totalPlayersOfRole - 1; currentIndex++)
        {
            for (int comparingIndex = currentIndex + 1; comparingIndex < totalPlayersOfRole; comparingIndex++)
            {
                if (arr[comparingIndex]->performanceIndex > arr[currentIndex]->performanceIndex)
                {
                    PlayerNode *temp = arr[currentIndex];
                    arr[currentIndex] = arr[comparingIndex];
                    arr[comparingIndex] = temp;
                }
            }
        }
    }
    if (status == 0)
    {
        printf("\n All %s across all teams:\n",
               (roleIndex == ROLE_BATSMAN) ? "Batsmen" : (roleIndex == ROLE_BOWLER) ? "Bowlers"
                                                                                    : "All-rounders");

        printf(" ====================================================================================\n");
        printf(" ID   Name                 Role        Runs   Avg     SR      Wkts   ER     Perf.Index\n");
        printf(" ====================================================================================\n");
        for (int index = 0; index < totalPlayersOfRole; index++)
        {
            const char *roleStr =
                (arr[index]->role == ROLE_BATSMAN) ? "Batsman" : (arr[index]->role == ROLE_BOWLER) ? "Bowler"
                                                                                                   : "All-rounder";
            printf(" %-5d %-20s %-12s %-7d %-7.2f %-7.2f %-7d %-7.2f %-10.2f\n",
                   arr[index]->playerId,
                   arr[index]->name,
                   roleStr,
                   arr[index]->totalRuns,
                   arr[index]->battingAverage,
                   arr[index]->strikeRate,
                   arr[index]->wickets,
                   arr[index]->economyRate,
                   arr[index]->performanceIndex);
        }
        printf(" ===============================================================================================\n");
    }
    if (arr)
    {
        free(arr);
    }
    return status;
}

void freeAllMemory(Team teamsArr[])
{
    for(int teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++)
    {
        for(int roleIndex = 0; roleIndex < 3; roleIndex++)
        {
            PlayerNode *currentNode = teamsArr[teamIndex].roleHead[roleIndex];
            while (currentNode != NULL)
            {
                PlayerNode *nextNode = currentNode->next;
                free(currentNode);
                currentNode = nextNode;
            }
            teamsArr[teamIndex].roleHead[roleIndex] = NULL;
        }
        teamsArr[teamIndex].totalPlayers = 0;
        teamsArr[teamIndex].avgBattingStrikeRate = 0.0f;
    }
}