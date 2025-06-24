#include "play.h"

#include <stdio.h>
#include <stdint.h>

#include "render.h"
#include "state.h"
#include "sha1.h"
#include "penrose.h"


typedef struct {
    uint32_t state;
} PRNG;

void prng_seed(PRNG* prng, uint32_t seed) {
    prng->state = seed;
}

uint32_t prng_next(PRNG* prng) {
    prng->state = prng->state * 1664525 + 1013904223;
    return prng->state;
}

PRNG prng;
prng_seed(&prng, 12345); 

// Generate a random number in [min, max]
int random_in_range(PRNG* prng, int min, int max) {
    return min + (prng_next(prng) % (max - min + 1));
}

int play(struct State* state, uint32_t* area, char command) {
  // We are required to adjust the state (we do not allow overflow
  if (state->turn == 0x7fffffffffffffff) return 0;

  // We get random numbers by taking the SHA1 hash of the state
  uint8_t digest[20];
  SHA1(digest,state,sizeof(struct State));
  for(int i=0;i<20;++i) {
    printf("%d\n",(int)digest[i]);
  }
  
  // Update the state according to the command
  switch(command) {
  case 's': {
    // Spin the trifire
    state->rotation = ( state->rotation + 1 )%3;
  } break;
  case 'r': {
    // translate right
    if (state->tri_x >= 640) return 0;
    state->tri_x += 32;
  } break;
  case 'l': {
    // translate left
    if (state->tri_x < 32) return 0;
    state->tri_x -= 32;
  } break;
  case 'f': {
    // Fire the trifire
    if (state->cannon == 0) return 0; // No cannon to fire
    state->cannon--;
    //move on the x axis based on the rotation
    if (state->rotation == 0) {
      // Move the trifire to the right a little bit
      if (state->tri_x > 640 - 96) return 0; // Can't move right
      state->tri_x += 32*random_in_range(&prng, 1, 2);
      if(state->tri_x > 640 - 96) {
        // If we go too far right correct it
        state->tri_x = 640 - 96;
      }
    } else if (state->rotation == 1) {
      // Move the trifire to the left
      if (state->tri_x < 32) return 0; // Can't move left
      state->tri_x -= 32*random_in_range(&prng, 3, 4);
      if (state->tri_x < 0) {
        // If we go too far left correct it
        state->tri_x = 0;
      }
    } else {
      // Move the trifire right a little bit
      if (state->tri_x < 32) return 0; // Can't move down
      state->tri_x += 32*random_in_range(&prng, 1, 2);
      if(state->tri_x > 640 - 96) {
        // If we go too far right correct it
        state->tri_x = 640 - 96;
      }
    }
    //Spin the trifire
    //regardless of whether or not we are able to move from the recoil we still spin
    state->rotation = ( state->rotation + 1 )%3;
  } break;
  default: return 0;
  }

  // We always advance state
  state->turn++;

  // We need to render the scene.  The only sprite active right now is
  // the trifire and we need to add coins and the cannon
  if ( !render(area, 640,480, penrose[state->rotation], PENROSE_WIDTH, PENROSE_HEIGHT,
	       state->tri_x, 235) ) return 0;

  return 1;
}
