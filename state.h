#pragma once
#include <stdio.h>

// Remember to update the state reader/writer in state.c
// and to fix get_state_pointers() for these fields
#define DEFAULT_STATE { 0,0,0,0,0 }
#define NSTATE_FIELDS (5) // There are 5 fields in the State struct
struct State {
  long turn;
  long coin_x;
  long rotation;
  long cannon;
  long tri_x;
};

extern int read_state_string(const char* buffer, struct State* s);
extern char* string_state(struct State* s,char* buffer,unsigned n);
extern int dump_state(struct State* s);
extern int read_state(FILE* fp, struct State* s);
