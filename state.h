#pragma once
#include <stdio.h>

// Remember to update the state reader/writer in state.c
// and to fix get_state_pointers() for these fields
#define DEFAULT_STATE { 0,0,0,0,0,0,0,0 }
#define NSTATE_FIELDS (8) // There are 8 fields in the State struct
struct State {
  long turn;
  long coin_x;
  long coin_y;
  long rotation;
  long cannon;
  long cannon_t;
  long cannon_offset;
  long tri_x;
};

extern int read_state_string(const char* buffer, struct State* s);
extern char* string_state(struct State* s,char* buffer,unsigned n);
extern int dump_state(struct State* s);
extern int read_state(FILE* fp, struct State* s);
