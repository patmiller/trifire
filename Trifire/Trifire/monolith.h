extern int playlong(char command, long* state);
extern int renderlong(int* area,
	       unsigned area_width, unsigned area_height,
	       long* state_array);
extern int ncoins();
extern int setcoin(int c, long* state_array);
extern int findtraj(int x, int y);
