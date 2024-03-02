#include "hwt.h"
#include <stdio.h>
#include <string.h>

void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input, const size_t input_size, const uint16_t hmwt) {
    uint32_t pos = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    uint8_t xof_block = (hmwt == HS) ? HS_XOF : HR_XOF;
    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, xof_block, &state);

    uint32_t dimension_minus_hmwt = DIMENSION - hmwt;
    uint32_t total_iterations = xof_block * 32;
    uint32_t max_degree = 0xffffffff;

    for (size_t i = 0; i < total_iterations; i++) {
        uint32_t deg = buf[i];
        uint32_t threshold = max_degree - (max_degree / (dimension_minus_hmwt + pos) * (dimension_minus_hmwt + pos));
        
        if (deg < threshold && pos < hmwt) {
            res[pos++] = 1; // Set the bit to indicate presence in the sparse polynomial
        }
    }

    // Initialize cnt_arr to 0
    memset(cnt_arr, 0, sizeof(uint8_t) * (DIMENSION / 8));

    // Calculate the Hamming weight for each segment of the polynomial
    for (size_t i = 0; i < DIMENSION; ++i) {
        if (res[i]) {
            size_t cnt_arr_idx = i / 8;
            cnt_arr[cnt_arr_idx]++;
        }
    }
}

