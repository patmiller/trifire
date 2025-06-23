#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "state.h"

// We're assuming very scripted JSON here
// {
// "turn": 0,
// "coin_x": 0,
// "rotation": 0,
// "tri_x": 0
// }
int read_state(FILE* fp, struct State* s) {
  char line[80];
  // Read a {
  if ( fgets(line,80,fp) == NULL ) return 0;
  if (strncmp(line,"{\n",80) != 0) return 0;
  
  // Read "turn": 0,
  if ( fgets(line,80,fp) == NULL ) return 0;
  if ( sscanf(line,"\"turn\": %lu,",&(s->turn)) != 1 ) return 0;

  // Read "coin_x": 0,
  if ( fgets(line,80,fp) == NULL ) return 0;
  if ( sscanf(line,"\"coin_x\": %u,",&(s->coin_x)) != 1 ) return 0;

  // Read "rotation": 0,
  if ( fgets(line,80,fp) == NULL ) return 0;
  if ( sscanf(line,"\"rotation\": %u,",&(s->rotation)) != 1 ) return 0;
  if ( s->rotation > 2 ) return 0;

  // Read "cannon": 0,
  if ( fgets(line,80,fp) == NULL ) return 0;
  if ( sscanf(line,"\"cannon\": %u,",&(s->cannon)) != 1 ) return 0;
  if ( s->cannon > 2 ) return 0;

  // Read "tri_x": 0 (no comma)
  if ( fgets(line,80,fp) == NULL ) return 0;
  if ( sscanf(line,"\"tri_x\": %u",&(s->tri_x)) != 1 ) return 0;

  // Read a }
  if ( fgets(line,80,fp) == NULL ) return 0;
  if (strncmp(line,"}\n",80) != 0) return 0;

  return 1;
}

int dump_state(struct State* s) {
  char filename[80];
  if (snprintf(filename,80,"state-%ld.json",s->turn) < 0) return 5;
  FILE* fp = fopen(filename,"w");
  if (!fp) return 6;

  if ( fputs("{\n",fp) == EOF ) return 7;
  if ( fprintf(fp,"\"turn\": %lu\n",s->turn) == EOF ) return 7;
  if ( fprintf(fp,"\"coin_x\": %u\n",s->coin_x) == EOF ) return 7;
  if ( fprintf(fp,"\"rotation\": %u\n",s->rotation) == EOF ) return 7;
  if ( fprintf(fp,"\"cannon\": %u\n",s->cannon) == EOF ) return 7;
  if ( fprintf(fp,"\"tri_x\": %u\n",s->tri_x) == EOF ) return 7;
  if ( fputs("}\n",fp) == EOF ) return 7;
  return 0;
}
