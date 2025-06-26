// Copyright 2025 - Patrick Miller
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "play.h"
#include "state.h"

int first_difference(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] && s2[i]) {
        if (s1[i] != s2[i])
            return i;  // Return index of first difference
        i++;
    }
    // If one string is longer
    if (s1[i] != s2[i])
        return i;
    return -1;  // No difference found
}

int main(int argc, char** argv) {
  // Simple command structure
  if ( argc < 3 ) {
    fprintf(stderr, "%s <command> <json of state>\n", argv[0]);
    exit(1);
  }

  const char* command = argv[1];
  const char* json = argv[2];
  const char* expected = (argc > 3)?(argv[3]):(NULL);


  // Parse some state out of the json
  struct State state;
  if (!read_state_string(json, &state)) {
    fprintf(stderr, "Invalid state json: %s\n", json);
    return 1;
  }

  // See if we can play the game
  if (!play(&state, command[0])) {
    fprintf(stderr, "play failed\n");
    return 1;
  }

  // Build the new json
  char response[2048];
  if (!string_state(&state, response, sizeof(response))) {
    fprintf(stderr, "json packing failed\n");
    return 1;
  }

  // Are we in run mode?
  if ( !expected ) {
    puts(response);
    return 0;
  }

  // OK.. we must be in compare mode
  if ( strcmp(response, expected) == 0 ) return 0;

  fprintf(stderr, "miscompare\n%s\n%s\n", expected, response);
  int offset = first_difference(expected, response);
  for (int i = 0; i < offset; ++i) {
    fputc(' ', stderr);
  }
  fputs("^\n", stderr);
  return 1;
}
