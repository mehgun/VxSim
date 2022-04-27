/* sysNvRamp.c - VxSim BSP NVRAM susbsystem support */

/*
 * Copyright (c) 2014, 2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This library contains BSP-specific routines for NVRAM subsystems,


SEE ALSO:
.pG "Configuration"

*/

/* includes */

#include <stdlib.h>
#include <sysLib.h>
#include <stdio.h>
#include <string.h>
#include <vxsimHostLib.h>
#include <prjParams.h>

/* externals */

/* defines */

#define NV_BOOT_OFFSET 0

/* globals */

#ifdef INCLUDE_NV_RAM
/***************************************************************************
*
* sysNvRamGet - get contents of non-volatile RAM
*
* This routine copies the contents of non-volatile memory into a specified
* string.  The string will be terminated with an EOS. For VxSim, the non
* volatile RAM support is based on a host file.
*
* RETURNS: OK, or ERROR if access is outside the non-volatile RAM range.
*
* SEE ALSO: sysNvRamSet()
*/

STATUS sysNvRamGet
    (
    char *	string,	/* where to copy non-volatile RAM    */
    int 	strLen,	/* maximum number of bytes to copy   */
    int 	offset	/* byte offset into non-volatile RAM */
    )
    {
    return ((STATUS) vxsimHostSysNvRamGet (string, strLen, offset +
    	NV_BOOT_OFFSET, NV_RAM_SIZE));
    }

/***************************************************************************
*
* sysNvRamSet - write to non-volatile RAM
*
* This routine copies a specified string into non-volatile RAM. For VxSim,
* the non-volatile RAM support is based on a host file.
*
* RETURNS: OK, or ERROR if access is outside the non-volatile RAM range.
*
* SEE ALSO: sysNvRamGet()
*/

STATUS sysNvRamSet
    (
    char *	string,	/* string to be copied into non-volatile RAM */
    int 	strLen,	/* maximum number of bytes to copy           */
    int 	offset	/* byte offset into non-volatile RAM         */
    )
    {
    return ((STATUS) vxsimHostSysNvRamSet (string, strLen, offset +
    	NV_BOOT_OFFSET, NV_RAM_SIZE));
    }
#endif /* INCLUDE_NV_RAM */
