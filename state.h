#pragma once
#include <stdio.h>

// Remember to update the state reader/writer in main.c
#define DEFAULT_STATE { 0,0,0,0,0 }
struct State {
  unsigned long turn;
  unsigned int coin_x;
  unsigned int rotation;
  unsigned int cannon;
  unsigned int tri_x;
};

extern int read_state(FILE* fp, struct State* s);
extern int dump_state(struct State* s);
