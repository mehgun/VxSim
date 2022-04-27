/* sysPlatform.c - vxsim platform included by sysLib.c */

/*
 * Copyright (c) 2014-2017, 2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This library provides linux VxWorks simulator specific routines.

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Kernel Programmer's Guide .
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#if CPU==SIMLINUX
#include <arch/simlinux/mmuArchLib.h>
#ifdef _WRS_CONFIG_SMP
#include <arch/simlinux/vxCpuSimlinuxLib.h>
#endif
#elif CPU==SIMNT
#include <arch/simnt/mmuArchLib.h>
#ifdef _WRS_CONFIG_SMP
#include <arch/simnt/vxCpuSimntLib.h>
#endif
#endif
#include <private/vxsimHostLibP.h>
#include <prjParams.h>
#include <private/sigLibP.h>
#include <private/aimCpuLibP.h>
#include <cpuPwrArchLib.h>
#include <private/cpuPwrLibP.h>
#include <private/kwriteLibP.h>     /* _func_kwrite */
#ifdef INCLUDE_POSIX_CLOCKS
#include <clockLib.h>
#include <xtime.h>
#endif /* INCLUDE_POSIX_CLOCKS */

/* defines */

#ifdef SYS_PLATFORM_DEBUG
#define SYS_PLATFORM_MSG(fmt, args...)        \
    do    {                                     \
        (void)vxsimHostPrintf (fmt, ##args);          \
    } while (FALSE)
#else
#define SYS_PLATFORM_MSG(fmt, args...)
#endif

/* forward declarations */

LOCAL BOOL sysPlatformProbe (char * compat);
LOCAL void sysPlatformEarlyInit ();
LOCAL void sysPlatformInit (void);
LOCAL void sysPlatformReset (int startType);
LOCAL char * sysPlatformModel (void);
LOCAL void sysPlatformUsDelay (int delay);
#ifdef _WRS_CONFIG_SMP
LOCAL UINT32 sysPlatformCpuAvailGet (void);
LOCAL STATUS sysPlatformCpuEnable (unsigned int physCpuIndex,
                                   WIND_CPU_STATE * pCpuState);
#endif /* _WRS_CONFIG_SMP */


#ifdef _WRS_CONFIG_SMP
#ifndef VX_ENABLE_CPU_TIMEOUT
#   define VX_ENABLE_CPU_TIMEOUT   5   /* default timeout five seconds */
#endif  /* VX_ENABLE_CPU_TIMEOUT */
#endif /* _WRS_CONFIG_SMP */

/* locals */

LOCAL AIM_CPU_FUNCS cpuFuncs =
    {
    NULL,       /* cpuStart */
    NULL,       /* cpuShutdown */
    NULL,       /* cpuShow */
#ifdef _WRS_CONFIG_SMP
    NULL        /* cpuSmpLoop */
#endif
    };

/* globals */

BOARD_FUNC_TBL sysPlatformFuncTbl = {
    .earlyInit  = sysPlatformEarlyInit,
    .init       = sysPlatformInit,
    .reset      = sysPlatformReset,
    .model      = sysPlatformModel,
    .usDelay    = sysPlatformUsDelay,
#ifdef _WRS_CONFIG_SMP
    .cpuEn      = sysPlatformCpuEnable,
    .cpuAvail   = sysPlatformCpuAvailGet,
#endif /*_WRS_CONFIG_SMP*/
};

BOARD_FUNC_TBL * _pBspBoardFuncTbl = &sysPlatformFuncTbl;

LOCAL STATUS sysPlatformKwrite (char * buffer, size_t len);

/*******************************************************************************
*
* sysPlatformProbe - probe the board
*
* RETURNS: TRUE when probed, otherwise FALSE
*
* ERRNO: N/A
*/

LOCAL BOOL sysPlatformProbe
    (
    char * compat
    )
    {
    return TRUE;
    }

/*******************************************************************************
*
* sysPlatformEarlyInit - initialize board early
*
* This routine initializes the debug console, and obtains the CPU and physical
* memory information.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysPlatformEarlyInit ()
    {
    _func_kwrite = sysPlatformKwrite;
    }

/*******************************************************************************
*
* sysPlatformInit - initialize board
*
* This routine initializes various feature of BSP. It sets up the features that
* can only be enabled in usrRoot().
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysPlatformInit (void)
    {
    SYS_PLATFORM_MSG ("initialize board\n");

    sysUserSmInit ();

    aimCpuLibInit (&cpuFuncs);

    _func_cpuPwrIdleEnter = vxC1StateSet;
    }

/*******************************************************************************
*
* sysPlatformReset - reset board
*
* This routine resets the entire board
*
* RETURNS: Does not return.
*
* ERRNO: N/A
*/

LOCAL void sysPlatformReset
    (
    int startType
    )
    {
    if (*(char *) EXC_MSG_ADRS != EOS)
        {
        (void)vxsimHostPrintf ((char *)EXC_MSG_ADRS);

        /* indicate exception message is old */

        *(char *) EXC_MSG_ADRS = EOS;
        }

    vxsimHostReboot (startType);
    }

#ifdef INCLUDE_POSIX_CLOCKS
/******************************************************************************
*
* sysPlatformClockInit - initialize clock from host
*
* This routine is INCLUDE_VXSIM_POSIX_CLOCKS component init routine.
* It sets clock in order ctime() localtime() gmtime() returns accurate values.
*
* RETURNS: N/A
*/

void sysPlatformClockInit (void)
    {
    struct timespec curTmSpec;
    FUNCPTR tzOffsetGetRtn;
    long vxsimTZOffset = 0;

    /* get host timezone offset if possible */

    tzOffsetGetRtn = vxsimHostProcAddrGet ("vxsimTZOffsetGet");

    if (tzOffsetGetRtn != NULL)
        {
        vxsimTZOffset = (long)vxsimHostProcCall (tzOffsetGetRtn,
                                                 0,0,0,0,0,0,0,0,0);
        vxsimTZOffset *= -60;
        }

#ifdef INCLUDE_ENV_VARS
    if ((getenv ("TIMEZONE") != NULL) || (getenv ("TZ") != NULL))
        {
        /* time zone set in environment */

        vxsimTZOffset = 0;
        }
#endif

    curTmSpec.tv_sec = (time_t)((long)vxsimHostTimeGet() + vxsimTZOffset);
    curTmSpec.tv_nsec = 0;
    (void)clock_settime (CLOCK_REALTIME, &curTmSpec);
    }
#endif /* INCLUDE_POSIX_CLOCKS */

/*******************************************************************************
*
* sysPlatformModel - return the model name of board
*
* This routine returns the model name of the board.
*
* RETURNS: A pointer to the string.
*
* ERRNO: N/A
*/

LOCAL char * sysPlatformModel (void)
    {
#if CPU==SIMNT
    return("SIMNT board");
#elif CPU==SIMLINUX
    return("SIMLINUX board");
#endif
    }

/******************************************************************************
*
* sysPlatformKwrite - output buffer on debug console
*
* This routine outputs the content of the buffer on the debug console.
*
* RETURNS: OK, or ERROR if the parameter is wrong.
*
* ERRNO: N/A
*/

LOCAL STATUS sysPlatformKwrite
    (
    char * buffer,
    size_t len
    )
    {
    if (len == 0)
        return OK;

    if (buffer == NULL)
        return ERROR;

    (void)vxsimHostBufferPut (buffer, len);
    return OK;
    }

/*******************************************************************************
*
* sysPlatformUsDelay - delay execution for specified microseconds
*
* If host does not provide microsecond resolution (Windows),
* delay can be rounded.
*
* RETURNS: N/A
*/

LOCAL void sysPlatformUsDelay
    (
    int delay
    )
    {
    vxsimHostUsSleep ((UINT32)delay);
    }

#ifdef _WRS_CONFIG_SMP
/*******************************************************************************
*
* sysPlatformCpuStart - set up context to run specified function
*
* RETURNS: N/A
*/

void sysPlatformCpuStart
    (
    int cpu,            /* cpu to start */
    UCHAR * entry,  /* cpu entry point */
    ULONG stackPtr      /* cpu stack pointer */
    )
    {
    WIND_CPU_STATE cpuState;
    _Vx_usr_arg_t pArgs[2] = {1, 0};

    _sigCtxSetup (&cpuState.regs, (void *) stackPtr, (VOIDFUNCPTR)entry, pArgs);
    vxCpuStart (cpu, &cpuState);
    }

/* for backward compatibility */

_WRS_FUNC_ALIAS (sysPlatformCpuStart, sysCpuStart, void);

/*******************************************************************************
*
* sysPlatformCpuAvailGet - return the number of CPUs available for SMP
*
* This routine gets the number of CPUs available for SMP.
*
* RETURNS: the number of CPU cores.
*
* ERRNO: N/A
*/

LOCAL UINT32 sysPlatformCpuAvailGet (void)
    {
    SYS_PLATFORM_MSG ("probe CPU nums of cpu nodes\n");

    return vxCpuAvailableGet();
    }

/*******************************************************************************
*
* sysPlatformCpuEnable - enable a secondary CPU.
*
* This routine brings a secondary CPU out of reset.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS sysPlatformCpuEnable
    (
    unsigned int      physCpuIndex,    /* physical cpu index */
    WIND_CPU_STATE * pCpuState         /* The cpu state gived by kernel */
    )
    {
    SYS_PLATFORM_MSG ("kick kernel and enable secondary CPU %d\n",
                         physCpuIndex);

    return (vxsimHostCpuEnable (physCpuIndex, (INSTR *)pCpuState->regs.reg_pc,
                                pCpuState->regs.reg_sp));
    }
#endif /* _WRS_CONFIG_SMP */
