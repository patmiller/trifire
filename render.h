#pragma once
#include <stdint.h>
#include "state.h"

extern int sprite(uint32_t* area,
		  unsigned area_width, unsigned area_height,
		  uint32_t* sprite,
		  unsigned sprite_width, unsigned sprite_height,
		  unsigned x, unsigned y);

extern int render(uint32_t* area,
		  unsigned area_width, unsigned area_height,
		  struct State* state);
