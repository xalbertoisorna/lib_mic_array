// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#define PORT_MCLK_IN    VX_PORT_1D 
#define PORT_PDM_CLK    VX_PORT_1G 
#define PORT_PDM_DATA   VX_PORT_1F 

// -------------------- Frecuency and Port definitions --------------------
#define MIC_ARRAY_CONFIG_MCLK_FREQ  (3072000)           /* 3.072 MHz */
#define MIC_ARRAY_CONFIG_PDM_FREQ   (768000)            /* 768 KHz */
#define MIC_ARRAY_CONFIG_PORT_MCLK PORT_MCLK_IN         /* X0D11, J14 - Pin 15, '11' */
#define MIC_ARRAY_CONFIG_PORT_PDM_CLK PORT_PDM_CLK      /* X0D00, J14 - Pin 2, '00' */
#define MIC_ARRAY_CONFIG_PORT_PDM_DATA PORT_PDM_DATA    /* X0D14..X0D21 | J14 - Pin 3,5,12,14 and Pin 6,7,10,11 */
#define MIC_ARRAY_CONFIG_CLOCK_BLOCK_A XS1_CLKBLK_2

// ------------------------- App Definitions -----------------------------------
#define APP_N_SAMPLES (320)
#define APP_OUT_FREQ_HZ (12000) // 12KHz
#define APP_SAMPLE_SECONDS (5)
#define APP_N_FRAMES (APP_OUT_FREQ_HZ * APP_SAMPLE_SECONDS / APP_N_SAMPLES)
#define APP_BUFF_SIZE (APP_N_FRAMES * APP_N_SAMPLES)
#define APP_MIC_COUNT (MIC_ARRAY_CONFIG_MIC_COUNT)

// ------------------------- Power Mgmt Configurations --------------------------
#include "power.h"

#define CASE_A (0)
#define CASE_B (1)
#define CASE_C (2)
#define CASE_D (3)
#define CASE_E (4)

#define TEST_CASE CASE_D

#if TEST_CASE == CASE_A
    #define PLL0_OD     (GEN_DIV_0)
    #define XCORE1_DIV  (GEN_DIV_0)
    #define REF_DIV     (GEN_DIV_5)
#elif TEST_CASE == CASE_B
    #define PLL0_OD     (GEN_DIV_0)
    #define XCORE1_DIV  (GEN_DIV_2)
    #define REF_DIV     (GEN_DIV_5)
#elif TEST_CASE == CASE_C
    #define PLL0_OD     (GEN_DIV_1)
    #define XCORE1_DIV  (GEN_DIV_5)
    #define REF_DIV     (GEN_DIV_2)
#elif TEST_CASE == CASE_D
    #define PLL0_OD     (GEN_DIV_2)
    #define XCORE1_DIV  (GEN_DIV_9)
    #define REF_DIV     (19)
#else
    #error "Invalid TEST_CASE value"
#endif
