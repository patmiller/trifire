// Copyright 2025 - Patrick Miller

#include "play.h"

#include <stdio.h>
#include <stdint.h>

#include "sha1.h"
#include "state.h"
#include "trajectory.h"
#include "penrose.h"

// We get random numbers by taking the SHA1 hash of the state
static int random_from_state(struct State* state) {
    uint8_t digest[20];
    SHA1(digest, state, sizeof(struct State));

    // We can use the first 4 bytes of the digest to determine the next action
    return digest[0] | digest[1] << 8 | digest[2] << 16 | digest[3] << 24;
}

int play(struct State* state, char command) {
  // We are required to adjust the state (we do not allow overflow)
  if (state->turn == 0x7fffffffffffffff) return 0;

  // If the cannon ball hit the coin last turn, we remove both
  // (the explosion would have rendered last frame).  The coin
  // will be replaced below
  if ( state->cannon_t &&
       trajectories[state->cannon_t][state->cannon_offset].x
       == state->coin_x &&
       trajectories[state->cannon_t][state->cannon_offset].y
       == state->coin_y ) {
    state->points += 1;  // Score!
    state->cannon_t = 0;
    state->cannon_offset = 0;
    state->coin_x = 0;
    state->coin_y = 0;
  }

  // With no coin, we add one
  if (state->coin_x == 0 && state->coin_y == 0) {
    int randint = random_from_state(state);
    if (randint < 0) randint = -randint;
    int coin = randint%NCOINS;
    state->coin_x = coins[coin].x;
    state->coin_y = coins[coin].y;
  }

  switch (command) {
  case 's': case 'S': {
    // Spin the trifire
    state->rotation = (state->rotation + 1)%3;
  } break;

  case 'r': case 'R': {
    // translate right (watch boundary)
    if (state->tri_x + 32 + PENROSE_WIDTH >= 640) return 0;
    state->tri_x += 32;
  } break;

  case 'l': case 'L': {
    // translate left
    if (state->tri_x < 32) return 0;
    state->tri_x -= 32;
  } break;

  // Fire the trifire
  case 'f': case 'F': {
     // No cannon to fire or cannon ball already in flight
    if (state->cannon_t != 0) return 0;

    // We get random numbers by taking the SHA1 hash of the state
    // We can use the first 4 bytes of the digest to determine the next action
    int randint = random_from_state(state);

    // Set cannon trajectory before the recoil
    state->cannon_t = state->rotation * 18 + (state->tri_x / 32) + 1;
    
    // Recoil on the x axis based on the rotation
    if (state->rotation == 0) {
      // Move the trifire to the right a little bit
      if ( state->tri_x < 640-32 ) {
        state->tri_x += 32*(1 + randint % 2);
        // If we go too far right correct it
        if (state->tri_x > 640 - 96) {
          state->tri_x = 640 - 96;
        }
      }
    } else if (state->rotation == 1) {
      // Move the trifire to the left
      if (state->tri_x >= 32) {
        state->tri_x -= 32*(3 + randint % 2);
        // If we go too far left correct it
        if (state->tri_x < 0) {
          state->tri_x = 0;
        }
      }
    } else {
      // Move the trifire right a little bit
      if (state->tri_x <= 640 - 96) {
        state->tri_x += 32*(1 + randint % 2);
        // If we go too far right correct it
        if (state->tri_x > 640 - 96) {
          state->tri_x = 640 - 96;
        }
      }
    }

    // Spin the trifire regardless of whether or not we are able to move
    // from the recoil we still spin
    state->rotation = (state->rotation + 1) % 3;
  } break;

  default:
    return 0;
  }

  // We always advance state
  state->turn++;

  // If the cannonball is active update its position
  if (state->cannon_t != 0) {
    // Kill the cannon ball if we have gone offscreen (miss)
    if (trajectories[state->cannon_t][state->cannon_offset].x == 65535) {
      state->cannon_t = 0;
      state->cannon_offset = 0;
    } else {
      state->cannon_offset++;
    }
  }
  return 1;
}

// This is just for the monolith version (swift interface)
int playlong(char command,long* state_array) {
  struct State state;
  array_to_state(state_array, &state);

  int result = play(&state,command);

  state_to_array(state_array, &state);

  // We want to return 2 if we will render an explosion
  // here
  if (result && state.cannon_t != 0) {
    unsigned ball_x = trajectories[state.cannon_t][state.cannon_offset].x;
    unsigned ball_y = trajectories[state.cannon_t][state.cannon_offset].y;
    // If the cannon ball has hit the coin, we only draw an explosion
    if (ball_x == state.coin_x && ball_y == state.coin_y) {
      return 2;
    }
  }

  return result;
}
