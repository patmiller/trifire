#pragma once
#include <stdint.h>

void write_bmp(const char* filename, uint32_t* rgb_array, int width, int height);
