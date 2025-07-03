// Copyright 2025 - Patrick Miller

#pragma once
#include <stdint.h>
#include "state.h"

extern int sprite(uint32_t* area,
                  unsigned area_width, unsigned area_height,
                  uint32_t* sprite,
                  unsigned sprite_width, unsigned sprite_height,
                  int x, int y);

extern int render(uint32_t* area,
                  unsigned area_width, unsigned area_height,
                  struct State* state);

extern int renderlong(int* area,
                  unsigned area_width, unsigned area_height,
                  long* state_array);
