cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(app_mic_array)

set(XMOS_SANDBOX_DIR    ${CMAKE_CURRENT_LIST_DIR}/../../..)

# conditional depending on target
set(APP_HW_TARGET   XK-EVK-XU416)
set(APP_INCLUDES    src vx4)
set(APP_DEPENDENT_MODULES 
    "lib_mic_array" 
    "github0.xmos.com:xmos-int/lib_ina4235(develop)" 
    "github0.xmos.com:xmos-int/lib_power_mgmt(develop)"
)


set(APP_COMPILER_FLAGS
    -Os
    -g
    -report
    -Wall
    -fxscope
    -DLIBXCORE_XASSERT_IS_ASSERT
    # Mic array config
    -DMIC_ARRAY_CONFIG_SAMPLES_PER_FRAME=320
    -DMIC_ARRAY_CONFIG_MIC_COUNT=1
    -DMIC_ARRAY_CONFIG_USE_PDM_ISR=0
    -DMIC_ARRAY_CONFIG_USE_DC_ELIMINATION=1
)

XMOS_REGISTER_APP()
