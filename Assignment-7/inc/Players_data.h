#ifndef PLAYERS_DATA_H
#define PLAYERS_DATA_H

typedef struct {
    int id;
    const char* name;
    const char* team;
    const char* role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
} Player;

extern const Player players[];
extern const char* teamNames[];
extern int teamCount;
extern int playerCount;

#endif