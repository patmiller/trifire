#pragma once
#include <stdint.h>
#include "state.h"

extern void SHA1(uint8_t digest[20], void* message, size_t n);
