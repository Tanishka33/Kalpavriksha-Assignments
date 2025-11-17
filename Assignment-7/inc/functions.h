#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include <stdbool.h>

#define TEAM_COUNT 10
#define MAX_NAME_LENGTH 64

typedef enum
{
    ROLE_BATSMAN = 0,
    ROLE_BOWLER = 1,
    ROLE_ALLROUNDER = 2,
    ROLE_UNKNOWN = 3
} Role;

typedef struct  PlayerNode
{
    int playerId;
    char name[MAX_NAME_LENGTH];
    Role role;

    int totalRuns;
    float battingAverage;
    float strikeRate;

    int wickets;
    float economyRate;

    float performanceIndex;

    struct PlayerNode *next; 
} PlayerNode;

typedef struct 
{
    int teamId;
    char teamName[MAX_NAME_LENGTH];
    int totalPlayers;
    float avgBattingStrikeRate;

    PlayerNode *roleHead[3];
} Team;

int initializeTeams(Team teams[]);
int loadInitialPlayers(Team teams[]);

int computePerformanceIndex(PlayerNode *p);

int insertPlayerSorted(PlayerNode **head, PlayerNode *p);
void printPlayersList(const PlayerNode *head, int limit);

int findTeamIndexByName(const char *name, Team teams[]);
int updateTeamStats(Team *team);

int addPlayer(Team teams[]);
int displayTeamPlayers(Team teams[]);
int displayTeamAvgScore(Team teams[]);
int displayTopK(Team teams[]);
int displayRoleOfTeam(Team teams[]);

void printPlayerRow(const PlayerNode *P);
void freeAllMemory(Team teams[]);

#endif


