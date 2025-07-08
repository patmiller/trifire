#pragma once

#include <stdint.h>
#define MAX_PLAYERS (10)
#define voteThreshold (0.75 * MAX_PLAYERS)

typedef struct{
    char vote;
    char* playerName;
} Vote;

extern Vote votes[MAX_PLAYERS];

extern char countVotes(Vote votes[], int numVotes);