/* sysLib.c - Simulator-target system dependent library */

/*
 * Copyright (c) 2014, 2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This library provides Windows VxWorks simulator specific routines.

INCLUDE FILES: 

SEE ALSO:
\tb VxWorks Kernel Programmer's Guide .
*/

/* includes */

#include <vxWorks.h>
#include <arch/simnt/mmuArchLib.h>
#include <private/vxsimHostLibP.h>
#include <boardLib.h>

#include "sysPhysMemDesc.c"
#ifdef INCLUDE_IPNET
#include "sysNet.c"
#endif
#include "sysPlatform.c"
#include "sysNvRam.c"

LOCAL BOARD_DESC vxsim_windows = {
    BOARD_DESC_VER_DEF,         /* uVer */
    "vxsim_windows",              /* pCompat */
    0,                          /* uFlags */
    sysPlatformProbe,         /* probe */
    &sysPlatformFuncTbl       /* pFuncTbl */
    };

BOARD_DEF(vxsim_windows)
