// Copyright 2025 - Patrick Miller

#include "state.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// We're assuming very scripted JSON here where each item
// is a signed long int, e.g.
// {
// "turn": 0,
// "coin_x": 0,
// "rotation": 0,
// "tri_x": 0
// }

// So, we just have keys that we look for in the form "foo", not foo
// and a pointer to a field in the struct where we want to put the long
struct keyvalue {
  const char* key;
  long* value;
};

// Here, we just initialize the key:value pairs for use in the reader
// and writer to keep them consistent
static void get_state_pointers(struct keyvalue fields[NSTATE_FIELDS+1],
                               struct State* s) {
  // Order doesn't much matter here, but we must have a correct
  // NSTATE_FIELDS count and the internal address must match
  fields[0].key = "\"turn\"";     fields[0].value = &(s->turn);
  fields[1].key = "\"rotation\""; fields[1].value = &(s->rotation);
  fields[2].key = "\"cannon\"";   fields[2].value = &(s->cannon);
  fields[3].key = "\"cannon_t\"";   fields[3].value = &(s->cannon_t);
  fields[4].key = "\"cannon_offset\"";   fields[4].value = &(s->cannon_offset);
  fields[5].key = "\"tri_x\"";    fields[5].value = &(s->tri_x);
  fields[6].key = "\"coin_x\"";   fields[6].value = &(s->coin_x);
  fields[7].key = "\"coin_y\"";   fields[7].value = &(s->coin_y);
  fields[8].key = NULL;           fields[8].value = NULL;
}

// Take a null-terminated string buffer (json dict as a string)
// and read the state fields. Any undefined field is set to the
// default
int read_state_string(const char* buffer, struct State* s) {
  struct State DEFAULT = DEFAULT_STATE;
  *s = DEFAULT;

  // Here are the fields we are looking for
  struct keyvalue fields[NSTATE_FIELDS+1];
  get_state_pointers(fields, s);

  // For each one, we look for, say "foo" in the string
  // (note the use of "foo" not just foo).  Then we look
  // for a colon and long int after the key
  for (struct keyvalue* p = fields; p->key; ++p) {
    const char* offset = strstr(buffer, p->key);
    if (!offset) continue;
    if ( sscanf(offset+strlen(p->key), ": %ld", p->value) < 0 ) return 0;
  }

  return 1;
}

// This just pulls the defined fields out of a state variable
// and converts it to string (in a sized buffer)
char* string_state(struct State* s, char* buffer, unsigned n) {
  // Opening {
  if (n < 2) return NULL;
  strcpy(buffer, "{");
  buffer += 1;
  n -= 1;

  // Here are the fields we are looking for
  struct keyvalue fields[NSTATE_FIELDS+1];
  get_state_pointers(fields, s);

  // For each key, dump in the form (with , and \n as appropriate)
  // "foo": 12345
  // Note: the terminating null character is added with the final }
  // below
  for (struct keyvalue* p = fields; p->key; ++p) {
    // We are doing this line at a time, but watch for overflow
    // of the line buffer and the overall buffer
    char line[80];
    unsigned n_line = snprintf(line, sizeof(line),
                               "%s: %ld", p->key, *p->value);
    if (n_line >= sizeof(line)) return NULL;
    if (n_line >= n) return NULL;
    strcpy(buffer, line);
    buffer += n_line;

    n -= n_line;
    if ( n < 2 ) return NULL;
    if ((p+1)->key) {
      buffer[0] = ',';
      buffer[1] = ' ';
      buffer += 2;
      n -= 2;
    }
  }

  // Closing }
  if (n < 2) return NULL;  // Remember the null terminator!
  strcpy(buffer, "}");
  buffer += 2;
  n -= 2;

  return buffer;
}

// This just dumps the json to a file called state-<turn>.json
int dump_state(struct State* s) {
  char filename[80];
  if (snprintf(filename, 80, "state-%ld.json", s->turn) < 0) return 5;
  FILE* fp = fopen(filename, "w");
  if (!fp) return 6;

  // Error 7 likely means that you need a bigger buffer here
  char buffer[1024];
  if (!string_state(s, buffer, sizeof(buffer))) return 7;
  if ( fputs(buffer, fp) == EOF ) return 7;
  return 0;
}

// Use a "big enough" buffer and read a block of data, then parse
// as a state json.
int read_state(FILE* fp, struct State* s) {
  // Make a big buffer, we'll detect if we don't have enough space
  char buffer[1024];
  int n = fread(buffer, 1, sizeof(buffer), fp);
  if ( n <= 0 || n >= 1023 ) return 0;

  // Then parse it
  if ( !read_state_string(buffer, s) ) return 0;

  return 1;
}
