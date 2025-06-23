#include "render.h"

#include <stdint.h>

int render(uint32_t* area,
	   unsigned area_width, unsigned area_height,
	   uint32_t* sprite,
	   unsigned sprite_width, unsigned sprite_height,
	   unsigned x, unsigned y
	   ) {
  for(int j=0;j<sprite_height;++j) {
    for(int i=0;i<sprite_width;++i) {
      unsigned pixel = sprite[i + j*sprite_width];
      unsigned xx = x + i;
      unsigned yy = y + j;
      if ( xx > area_width ) return 0;
      if ( yy > area_height ) return 0;
      area[xx + yy*area_width] = pixel;
    }
  }
  return 1;
}
