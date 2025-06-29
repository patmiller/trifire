// Copyright 2025 Patrick Miller
#pragma once
#include <stdint.h>

extern unsigned bmp_size(int width, int height);
extern void pack_bmp(uint32_t* rgb_array, int width, int height, uint8_t* bmp);
extern void write_bmp(const char* filename,
                      uint32_t* rgb_array, int width, int height);
