// Copyright 2025 - Patrick Miller

#include "render.h"

#include <stdint.h>
#include <string.h>

#include "ball.h"
#include "bricks.h"
#include "explosion.h"
#include "penrose.h"
#include "penrosecoin.h"
#include "trajectory.h"
#include "state.h"

// x and y are ints so that we can start a (clipped) sprite
// that may be outside the drawable area
int sprite(uint32_t* area,
           unsigned area_width, unsigned area_height,
           uint32_t* sprite,
           unsigned sprite_width, unsigned sprite_height,
           int x, int y
           ) {
  for (unsigned j = 0; j < sprite_height; ++j) {
    for (unsigned i = 0; i < sprite_width; ++i) {
        unsigned pixel = sprite[i + j*sprite_width];
        if (pixel != 0xFFFFFF) {
          int xx = x + i;
          int yy = y + j;
          // Ignore pixels that render outside the area
          if ( xx < 0 || xx >= (int)area_width) continue;
          if ( yy < 0 || yy >= (int)area_height) continue;
          area[xx + yy*area_width] = pixel;
        }
    }
  }
  return 1;
}

int render(uint32_t* area,
           unsigned area_width, unsigned area_height,
           struct State* state) {
  // We need to render the scene.  Put the trifire in as a bottom
  // sprite
  if ( !sprite(area, area_width, area_height,
               penrose[state->rotation],
               PENROSE_WIDTH, PENROSE_HEIGHT,
               state->tri_x, 235) ) return 0;

  // Check if the cannonball is active. If so, there will be a coin and possibly
  // an explosion
  if (state->cannon_t != 0) {
    unsigned ball_x = trajectories[state->cannon_t][state->cannon_offset].x;
    unsigned ball_y = trajectories[state->cannon_t][state->cannon_offset].y;
    // If the cannon ball has hit the coin, we only draw an explosion
    if (ball_x == state->coin_x && ball_y == state->coin_y) {
      // Explosion centered on coin
      if (!sprite(area, area_width, area_height,
                  explosion, EXPLOSION_WIDTH, EXPLOSION_HEIGHT,
                  state->coin_x-PENROSECOIN_WIDTH/2,
                  state->coin_y-PENROSECOIN_HEIGHT/2)) {
        return 0;
      }

    } else {
      // Draw the cannon ball and the coin
      // Coin
      if (!sprite(area, area_width, area_height,
                  penrosecoin, PENROSECOIN_WIDTH, PENROSECOIN_HEIGHT,
                  state->coin_x-PENROSECOIN_WIDTH/2,
                  state->coin_y-PENROSECOIN_HEIGHT/2)) {
        return 0;
      }

      // Cannon ball
      if (!sprite(area, area_width, area_height,
                  ball, BALL_WIDTH, BALL_HEIGHT,
                  trajectories[state->cannon_t][state->cannon_offset].x,
                  trajectories[state->cannon_t][state->cannon_offset].y)) {
        return 0;
      }
    }

  } else if (state->coin_x > 0 && state->coin_y > 0) {
    // If only the coin is active, draw it
    // Coin
    if (!sprite(area, area_width, area_height,
                penrosecoin, PENROSECOIN_WIDTH, PENROSECOIN_HEIGHT,
                state->coin_x-PENROSECOIN_WIDTH/2,
                state->coin_y-PENROSECOIN_HEIGHT/2)) {
      return 0;
    }
  }

  return 1;
}

int renderlong(int* area,
	       unsigned area_width, unsigned area_height,
	       long* state_array) {
  struct State state;
  array_to_state(state_array, &state);
  
  memcpy(area, bricks, area_width*area_height*sizeof(int));
  int result = render((uint32_t*)area, area_width, area_height, &state);

  // This creates a BGR with full alpha from an RGB
  for(int i=0;i<area_width*area_height;++i) {
    uint32_t px = area[i];
    uint32_t alpha = 0xff;
    uint32_t red = (px & 0xff0000) >> 16;
    uint32_t green = (px & 0xff00) >> 8;
    uint32_t blue = (px & 0xff);
    area[i] = (alpha << 24) | (blue << 16) | (green << 8) | red;
  }
  
  state_to_array(state_array, &state);
  return result;

}
