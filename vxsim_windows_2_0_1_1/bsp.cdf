/* bsp.cdf - Board definition description file */

/*
 * Copyright (c) 2014-2015, 2018, 2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Bsp windows {
    CPU             SIMNT
    DATA_MODEL      LP64_ONLY
    MP_OPTIONS      UP SMP
    VIP_BOOT        REQUIRED
    BUILD_SPECS     default
    BUILD_TARGETS   vxWorks
    FP              hard
    REQUIRES        INCLUDE_KERNEL  \
                    INCLUDE_BOARD_LIB_INIT  \
                    DRV_BUS_FDT_SIMPLE \
                    DRV_INT_VXSIM \
                    DRV_TIMER_VXSIM \
                    BOARD_SELECTION
}
