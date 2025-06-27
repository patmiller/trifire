// Copyright 2025 - Patrick Miller

#include "render.h"

#include <stdint.h>

#include "penrose.h"
#include "ball.h"
#include "trajectory.h"
#include "state.h"

int sprite(uint32_t* area,
           unsigned area_width, unsigned area_height,
           uint32_t* sprite,
           unsigned sprite_width, unsigned sprite_height,
           unsigned x, unsigned y
           ) {
  for (unsigned j = 0; j < sprite_height; ++j) {
    for (unsigned i = 0; i < sprite_width; ++i) {
      unsigned pixel = sprite[i + j*sprite_width];
      unsigned xx = x + i;
      unsigned yy = y + j;
      if ( xx > area_width) return 0;
      if ( yy > area_height ) return 0;
      area[xx + yy*area_width] = pixel;
    }
  }
  return 1;
}

int render(uint32_t* area,
           unsigned area_width, unsigned area_height,
           struct State* state) {
  // We need to render the scene.  The only sprite active right now is
  // the trifire and we need to add coins and the cannon
  if ( !sprite(area, area_width, area_height,
               penrose[state->rotation],
               PENROSE_WIDTH, PENROSE_HEIGHT,
               state->tri_x, 235) ) return 0;

  //check if the cannonball is active
  if (state->cannon_t != 0) {
    //if cannon ball is active, we need to render it
    if (!sprite(area, area_width, area_height,
		ball, BALL_WIDTH, BALL_HEIGHT,
		trajectories[state->cannon_t][state->cannon_offset].x,
		trajectories[state->cannon_t][state->cannon_offset].y)) {
      return 0;
    }
  }
  
  return 1;
}
