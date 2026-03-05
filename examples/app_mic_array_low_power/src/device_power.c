// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>

#include <xcore/hwtimer.h>
#include <xcore/parallel.h>
#include <vx4b_defines.h>
#include <xsystem/local_tile.h>
#include <xsystem/switch.h>

#include "power.h"
#include "sensor_ina4235.h"
#include "device_power.h"
#include "app_config.h"

static inline
float ticks_to_ms(unsigned ticks, float freq_mhz)
{
    /* freq_mhz * 1e3 = ticks per millisecond */
    return (float)ticks / (freq_mhz * 1e3f);
}

static inline
float __energy_read()
{
    sensor_ina4235_sample_t sample;
    sensor_ina4235_read(&sample);
    return sample.energy;
}

void energy_measure_init(){
    sensor_ina4235_init();
    sensor_ina4235_set_channel(VDD);
}

void energy_measure_begin(energy_ctx_t *ctx)
{
    ctx->energy_start = __energy_read();  /* J */
    ctx->t_start      = get_reference_time(); /* ticks */
    printf("Energy start:\t%f J\n", ctx->energy_start);
}

void energy_measure_end(energy_ctx_t *ctx)
{
    float    en       = __energy_read();      /* J */
    unsigned t_end    = get_reference_time();   /* ticks */

    float elapsed_ms  = ticks_to_ms(t_end - ctx->t_start, ctx->freq_mhz); /* ms  */
    float elapsed_s   = elapsed_ms / 1e3f;                                 /* s   */
    float energy_diff = en - ctx->energy_start;                            /* J   */
    float mean_power  = (energy_diff / elapsed_s) * 1e3f;                  /* J/s = W, * 1e3 = mW */

    //printf("Energy end:\t%f J\n",    en);
    printf("Energy diff:\t%f J\n",   energy_diff);
    printf("Elapsed time:\t%f ms\n", elapsed_ms);
    printf("Mean power:\t%f mW\n",   mean_power);
}

static void setup_XTAL_osc(void) {
  power_clock_rc_osc_disable(); // Disable RC oscillator
  power_clock_xtal_enable(); // Enable XTAL oscillator
  power_clock_mux_ref_clk(ref_clk_xtal); // Switch to XTAL oscillator
  power_clock_pll0_disable(); // Disable PLL0 to allow changing its reference clock
}

void power_mgmt_setup()
{
    tile_ctrl_t ctrl = {
        .extmem_enable = false,
        .usb_enable = false,
        .mipi_enable = false,
    };
    power_clock_rc_osc_disable(); 
    power_sswitch_set_frecuency(GEN_DIV_7);
    power_tile_configure(&ctrl, true);
    power_tile_set_mode(tile0, tile_off);
    power_ram_set_mode(sram0, ram_shutdown);
    power_ram_set_mode(soc_ram0, ram_shutdown);
    power_ram_set_mode(soc_ram1, ram_shutdown);
    // tile
    power_clock_pll0_set_OD(PLL0_OD);
    power_tile_set_divider(tile1, XCORE1_DIV);
    power_clock_ref_set_divider(REF_DIV);
    return;
}
