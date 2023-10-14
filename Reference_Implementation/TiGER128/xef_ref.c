/*
 * Original code is
 * Copyright (c) 2018, PQShield
 * Markku-Juhani O. Saarinen

 * TiGER Team modified the original code by optimizing it to only work on XE3-91.
 */


//  Endian-neutral parametrized reference implementation of the XEf codes.

#include <stdint.h>
#include <stddef.h>
#include <string.h>

const unsigned xef_reg[6] = { 11, 13, 15, 16, 17, 19 }; // XE3-91

size_t xef_compute(void *block, size_t len, unsigned f)
{
    if (f != 3 || len != 16) {
        return len;
    }

    uint8_t *v = (uint8_t *) block;
    size_t i, bit;
    uint64_t x, r[6] = {0};

    // reduce the polynomials
    for (i = 0, bit = 0; i < len; i++, bit += 8) {
        x = (uint64_t) v[i];
        for (int j = 0; j < 6; j++) {
            r[j] ^= x << (bit % xef_reg[j]);
        }
    }

    // pack the result (or rather, XOR over the original)
    for (i = 0; i < 6; i++) {
        x = r[i] ^ (r[i] >> xef_reg[i]);
        for (size_t j = 0; j < xef_reg[i]; j++, bit++) {
            v[bit >> 3] ^= ((x >> j) & 1) << (bit & 7);
        }
    }

    return bit;
}

size_t xef_fixerr(void *block, size_t len, unsigned f)
{
    if (f != 3 || len != 16) {
        return len;
    }

    uint8_t *v = (uint8_t *) block;
    size_t i, j, bit;
    uint64_t r[6] = {0};

    // unpack the registers
    for (bit = len << 3, i = 0; i < 6; i++) {
        for (j = 0; j < xef_reg[i]; j++, bit++) {
            r[i] ^= ((uint64_t) ((v[bit >> 3] >> (bit & 7)) & 1)) << j;
        }
    }

    // fix errors
    for (i = 0; i < (len << 3); i++) {
        unsigned th = 4; // 7 - 3
        for (j = 0; j < 6; j++) {
            th += (r[j] >> (i % xef_reg[j])) & 1;
        }
        v[i >> 3] ^= (th >> 3) << (i & 7);
    }

    // return the true length
    return bit;
}
