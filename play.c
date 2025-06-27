// Copyright 2025 - Patrick Miller

#include "play.h"

#include <stdio.h>
#include <stdint.h>

#include "penrose.h"
#include "ball.h"
#include "render.h"
#include "sha1.h"
#include "state.h"
#include "trajectory.h"




int play(struct State* state, char command) {
  // We are required to adjust the state (we do not allow overflow
  if (state->turn == 0x7fffffffffffffff) return 0;

  switch (command) {
  case 's': {
    // Spin the trifire
    state->rotation = (state->rotation + 1)%3;
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
  //Fire the trifire
  case 'f': {
    // We get random numbers by taking the SHA1 hash of the state
    uint8_t digest[20];
    SHA1(digest, state, sizeof(struct State));
    // We can use the first 4 bytes of the digest to determine the next action
    int randint = digest[0] | digest[1] << 8 | digest[2] << 16 | digest[3] << 24;

    if (state->cannon_t != 0) return 0; // No cannon to fire or cannon ball already in flight

    //recoil on the x axis based on the rotation
    if (state->rotation == 0) {
      // Move the trifire to the right a little bit
      if (state->tri_x > 640 - 96) return 0; // Can't move right
      state->tri_x += 32*(1 + randint % 2);
      // If we go too far right correct it
      if(state->tri_x > 640 - 96) {
        state->tri_x = 640 - 96;
      }
    } else if (state->rotation == 1) {
      // Move the trifire to the left
      if (state->tri_x < 32) return 0; // Can't move left
      state->tri_x -= 32*(3 + randint % 2);
      // If we go too far left correct it
      if (state->tri_x < 0) {
        state->tri_x = 0;
      }
    } else {
      // Move the trifire right a little bit
      if (state->tri_x > 640 - 96) return 0;
      state->tri_x += 32*(1 + randint % 2);
      // If we go too far right correct it
      if(state->tri_x > 640 - 96) {
        state->tri_x = 640 - 96;
      }
    }

    // TODO: Maybe set before the recoil?

    //Set cannon trajectory
    state->cannon_t = state->rotation * 17 + (state->tri_x / 32) + 1;

    //Spin the trifire regardless of whether or not we are able to move from the recoil we still spin
    state->rotation = ( state->rotation + 1 ) % 3;

  } break;
  default: return 0;
  }

  // We always advance state
  state->turn++;

  // If the cannonball is active update its position
  if( state->cannon_t != 0 ) {
    state->cannon_offset++;

    // TODO: See if we hit the coin
    
    // Kill the cannon ball if we have gone offscreen
    if (trajectories[state->cannon_t][state->cannon_offset].x == 65536) {
      trajectories[state->cannon_t] = 0;
    }
  }
  return 1;
}
