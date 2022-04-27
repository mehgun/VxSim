# Makefile - makefile for vxsim/linux
#
# Copyright (c) 2014, 2019, 2021 Wind River Systems, Inc.
#
# The right to copy, distribute, modify, or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
CPU     = SIMNT
TOOL    = llvm

# Make from the BSP folder is not supported

BYPASS_BSP_MAKE = 1

include $(WIND_KRNL_MK)/defs.bsp.mk

# Only redefine make definitions below this point, or your definitions will
# be overwritten by the makefile stubs above.

TARGET_DIR = vxsim_windows
VENDOR     = WindRiver
BOARD      = windows

# Only redefine make definitions above this point, or the expansion of
# makefile target dependencies may be incorrect.

include $(WIND_KRNL_MK)/rules.bsp.mk
