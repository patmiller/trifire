#include "play.h"

#include <stdio.h>
#include <stdint.h>

#include "render.h"
#include "state.h"
#include "sha1.h"
#include "penrose.h"

int play(struct State* state, char command) {
  // We are required to adjust the state (we do not allow overflow
  if (state->turn == 0x7fffffffffffffff) return 0;

  // We get random numbers by taking the SHA1 hash of the state
  uint8_t digest[20];
  SHA1(digest,state,sizeof(struct State));
  
  // Update the state according to the command
  switch(command) {
  case 's': {
    // Spin the trifire
    state->rotation = ( state->rotation + 1 )%3;
  } break;
  case 'r': {
    // translate right
    state->tri_x += 32;
    if (state->tri_x >= 640) return 0;
  } break;
  case 'l': {
    // translate left
    if (state->tri_x < 32) return 0;
    state->tri_x -= 32;
  } break;
  default: return 0;
  }

  // We always advance state
  state->turn++;

  return 1;
}
