#include <stdio.h>
#include "monolith.h"

int main() {
  long state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int status = playlong('r', state);
  printf("{\"turn\": %ld, "
	 "\"points\": %ld,"
	 "\"tri_x\": %ld,"
	 "\"rotation\": %ld,"
	 "\"coin_x\": %ld,"
	 "\"coin_y\": %ld,"
	 "\"cannon_t\": %ld,"
	 "\"cannon_offset\": %ld}\n",
	 state[0],
	 state[1],
	 state[2],
	 state[3],
	 state[4],
	 state[5],
	 state[6],
	 state[7]);
  // 1==OK 2==Explosion
  return !status;
}
