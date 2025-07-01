#include <stdio.h>
extern int playlong(
	     char command,
	     long* turn,
	     long* points,
	     long* tri_x,
	     long* rotation,
	     long* coin_x,
	     long* coin_y,
	     long* cannon_t,
	     long* cannon_offset);

int main() {
  long turn = 0;
  long points = 0;
  long tri_x = 0;
  long rotation = 0;
  long coin_x = 0;
  long coin_y = 0;
  long cannon_t = 0;
  long cannon_offset = 0;
  int status = playlong('r', 
			&turn,
			&points,
			&tri_x,
			&rotation,
			&coin_x,
			&coin_y,
			&cannon_t,
			&cannon_offset);
  printf("{\"turn\": %ld, "
	 "\"points\": %ld,"
	 "\"tri_x\": %ld,"
	 "\"rotation\": %ld,"
	 "\"coin_x\": %ld,"
	 "\"coin_y\": %ld,"
	 "\"cannon_t\": %ld,"
	 "\"cannon_offset\": %ld}\n",
	 turn,
	 points,
	 tri_x,
	 rotation,
	 coin_x,
	 coin_y,
	 cannon_t,
	 cannon_offset);
  return status;
}
