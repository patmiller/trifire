#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1) // Ensure no padding

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

void write_bmp(const char* filename, uint32_t* rgb_array, int width, int height) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("File open failed");
        return;
    }

    int row_padded = (3 * width + 3) & ~3;  // each row must be multiple of 4 bytes
    int image_size = row_padded * height;
    int file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + image_size;

    BMPFileHeader file_header = {
        .bfType = 0x4D42,  // 'BM'
        .bfSize = file_size,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader),
    };

    BMPInfoHeader info_header = {
        .biSize = sizeof(BMPInfoHeader),
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 24, // 24-bit bitmap
        .biCompression = 0,
        .biSizeImage = image_size,
        .biXPelsPerMeter = 2835,
        .biYPelsPerMeter = 2835,
        .biClrUsed = 0,
        .biClrImportant = 0,
    };

    fwrite(&file_header, sizeof(file_header), 1, f);
    fwrite(&info_header, sizeof(info_header), 1, f);

    // Write pixel data (bottom-up BMP format)
    uint8_t* row = malloc(row_padded);
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            uint32_t rgb = rgb_array[y * width + x];
            row[x * 3 + 0] = rgb & 0xFF;        // Blue
            row[x * 3 + 1] = (rgb >> 8) & 0xFF; // Green
            row[x * 3 + 2] = (rgb >> 16) & 0xFF;// Red
        }
        // Pad the row
        for (int i = width * 3; i < row_padded; i++) {
            row[i] = 0;
        }
        fwrite(row, row_padded, 1, f);
    }

    free(row);
    fclose(f);
}
