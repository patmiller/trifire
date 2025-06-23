#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "bricks.h"
#include "play.h"
#include "state.h"

// Update defaults in state.h
struct State state = DEFAULT_STATE;

int main(int argc, char** argv) {
  // Quit unless ./a.out <state> <command>
  if ( argc < 3 ) return 1;

  // We need the state number so we can try and read a state file
  long turn = strtol(argv[1],NULL,10);
  if (errno == EINVAL) return 2;

  // Special case to build our state-0.json file
  if (turn < 0) {
    return dump_state(&state);
  }

  // Open the file with the state in it
  char filename[80];
  if (snprintf(filename,80,"state-%ld.json",turn) < 0) return 3;
  FILE* fp = fopen(filename,"r");
  if ( fp == NULL ) return 4;
  if ( !read_state(fp,&state) ) return 8;

  // Play the game with the command.  I expect it to update
  // state (turn must advance by 1) AND for it to render the
  // scene into the 640x480 BMP I pass in
  if (!play(&state,bricks,argv[2][0])) return 9;
  if (state.turn != turn + 1) return 10;

  write_bmp("output.bmp", bricks, BRICKS_WIDTH, BRICKS_HEIGHT);
  return dump_state(&state);
}
