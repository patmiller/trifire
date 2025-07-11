#pragma once

#include <stdint.h>
#include <math.h>

#define MAX_PLAYERS (10)
extern int voteThreshold = (int)(ceil(0.75 * MAX_PLAYERS));

typedef struct{
    char vote;
    char* playerName;
} Vote;

extern Vote votes[MAX_PLAYERS];

extern char countVotes(Vote votes[], struct State* state);