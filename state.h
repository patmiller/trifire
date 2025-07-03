// Copyright 2025 Patrick Miller
#pragma once
#include <stdio.h>

// Remember to update the state reader/writer in state.c
// and to fix get_state_pointers() for these fields
#define DEFAULT_STATE { 0, 0, 0, 0, 0, 0, 0, 0 }
#define NSTATE_FIELDS (8)  // There are 8 fields in the State struct
struct State {
  long turn;             // 0
  long points;           // 1
  long tri_x;            // 2
  long rotation;         // 3
  long coin_x;           // 4
  long coin_y;           // 5
  long cannon_t;         // 6
  long cannon_offset;    // 7
};

extern int read_state_string(const char* buffer, struct State* s);
extern char* string_state(struct State* s, char* buffer, unsigned n);
extern int dump_state(struct State* s);
extern int read_state(FILE* fp, struct State* s);
void array_to_state(long* state_array, struct State* state);
void state_to_array(long* state_array, struct State* state);
