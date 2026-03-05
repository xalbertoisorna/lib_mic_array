// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>

typedef struct {
    float    freq_mhz;
    float    energy_start;
    unsigned t_start;
} energy_ctx_t;

void energy_measure_init();
void energy_measure_begin(energy_ctx_t *ctx);
void energy_measure_end(energy_ctx_t *ctx);

void power_mgmt_setup();
