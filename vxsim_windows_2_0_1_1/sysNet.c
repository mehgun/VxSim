/* sysNet.c - VxSim BSP network subsystem support */

/*
 * Copyright (c) 2014-2016, 2018, 2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This library contains BSP-specific routines for network subsystems,
including network interface card device driver configuration and
support routines.

Note that the routines implemented in this file will not necessarily
be included in future versions of Wind River Systems' VxSim for Windows
BSP.

SEE ALSO:
.pG "Configuration"

*/

/* includes */

#include <end.h>
#include <stdlib.h>
#include <muxLib.h>
#include <ipProto.h>
#include <sysLib.h>
#include <net/utils/routeCmd.h>
#include <net/utils/ifconfig.h>
#include <stdio.h>
#include <string.h>

#include <vxsimHostLib.h>

/* externals */

/* defines */

#define	MAX_CMD_LENGTH	300	/* max ifconfig command string length */

/* globals */

/******************************************************************************
*
* sysNetIfConfig - configure IP address for additional interfaces
*
* RETURNS: N/A
*/

void sysNetIfConfig ()
    {
#ifdef INCLUDE_IPNET
    int		addIx = 0;		/* number of additional interfaces */
    char *	device;			/* device name */
    int		unit;			/* unit number */
    char *	devStr;			/* additional device parameter string */
    char *	ipAddr;			/* ip address */
    char *	ipMask;			/* ip mask */
    struct in_addr netmask;		/* network mask */
    char	cmd [MAX_CMD_LENGTH];	/* ifconfig command string */
    size_t	cmdLen;
#ifdef _WRS_CONFIG_SMP
    /*
     * Temporary workaround (VXWRAD-253): vxsimHostNetAddIfGet() only returns
     * valid addresses for CPU 0. So temporarily set our affinity to CPU 0.
     */
    cpuset_t    orig_aff;
    cpuset_t    aff;

    (void)taskCpuAffinityGet (0, &orig_aff);

    CPUSET_ZERO (aff);
    CPUSET_SET (aff, 0);

    (void)taskCpuAffinitySet (0, aff);
#endif

    /* Configure all additional network interfaces if requested */

    while (vxsimHostNetAddIfGet (addIx, &device, &unit, &devStr, &ipAddr,
			      &ipMask) == OK)
	{
	addIx ++;

	if (ipAttach (unit, device) == ERROR)
	    {
	    (void)vxsimHostPrintf ("Failed to attach to device %s%d",
                                   device, unit);
	    continue;
	    }

	if (ipAddr != NULL && (strlen(ipAddr) != 0))
	    {
	    /* Configure network interface */

	    snprintf (cmd, MAX_CMD_LENGTH, "%s%d %s", device, unit, ipAddr);
	    cmdLen = strlen(cmd);

	    if (ipMask != NULL)
		{
		netmask.s_addr = (in_addr_t) strtoul (ipMask, NULL, 16);

		if (netmask.s_addr != 0)
		    {
		    netmask.s_addr = htonl(netmask.s_addr);
		    if (cmdLen < (MAX_CMD_LENGTH - strlen (" netmask ") - 16))
		    	{
			strncat (cmd, " netmask ", MAX_CMD_LENGTH - cmdLen);
			inet_ntoa_b (netmask, cmd + strlen (cmd));
			cmd [MAX_CMD_LENGTH - 1] = EOS;
			cmdLen = strlen (cmd);
			}
		    }
		}
	    strncat (cmd, " up", MAX_CMD_LENGTH - cmdLen);
	    cmd [MAX_CMD_LENGTH - 1] = EOS;

	    if (ifconfig (cmd) != OK)
	    	{
	    	(void)vxsimHostPrintf ("Error configuring network " \
					"interface %s%d\n", device, unit);
		}
	    }

	}

#ifdef _WRS_CONFIG_SMP
    /* Temporary workaround (VXWRAD-253) */
    (void)taskCpuAffinitySet (0, orig_aff);
#endif

#endif /* INCLUDE_IPNET */
    }
