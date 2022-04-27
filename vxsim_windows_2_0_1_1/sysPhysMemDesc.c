/* sysPhysMemDesc.c - Initial memory map descriptor included by sysLib.c */

/*
 * Copyright (c) 2014-2015, 2020-2021 Wind River Systems, Inc.
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
#include <vsbConfig.h>
#include <vmLib.h>
#include <bootLib.h>
#include <sysLib.h>
#if CPU==_VX_SIMLINUX
#include <arch/simlinux/mmuArchLib.h>
#elif CPU==_VX_SIMNT
#include <arch/simnt/mmuArchLib.h>
#endif
#include <private/vxsimHostLibP.h>
#include <prjParams.h>

/* defines */

#define NOT_INITIALIZED         0
#define DUMMY_PHYS_ADDR         -1
#define DUMMY_VIRT_ADDR         -1
#define DUMMY_LENGTH            -1
#define DUMMY_INIT_STATE_MASK   -1
#define DUMMY_INIT_STATE        -1

#define VXSIM_SM_CTRL_SIZE      MMU_PAGE_SIZE

#define USER_SM_POOL_SIZE       0
#define VXSIM_SM_SIZE           0
#undef  VXSIM_HAS_SHARED_MEM

#if     defined(INCLUDE_VXSIM_USER_SM) && defined(VXSIM_USER_SM_SIZE)
#if     (VXSIM_USER_SM_SIZE != 0)
#undef  USER_SM_POOL_SIZE
#define USER_SM_POOL_SIZE       ROUND_UP (VXSIM_USER_SM_SIZE, \
                                          MMU_PAGE_SIZE)
#define VXSIM_HAS_SHARED_MEM
#endif  /* (VXSIM_USER_SM_SIZE != 0) */
#endif  /* INCLUDE_VXSIM_USER_SM && VXSIM_USER_SM_SIZE */

#ifdef  VXSIM_HAS_SHARED_MEM
#undef  VXSIM_SM_SIZE
#define VXSIM_SM_SIZE           ROUND_UP (USER_SM_POOL_SIZE + \
                                          VXSIM_SM_CTRL_SIZE, \
                                          MMU_PAGE_TBL_SIZE)
#endif  /* VXSIM_HAS_SHARED_MEM */

#define SYS_RUP(x, align) (((unsigned long)(x) + ((unsigned long)align - 1)) & \
                           ~((unsigned long) align - 1))

/* globals */

BOOL    sysOverlapMemoryModel = TRUE;   /* required by host vxsim binary */
char *  sysMemBaseAdrs  = (char *) LOCAL_MEM_LOCAL_ADRS; /* base address */
size_t  sysMemSize      = LOCAL_MEM_SIZE;                /* memory size */

#if (CPU_FAMILY==SIMNT || CPU_FAMILY==SIMLINUX)
/*
 * Simulate i80x86 family cpu id to indicate floating point usage.
 * These values need to be coherent with FPREG_SET type and compiler options.
 * 32 bit simulator uses i387 fp registers.
 * 64 bit simulator supports additional xmm[0-15].
 */

CPUID sysCpuId =
    {
    0,                                                  /* highestValue */
    {0},                                                /* vendorId */
    0,                                                  /* signature */
    0,                                                  /* featuresEbx */
#ifdef _WRS_CONFIG_LP64
    VX_CPUID_XSAVE,                                        /* featuresEcx */
    VX_CPUID_MMX|VX_CPUID_FXSR|VX_CPUID_SSE|VX_CPUID_SSE2, /* featuresEdx */
#else /* _WRS_CONFIG_LP64 */
    0,                                                  /* featuresEcx */
    0,                                                  /* featuresEdx */
#endif /* _WRS_CONFIG_LP64 */
    0,                                                  /* cacheEax */
    0,                                                  /* cacheEbx */
    0,                                                  /* cacheEcx */
    0,                                                  /* cacheEdx */
    {0,1},                                              /* serialNo64 */
    {0}                                                 /* brandString */
    };
#endif /* (CPU_FAMILY==SIMNT || CPU_FAMILY==SIMLINUX) */

#ifdef _WRS_CONFIG_SMP
int     sysCpuConfigured = VX_SMP_NUM_CPUS;     /* indicate number of CPUs */
#endif /* _WRS_CONFIG_SMP */

#if (VXSIM_KERNEL_SYS_MEM_RGN_BASE % MMU_PAGE_TBL_SIZE != 0)
#error "VXSIM_KERNEL_SYS_MEM_RGN_BASE must be aligned on MMU_PAGE_TBL_SIZE"
#endif
VIRT_ADDR sysKernelSysMemRgnBase = VXSIM_KERNEL_SYS_MEM_RGN_BASE;

#if ((VXSIM_KERNEL_SYS_MEM_RGN_SIZE % MMU_PAGE_TBL_SIZE) != 0)
#error "VXSIM_KERNEL_SYS_MEM_RGN_SIZE must be aligned on MMU_PAGE_TBL_SIZE"
#endif
size_t sysKernelSysMemRgnSize = VXSIM_KERNEL_SYS_MEM_RGN_SIZE;

#if (VXSIM_KERNEL_SYS_MEM_RGN_BASE + VXSIM_KERNEL_SYS_MEM_RGN_SIZE > \
     VXSIM_RTP_PRIVATE_RGN_BASE)
#error "VXSIM_KERNEL_SYS_MEM_RGN overlaps VXSIM_RTP_PRIVATE_RGN"
#endif
#if (VXSIM_RTP_PRIVATE_RGN_BASE % MMU_PAGE_TBL_SIZE != 0)
#error "VXSIM_RTP_PRIVATE_RGN_BASE must be aligned on #MMU_PAGE_TBL_SIZE"
#endif

_Vx_VIRT_ADDR sysRtpPrivMemRgnBase = VXSIM_RTP_PRIVATE_RGN_BASE;

#ifndef _WRS_CONFIG_RTP
#   define VXSIM_RTP_PRIVATE_RGN_SIZE 0
#endif /* _WRS_CONFIG_RTP */

size_t sysRtpPrivMemRgnSize =  SYS_RUP(VXSIM_RTP_PRIVATE_RGN_SIZE,
                                       MMU_PAGE_TBL_SIZE);
_Vx_VIRT_ADDR sysSharedMemRgnBase = VXSIM_RTP_PRIVATE_RGN_BASE +
                                          SYS_RUP(VXSIM_RTP_PRIVATE_RGN_SIZE,
                                                  MMU_PAGE_TBL_SIZE);
size_t sysSharedMemRgnSize = SYS_RUP(VXSIM_SHARED_RGN_SIZE,
                                      MMU_PAGE_TBL_SIZE);
_Vx_VIRT_ADDR sysKernelVirtPoolMemRgnBase = VXSIM_RTP_PRIVATE_RGN_BASE +
                                          SYS_RUP(VXSIM_RTP_PRIVATE_RGN_SIZE,
                                                  MMU_PAGE_TBL_SIZE) +
                                          SYS_RUP (VXSIM_SHARED_RGN_SIZE,
                                                   MMU_PAGE_TBL_SIZE);
size_t sysKernelVirtPoolMemRgnSize =
                                SYS_RUP(VXSIM_KERNEL_VIRT_POOL_RGN_SIZE,
                                        MMU_PAGE_TBL_SIZE);

/* symbols looked for by host */

void *  simnetMemAdrs = 0;      /* let system decide */
#ifdef INCLUDE_NETWORK
UINT32  simnetMemSize = VXSIM_SIMNET_MEM_RGN_SIZE;
#else
UINT32  simnetMemSize = 0;
#endif

#ifdef VXSIM_HAS_SHARED_MEM
_Vx_VIRT_ADDR sysSmCtrlAddr;
void *  sysSmAddr = 0;
UINT32	sysSmSize = VXSIM_SM_SIZE - VXSIM_SM_CTRL_SIZE;

#ifdef USER_SM_POOL_SIZE
LOCAL void *    sysUserSmAddr = 0;
LOCAL size_t    sysUserSmSize = USER_SM_POOL_SIZE;      /* User SM size */
#endif /* USER_SM_POOL_SIZE */

int 	sysSmLevel;			/* SM_INT_ARG1 = shared mem level */
void *  sysSmAnchorAdrs	= 0x0;		/* SM_ANCHOR_ADRS */
void *  sysDshmAddr = 0x0;		/* dSHM base address */
void *	sysMipcSmAddr = 0x0;		/* MIPC base address */
#endif  /* VXSIM_HAS_SHARED_MEM */

/* locals */

LOCAL PHYS_MEM_DESC sysPhysRamDesc [] =
    {
        {
        /* Kernel system memory */

        (VIRT_ADDR) LOCAL_MEM_LOCAL_ADRS,
        (PHYS_ADDR) (LOCAL_MEM_LOCAL_ADRS),
        LOCAL_MEM_SIZE,         /* may be modified at runtime */
        VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
        (VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE)
        },
    };

LOCAL int sysPhysRamDescCount = NELEMENTS(sysPhysRamDesc);

#ifdef INCLUDE_USER_RESERVED_MEMORY
/*
 * sysUserMemDesc is used to provide user reserved memory
 * if INCLUDE_USER_RESERVED_MEMORY is defined.
 */

LOCAL PHYS_MEM_DESC sysUserMemDesc[] =
    {
        {
        (VIRT_ADDR) MEM_DESC_ADDR_KERNEL_ASSIGNED,  /* kernel assigned */
        (PHYS_ADDR) 0,  /* computed at runtime */
        0,              /* computed at runtime */
        (VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
         VM_STATE_MASK_CACHEABLE),
        (VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE)
        }
    };
LOCAL int sysUserMemDescCount = NELEMENTS(sysUserMemDesc);
#endif /* INCLUDE_USER_RESERVED_MEMORY */

#ifdef INCLUDE_EDR_PM
/*
 * sysPmMemDesc is used to provide persistent memory
 * if INCLUDE_EDR_PM is defined.
 */

LOCAL PHYS_MEM_DESC sysPmMemDesc[] =
    {
        {
        (VIRT_ADDR) MEM_DESC_ADDR_KERNEL_ASSIGNED,      /* kernel assigned */
        (PHYS_ADDR) 0,  /* computed at runtime */
        0,              /* computed at runtime */
        (VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE |
         VM_STATE_MASK_CACHEABLE),
        (VM_STATE_VALID | VM_STATE_WRITABLE | VM_STATE_CACHEABLE)
        }
    };
LOCAL int sysPmMemDescCount = NELEMENTS(sysPmMemDesc);
#endif /* INCLUDE_EDR_PM */

#if defined (INCLUDE_NETWORK)
/* global variables retrieved by VxSim before network initialization */

#endif /* INCLUDE_NETWORK */

/******************************************************************************
*
* sysMemDescGet - return memory descriptors describing memory layout
*
* This routine is used during system boot to describe the memory
* layout to the kernel and included components. It supports
* memory autoconfiguration. It also supports carving memory from
* the first contiguous block of RAM for 32-bit DMA and persistent
* memory. This routine assumes that enough memory is provided
* in the 0th descriptor to support all of these segments.
*
* RETURNS: OK, if a valid PHYS_MEM_DESC is copied to the caller.
*/

STATUS sysMemDescGet
    (
    MEM_DESC_TYPE	memDescType,	/* type of memory being queried */
    int			index,		/* index of memory block for the type */
    PHYS_MEM_DESC *	pMemDesc	/* where to return description */
    )
    {
    PHYS_MEM_DESC *	pDesc;
    int			descCount;

    if (memDescType == MEM_DESC_RAM)
    	{
	pDesc = sysPhysRamDesc;

	descCount = sysPhysRamDescCount;
	}
#ifdef INCLUDE_USER_RESERVED_MEMORY
    else if (memDescType == MEM_DESC_USER_RESERVED_RAM)
    	{
	pDesc = sysUserMemDesc;
	descCount = sysUserMemDescCount;
	}
#endif /* INCLUDE_USER_RESERVED_MEMORY */
#ifdef INCLUDE_EDR_PM
    else if (memDescType == MEM_DESC_PM_RAM)
    	{
	pDesc = sysPmMemDesc;
	descCount = sysPmMemDescCount;
	}
#endif /* INCLUDE_EDR_PM */
    else
    	return ERROR;	/* no support for MEM_DESC_APPL_RAM */

    if (index >= descCount)
    	{
	return ERROR;
	}

    if ((pDesc->len == (size_t)DUMMY_LENGTH) ||
    	(pDesc->physicalAddr == (PHYS_ADDR) DUMMY_PHYS_ADDR))
	return ERROR;

    /*
     * Copy the descriptor, note that it is OK to return a a descriptor that
     * has a length of 0. This allows the BSP to allocate "empty" entries
     * in the descriptor arrays, and fill them in at run-time without having
     * to alter the logic of this routine.
     */

    *pMemDesc = pDesc[index];
    return OK;
    }

/******************************************************************************
*
* sysMemDescInit - hook for memory auto sizing
*
* This routine is used to perform any adjustments/clean up prior to calling
* sysMemDescGet().
*
* RETURNS: N/A
*/

void sysMemDescInit (void)
    {
    LOCAL BOOL memFirstTime = TRUE;

    if (memFirstTime)
    	{
#if defined(INCLUDE_USER_RESERVED_MEMORY)||defined(INCLUDE_EDR_PM)
	PHYS_ADDR	physicalAddrEnd;
#endif
#ifdef INCLUDE_USER_RESERVED_MEMORY
        size_t		userSize = SYS_RUP(USER_RESERVED_MEM, MMU_PAGE_SIZE);
#else
	size_t		userSize = 0;
#endif /* INCLUDE_USER_RESERVED_MEMORY */
#ifdef INCLUDE_EDR_PM
	size_t		pmSize = SYS_RUP(PM_RESERVED_MEM, MMU_PAGE_SIZE);
#else
	size_t		pmSize = 0;
#endif /* INCLUDE_EDR_PM */

	/* shrink 0th segment by reserved sizes */

	sysPhysRamDesc[0].len = sysMemSize - userSize - pmSize;
#if defined(INCLUDE_USER_RESERVED_MEMORY)||defined(INCLUDE_EDR_PM)
	physicalAddrEnd = sysPhysRamDesc[0].physicalAddr +
			  sysPhysRamDesc[0].len;
#endif

#ifdef INCLUDE_USER_RESERVED_MEMORY
	/* set physical address and size for user reserved memroy */

	sysUserMemDesc[0].len = userSize;
	sysUserMemDesc[0].physicalAddr = physicalAddrEnd;
	physicalAddrEnd += sysUserMemDesc[0].len;
#endif /* INCLUDE_USER_RESERVED_MEMORY */

#ifdef INCLUDE_EDR_PM
	/* set physical address and size for persistent memroy */

	sysPmMemDesc[0].len = pmSize;
	sysPmMemDesc[0].physicalAddr = physicalAddrEnd;
	physicalAddrEnd += sysPmMemDesc[0].len;
#endif /* INCLUDE_EDR_PM */

	/* first time we are called we patch the descriptors */

	memFirstTime = FALSE;
	}
    }

/*****************************************************************************
*
* sysUserSmInit - initialize VxSim user shared memory
*
* RETURNS: N/A
*/

void sysUserSmInit (void)
    {
    static BOOL smIsInitialized = FALSE;

    if (!smIsInitialized)
        {
#ifdef  VXSIM_HAS_SHARED_MEM
        sysSmCtrlAddr = SYS_RUP ((sysKernelVirtPoolMemRgnBase +
                                  sysKernelVirtPoolMemRgnSize),
                                 MMU_PAGE_TBL_SIZE);
        sysSmCtrlAddr = (_Vx_VIRT_ADDR)vxsimHostShMemInit
                            ((char *)sysSmCtrlAddr, sysSmSize, &sysSmLevel);
        sysSmAddr = (char *)sysSmCtrlAddr + VXSIM_SM_CTRL_SIZE;
#ifdef USER_SM_POOL_SIZE
        sysUserSmAddr = sysSmAddr;
#endif /* USER_SM_POOL_SIZE */
#endif /* VXSIM_HAS_SHARED_MEM */
        smIsInitialized = TRUE;
        }
    }

#ifdef  VXSIM_HAS_SHARED_MEM
#ifdef USER_SM_POOL_SIZE
/*****************************************************************************
*
* sysUserSmInfoGet - get information about VxSim user shared memory
*
* This routine gets information (size and address) about the VxSim user
* shared memory. If no user shared memory is configured in the system,
* the size returned is set to 0.
*
* RETURNS: N/A
*/

void sysUserSmInfoGet
    (
    size_t *    pSize,  /* where to return user shared memory size in bytes */
    void **     pAddr   /* where to return user shared memory address */
    )
    {
    *pSize = sysUserSmSize;
    *pAddr = sysUserSmAddr;
    }
#endif /* USER_SM_POOL_SIZE */
#endif /* VXSIM_HAS_SHARED_MEM */
