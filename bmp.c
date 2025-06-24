// Copyright 2025 - Patrick Miller

#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)  // Ensure no padding in structs

typedef struct {
    uint16_t bfType;      // "BM"
    uint32_t bfSize;      // File size in bytes
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;   // Offset to pixel data
} BMPFileHeader;

typedef struct {
    uint32_t biSize;         // Size of this header
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;     // Bits per pixel
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;

#pragma pack(pop)

// Small helper to predict BMP file size from width/height
unsigned bmp_size(int width, int height) {
    // each row must be multiple of 4 bytes
    int row_padded = (3 * width + 3) & ~3;
    int image_size = row_padded * height;
    int file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + image_size;

    return file_size;
}

// Take the RGB pixels in the array and pack into a BGR BMP formatted blob
void pack_bmp(uint32_t* rgb_array, int width, int height, uint8_t* bmp) {
    // each row must be multiple of 4 bytes
    int row_padded = (3 * width + 3) & ~3;
    int image_size = row_padded * height;
    int file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + image_size;

    // File header with magic number BM
    BMPFileHeader file_header = {
        .bfType = 0x4D42,  // 'BM'
        .bfSize = file_size,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader),
    };

    // Tells us about the form of this BMP block
    BMPInfoHeader info_header = {
        .biSize = sizeof(BMPInfoHeader),
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 24,  // 24-bit bitmap
        .biCompression = 0,
        .biSizeImage = image_size,
        .biXPelsPerMeter = 2835,
        .biYPelsPerMeter = 2835,
        .biClrUsed = 0,
        .biClrImportant = 0,
    };

    // "write" the header and info blocks
    unsigned offset = 0;
    memcpy(bmp+offset, &file_header, sizeof(file_header));
    offset += sizeof(file_header);

    memcpy(bmp+offset, &info_header, sizeof(info_header));
    offset += sizeof(info_header);

    // Write pixel data (bottom-up BMP format)
    uint8_t* row = bmp+offset;
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            uint32_t rgb = rgb_array[y * width + x];
            row[x * 3 + 0] = rgb & 0xFF;          // Blue
            row[x * 3 + 1] = (rgb >> 8) & 0xFF;   // Green
            row[x * 3 + 2] = (rgb >> 16) & 0xFF;  // Red
        }
        // Pad the row
        for (int i = width * 3; i < row_padded; i++) {
            row[i] = 0;
        }
        row += row_padded;
    }
}

// This allocates a buffer, packs it with BMP data, writes it and then
// frees it.
void write_bmp(const char* filename, uint32_t* rgb_array,
               int width, int height) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("File open failed");
        return;
    }

    unsigned size = bmp_size(width, height);
    uint8_t* bmp = malloc(size);
    pack_bmp(rgb_array, width, height, bmp);
    fwrite(bmp, size, 1, f);
    free(bmp);

    fclose(f);
}
