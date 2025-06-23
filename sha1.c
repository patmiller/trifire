//  portable, public-domain C implementation of SHA-1, based on FIPS PUB 180-1

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SHA-1 context
typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    uint8_t buffer[64];
} SHA1_CTX;

// Helper macros
#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32-(b))))

static void SHA1Transform(uint32_t state[5], const uint8_t buffer[64]) {
    uint32_t a, b, c, d, e, i, j, t, m[80];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (buffer[j] << 24) | (buffer[j+1] << 16) | (buffer[j+2] << 8) | (buffer[j+3]);
    for ( ; i < 80; ++i)
        m[i] = ROTLEFT(m[i-3] ^ m[i-8] ^ m[i-14] ^ m[i-16], 1);

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    for(i = 0; i < 80; ++i) {
        if (i < 20)
            t = ROTLEFT(a,5) + ((b & c) | ((~b) & d)) + e + m[i] + 0x5A827999;
        else if (i < 40)
            t = ROTLEFT(a,5) + (b ^ c ^ d) + e + m[i] + 0x6ED9EBA1;
        else if (i < 60)
            t = ROTLEFT(a,5) + ((b & c) | (b & d) | (c & d)) + e + m[i] + 0x8F1BBCDC;
        else
            t = ROTLEFT(a,5) + (b ^ c ^ d) + e + m[i] + 0xCA62C1D6;

        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

static void SHA1Init(SHA1_CTX* ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->count[0] = ctx->count[1] = 0;
}

static void SHA1Update(SHA1_CTX* ctx, const uint8_t* data, size_t len) {
    size_t i = 0, index = (ctx->count[0] >> 3) & 0x3F;
    ctx->count[0] += len << 3;
    if (ctx->count[0] < (len << 3)) ctx->count[1]++;
    ctx->count[1] += len >> 29;

    size_t partLen = 64 - index;
    if (len >= partLen) {
        memcpy(&ctx->buffer[index], &data[0], partLen);
        SHA1Transform(ctx->state, ctx->buffer);
        for (i = partLen; i + 63 < len; i += 64)
            SHA1Transform(ctx->state, &data[i]);
        index = 0;
    } else {
        i = 0;
    }
    memcpy(&ctx->buffer[index], &data[i], len - i);
}

static void SHA1Final(uint8_t digest[20], SHA1_CTX* ctx) {
    uint8_t bits[8];
    uint8_t pad[64] = { 0x80 };
    uint32_t i;

    for (i = 0; i < 8; ++i)
        bits[i] = (uint8_t)((ctx->count[(i >= 4)] >> ((3 - (i & 3)) * 8)) & 0xFF);

    size_t index = (ctx->count[0] >> 3) & 0x3f;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);
    SHA1Update(ctx, pad, padLen);
    SHA1Update(ctx, bits, 8);

    for (i = 0; i < 20; ++i)
        digest[i] = (uint8_t)((ctx->state[i>>2] >> ((3 - (i & 3)) * 8)) & 0xFF);
}

void SHA1(uint8_t digest[20], void* message, size_t n) {
  SHA1_CTX ctx;
  SHA1Init(&ctx);
  SHA1Update(&ctx, (const uint8_t*)message, n);
  SHA1Final(digest, &ctx);
}
