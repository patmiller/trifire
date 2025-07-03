// Copyright 2025 - Patrick Miller
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "bricks.h"
#include "render.h"
#include "ball.h"
#include "trajectory.h"

int main(int argc, char** argv) {
  struct State state = DEFAULT_STATE;

  // Simple command structure.  a.out command json bmpname
  if ( argc < 4 ) {
    fprintf(stderr, "%s: firstrajectory lasttrajectory bmpname\n", argv[0]);
    return 1;
  }

  int first = atoi(argv[1]);
  int last = atoi(argv[2]);
  const char* bmpfile = argv[3];

  // If 0, all trajectories
  // Otherwise just the requested trajectory
  for (int cannon_t = first; cannon_t <= last; ++cannon_t) {
    printf("-- traj %d\n",cannon_t);
    if (cannon_t > 0 && cannon_t < NTRAJECTORIES) {
      // Overwrite our brick (we will not use it again, so it is ok)
      for (int i=1; trajectories[cannon_t][i].x != 65535; ++i) {
	if ( trajectories[cannon_t][i].x < 10) {
	  printf("%d %d\n",
		 trajectories[cannon_t][i].x,
		 trajectories[cannon_t][i].y);
	}
        sprite(bricks, BRICKS_WIDTH, BRICKS_HEIGHT,
               ball, BALL_WIDTH, BALL_HEIGHT,
               trajectories[cannon_t][i].x,
               trajectories[cannon_t][i].y);
      }
    }
  }

  // Write it out
  puts(bmpfile);
  write_bmp(bmpfile, bricks, BRICKS_WIDTH, BRICKS_HEIGHT);
  return 0;
}
