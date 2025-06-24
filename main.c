// Copyright 2025 - Patrick Miller
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "bricks.h"
#include "play.h"
#include "render.h"
#include "state.h"

// Update defaults in state.h
struct State state = DEFAULT_STATE;

int main(int argc, char** argv) {
  // Quit unless ./a.out <state> <command>
  if ( argc < 3 ) return 1;

  // We need the state number so we can try and read a state file
  long turn = strtol(argv[1], NULL, 10);
  if (errno == EINVAL) return 2;

  // Special case to build our state-0.json file
  if (turn < 0) {
    return dump_state(&state);
  }

  // Open the file with the state in it
  char filename[80];
  if (snprintf(filename, sizeof(filename), "state-%ld.json", turn) < 0) {
    return 3;
  }
  FILE* fp = fopen(filename, "r");
  if ( fp == NULL ) return 4;
  if ( !read_state(fp, &state) ) return 8;

  // Play the game with the command.  It only has to update
  // the state (turn must advance by 1)
  if (!play(&state, argv[2][0])) return 9;
  if (state.turn != turn + 1) return 10;

  // Now, build out the BMP with the new scene in it
  if (!render(bricks, BRICKS_WIDTH, BRICKS_HEIGHT, &state)) {
    return 11;
  }

  // I'll write it to a frame-nnnn.bmp AND to output.bmp
  // so I can cheaply look at my last item or see a little
  // movie
  if ( snprintf(filename, sizeof(filename), "frame-%ld.bmp", state.turn) < 0 ) {
    return 12;
  }
  write_bmp(filename, bricks, BRICKS_WIDTH, BRICKS_HEIGHT);
  write_bmp("output.bmp", bricks, BRICKS_WIDTH, BRICKS_HEIGHT);
  return dump_state(&state);
}
