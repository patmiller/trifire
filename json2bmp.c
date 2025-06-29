// Copyright 2025 - Patrick Miller
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "bricks.h"
#include "play.h"
#include "render.h"
#include "state.h"

int main(int argc, char** argv) {
  struct State state = DEFAULT_STATE;

  // Simple command structure.  a.out command json bmpname
  if ( argc < 4 ) {
    fprintf(stderr, "%s: command json bmpname\n", argv[0]);
    return 1;
  }

  const char* command = argv[1];
  const char* json = argv[2];
  const char* bmpfile = argv[3];

  // Parse some state out of the json
  if (json[0]) {
    if (!read_state_string(json, &state)) {
      fprintf(stderr, "Invalid state json: %s\n", json);
      return 1;
    }
  }

  // See if we can play the game
  for (const char* cp = command; *cp; ++cp) {
    if (!play(&state, *cp)) {
      fprintf(stderr, "play failed\n");
      return 1;
    }
  }

  // Overwrite our brick (we will not use it again, so it is ok)
  if (!render(bricks, BRICKS_WIDTH, BRICKS_HEIGHT, &state)) {
    fprintf(stderr, "render failed\n");
    return 1;
  }

  // Write it out
  write_bmp(bmpfile, bricks, BRICKS_WIDTH, BRICKS_HEIGHT);
  return 0;
}
