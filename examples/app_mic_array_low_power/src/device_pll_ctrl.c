// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <assert.h>

#include <print.h>
#include <platform.h>
#include <vx4b_defines.h>
#include <xsystem/local_tile.h>
#include <xsystem/switch.h>
#include <xcore/hwtimer.h>

#include "device_pll_ctrl.h"


/*
[24.576 MHz conf]
    24 MHz ref
        |
    (R = 0) -> (24 / (R + 1)) -> 24 MHz
        |
    (F = 101) + FRAC -> 24 MHz * (F + 1 + 2/5 = 102.4) -> 2457.60 MHz  VCO
        | 
    (OD = 4) -> 2457.60 MHz / (OD + 1) / 2 -> 245.76 MHz 
        |
    (APP DIV = 4) ->  245.76 MHz / (4 + 1) = 245.76/5 MHz
        |
    (APP DIV2 = 2) -> 245.76/10 MHz = 24.576 MHz

[3.072 MHz conf]
    24 MHz ref
        |
    (R = 0) -> (24 / (R + 1)) -> 24 MHz
        |
    (F = 63) + FRAC -> 24 MHz * (63 + 1 + 0/0) = 24 * 64 -> 1536 MHz VCO
        | 
    (OD = 4) -> 1536 MHz / (OD + 1) / 2 = 1536 / 10 -> 153.60 MHz
        |
    (APP DIV = 24) ->  153.60 MHz / (24 + 1) = 6.144 MHz
        |
    (APP DIV2 = 2) -> 6.144 / 2 MHz = 3.072 MHz
*/


#define __RI    0
#define __FD    63
#define __OD    4
#define __AD    24
#define __FRAC  0
#define __FRACP 0
#define __FRACN 0


void device_pll_init(void)
{
    printf("Initializing PLL\n");
    xsystem_tile_id_t tileid = get_local_tile_id();

    // [0] PLL CTL DISABLE
    uint32_t DEVICE_PLL_DISABLE = 0x00000000;
    DEVICE_PLL_DISABLE = VX_PLL1_DISABLE_SET(DEVICE_PLL_DISABLE, 0);
    
    // [1] Mux 
    uint32_t DEVICE_PLL_MUX_VAL = 0x00000000;
    DEVICE_PLL_MUX_VAL = VX_APP_CLK1_MUX_BIT_SET(DEVICE_PLL_MUX_VAL, 1);
    DEVICE_PLL_MUX_VAL = VX_APP_CLK_IN_PHASE_BIT_SET(DEVICE_PLL_MUX_VAL, 1);

    // [2] PLL CTL
    uint32_t DEVICE_PLL_CTL_VAL = 0x00000000;
    DEVICE_PLL_CTL_VAL = VX_PLL1_R_DIVIDER_SET(DEVICE_PLL_CTL_VAL, __RI);      // input divider
    DEVICE_PLL_CTL_VAL = VX_PLL1_F_MULTIPLIER_SET(DEVICE_PLL_CTL_VAL, __FD); // feedback mult 
    DEVICE_PLL_CTL_VAL = VX_PLL1_OD_DIVIDER_SET(DEVICE_PLL_CTL_VAL, __OD);     // output divider 
    DEVICE_PLL_CTL_VAL = VX_PLL1_DISABLE_SET(DEVICE_PLL_CTL_VAL, 0);        // disable PLL before configuration
    DEVICE_PLL_CTL_VAL = VX_PLL1_BYPASS_SET(DEVICE_PLL_CTL_VAL, 0);         // no bypass
    DEVICE_PLL_CTL_VAL = VX_PLL1_NLOCK_SET(DEVICE_PLL_CTL_VAL, 1);          // wait for PLL lock

    // [3] FRAC (2/5)
    uint32_t DEVICE_PLL_FRAC_NOM = 0x00000000;
    uint32_t frac_enable = (__FRAC == 0) ? 0 : 1;
    DEVICE_PLL_FRAC_NOM = VX_SS_FRAC_N_ENABLE_SET(DEVICE_PLL_FRAC_NOM, frac_enable); // enable fractional mode
    DEVICE_PLL_FRAC_NOM = VX_SS_FRAC_N_PERIOD_CYC_CNT_SET(DEVICE_PLL_FRAC_NOM, __FRACP); // +1 -> 5
    DEVICE_PLL_FRAC_NOM = VX_SS_FRAC_N_F_HIGH_CYC_CNT_SET(DEVICE_PLL_FRAC_NOM, __FRACN); // +1 -> 2

    // [4] APP DIVIDER
    uint32_t DEVICE_PLL_DIV_0 = 0x00000000;
    DEVICE_PLL_DIV_0 = VX_APP_CLK_DIV_ENABLE_SET(DEVICE_PLL_DIV_0, 1);
    DEVICE_PLL_DIV_0 = VX_APP_CLK_DIV_VALUE_SET(DEVICE_PLL_DIV_0, __AD); // div by 5

    // CONFIGURE
    sswitch_reg_try_write(tileid, VX_SSB_CSR_PLL1_CTRL_NUM, DEVICE_PLL_DISABLE);            // disable PLL before configuration
    sswitch_reg_try_write(tileid, VX_SSB_CSR_CLK_SWITCH_CTRL_NUM, DEVICE_PLL_MUX_VAL);      // switch app clock to PLL1 output
    sswitch_reg_try_write(tileid, VX_SSB_CSR_PLL1_CTRL_NUM, DEVICE_PLL_CTL_VAL);            // configure PLL control register
    sswitch_reg_try_write(tileid, VX_SSB_CSR_PLL1_FRACN_CTRL_NUM, DEVICE_PLL_FRAC_NOM);     // configure PLL fractional control register
    sswitch_reg_try_write(tileid, VX_SSB_CSR_APP_CLK1_DIV_NUM, DEVICE_PLL_DIV_0);           // configure app clock divider
}
