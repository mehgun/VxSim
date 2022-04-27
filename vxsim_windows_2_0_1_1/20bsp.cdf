/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2014-2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Profile BSP_DEFAULT {
    PROFILES   PROFILE_OS_DEFAULT
    COMPONENTS += INCLUDE_PASSFS                \
                  INCLUDE_VIRTUAL_DISK          \
                  INCLUDE_TIMESTAMP             \
                  INCLUDE_SYS_TIMESTAMP         \
                  INCLUDE_ADR_SPACE_LIB         \
                  INCLUDE_FORMATTED_IO          \
                  INCLUDE_HW_FP                 \
                  INCLUDE_COPROCESSOR           \
                  INCLUDE_AUX_CLK               \
                  INCLUDE_COPROCESSOR_SHOW      \
                  INCLUDE_PASSFS_SYM_TBL        \
                  INCLUDE_STARTUP_SCRIPT        \
                  INCLUDE_STAT_SYM_TBL          \
                  INCLUDE_DISK_UTIL             \
                  INCLUDE_SHELL                 \
                  INCLUDE_SHELL_VI_MODE         \
                  INCLUDE_SHELL_EMACS_MODE      \
                  INCLUDE_SHELL_INTERP_C        \
                  INCLUDE_SHELL_INTERP_CMD      \
                  INCLUDE_SHOW_ROUTINES         \
                  INCLUDE_DEBUG                 \
                  INCLUDE_MAPPED_FILES_SHOW     \
                  INCLUDE_DATACOLLECTOR_SHOW    \
                  INCLUDE_ISR_SHOW              \
                  INCLUDE_FDT_SHOW              \
                  INCLUDE_POOL_SHOW             \
                  INCLUDE_VXBUS_SHOW            \
                  INCLUDE_MEM_SHOW              \
                  INCLUDE_SYM_TBL_SHOW          \
                  INCLUDE_LOADER                \
                  INCLUDE_UNLOADER              \
                  INCLUDE_SPY                   \
                  INCLUDE_CPLUS                 \
                  INCLUDE_CPLUS_LANG            \
                  INCLUDE_ROMFS                 \
                  INCLUDE_EDR_PM                \
                  INCLUDE_EDR_ERRLOG            \
                  INCLUDE_EDR_SHOW              \
                  INCLUDE_EDR_SYSDBG_FLAG       \
                  INCLUDE_KERNEL_HARDENING      \
                  INCLUDE_POSIX_AIO             \
                  INCLUDE_POSIX_AIO_SYSDRV      \
                  INCLUDE_POSIX_FTRUNC          \
                  INCLUDE_POSIX_MEM             \
                  INCLUDE_POSIX_MQ              \
                  INCLUDE_POSIX_PTHREADS        \
                  INCLUDE_POSIX_SCHED           \
                  INCLUDE_POSIX_SEM             \
                  INCLUDE_POSIX_SIGNALS         \
                  INCLUDE_POSIX_TIMERS          \
                  INCLUDE_POSIX_CLOCKS          \
                  INCLUDE_POSIX_DIRLIB          \
                  INCLUDE_END                   \
                  INCLUDE_DEBUG_AGENT           \
                  INCLUDE_DEBUG_AGENT_START     \
                  INCLUDE_STOP_MODE_AGENT       \
                  INCLUDE_STOP_MODE_AGENT_START \
                  INCLUDE_SYSTEMVIEWER_AGENT    \
                  INCLUDE_ANALYSIS_AGENT        \
                  INCLUDE_ANALYSIS_SCRIPT       \
                  INCLUDE_ANALYSIS_DEBUG_SUPPORT \
                  INCLUDE_RTP                   \
                  INCLUDE_SHL                   \
                  INCLUDE_SHL_SHOW              \
                  INCLUDE_SHARED_DATA           \
                  INCLUDE_RTP_APPL_INIT_BOOTLINE \
                  INCLUDE_SC_POSIX              \
                  INCLUDE_TRIGGERING            \
                  INCLUDE_WINDVIEW              \
                  INCLUDE_WVUPLOAD_FILE         \
                  INCLUDE_CACHE_SUPPORT         \
                  INCLUDE_CACHE_ENABLE          \
                  INCLUDE_SMP_DEMO              \
                  INCLUDE_NV_RAM                \
                  INCLUDE_NET_DRV               \
                  INCLUDE_SIO                   \
                  INCLUDE_RAM_DISK              \
                  INCLUDE_RAM_DISK_FORMAT_ANY   \
                  INCLUDE_DOSFS
}

Profile PROFILE_DEVELOPMENT {
    COMPONENTS -= INCLUDE_NET_SYM_TBL
}

Profile PROFILE_STANDALONE_DEVELOPMENT {
    PROFILES_EXCLUSIONS += INCLUDE_PASSFS_SYM_TBL
}

/* BSP specific component to initialize devices specified through -ni option */

Component INCLUDE_VXSIM_IFCONFIG {
        NAME            initialize VxSim additional network interfaces
        _CHILDREN       FOLDER_BSP_CONFIG
        _INIT_ORDER     usrNetworkInit
        INIT_AFTER      INCLUDE_IPCOM
        REQUIRES        INCLUDE_IPWRAP_IFCONFIG \
                        INCLUDE_IPWRAP_IPPROTO \
                        INCLUDE_IPCOM
        _REQUIRES       INCLUDE_IPNET
        INIT_RTN        sysNetIfConfig ();
}

Component INCLUDE_NV_RAM {
        NAME            VxSim NVRAM support
        CFG_PARAMS      NV_RAM_SIZE
        _FOLDER         FOLDER_SUBSYSTEM
}

Component INCLUDE_USER_RESERVED_MEMORY {
        NAME            user-reserved memory.
        SYNOPSIS        user-reserved memory
        _CHILDREN       FOLDER_MEMORY
        HDR_FILES       private/userReservedMemP.h
        MODULES         userReservedMem.o
        INIT_RTN        userReservedMemInit (USER_RESERVED_MEM, \
                                       CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT);
#ifdef _WRS_CONFIG_USE_MEMDESC
        CFG_PARAMS      USER_RESERVED_MEM CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT
#else /* !_WRS_CONFIG_USE_MEMDESC */
        CFG_PARAMS      CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT
#endif /* _WRS_CONFIG_USE_MEMDESC */
}

Component INCLUDE_VXSIM_POSIX_CLOCKS {
        Name            VxSim clock initialization from host clock
        SYNOPSIS        Synchronize VxSim clock with host one
        REQUIRES        INCLUDE_POSIX_CLOCKS
        INCLUDE_WHEN    INCLUDE_POSIX_CLOCKS
        _INIT_ORDER     usrKernelExtraInit
        _CHILDREN       FOLDER_BSP_CONFIG
	PROTOTYPE	extern void sysPlatformClockInit (void);
        INIT_RTN        sysPlatformClockInit ();
}

Component INCLUDE_CPU_INIT {
        _CHILDREN       FOLDER_NOT_VISIBLE
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_MEM_DESC {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}

Component DRV_SYS_CON {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}
Component DRV_SYS_CON_POWEROFF {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}
Component DRV_SYS_CON_REBOOT {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}
Component INCLUDE_REGMAP_SYS {
        REQUIRES        COMPONENT_NOT_SUPPORTED
}


Parameter CLEAR_USER_RESERVED_MEMORY_ON_COLD_BOOT {
        NAME            clear user-reserved memory on reboot
        SYNOPSIS        Clears user-reserved memory on cold reboot
        DEFAULT         TRUE
}

Parameter USER_RESERVED_MEM {
    NAME        user reserved Memory
    SYNOPSIS    Reserved memory size ( bytes)
    DEFAULT     0x0
}

/*
 * VX_SMP_NUM_CPUS is a SMP parameter only and only available for SMP
 * builds. Due to a limitation of the project tool at the time this
 * parameter is created where the tool can not recognize the ifdef SMP
 * selection, this parameter is set up such that _CFG_PARAMS is not
 * specified here. In the 00vxWorks.cdf file, where the parameter
 * VX_SMP_NUM_CPUS is defined, the _CFG_PARAMS is specified only for
 * VxWorks SMP. Hence the redefining of VX_SMP_NUM_CPUS here should only
 * override the value and not the rest of the properties. And for UP, the
 * parameter is ignored since the parameter is not tied to any component
 * (_CFG_PARAMS is not specified).
 */

Parameter VX_SMP_NUM_CPUS {
	NAME		Number of CPUs available to be enabled for VxWorks SMP
	TYPE		UINT
	DEFAULT		2
}

Parameter LOCAL_MEM_SIZE {
        NAME        Local memory size
        SYNOPSIS    Fixed (static) memory size
#ifdef _WRS_CONFIG_LP64
        DEFAULT         0x10000000
#else
        DEFAULT         0x08000000
#endif /* _WRS_CONFIG_LP64 */
}

/*
 * RAM_HIGH_ADRS is not used for simulators but it must be defined for
 * the build mechanism.
 */

Parameter RAM_HIGH_ADRS {
	NAME		Runtime kernel load address
#ifdef _WRS_CONFIG_LP64
	DEFAULT		0xffffffffffffffff
#else
	DEFAULT		0xffffffff
#endif /* _WRS_CONFIG_LP64 */
}

Parameter DTS_FILE {
        NAME    DTS file name to be used
        DEFAULT vxsim.dts
}

/* overwrite default <sysConsoleBaudRate> */

Parameter CONSOLE_BAUD_RATE {
        DEFAULT 115200
}

/* specify timers and set default timer values */

Parameter SYSCLK_TIMER_NAME {
    DEFAULT         "vxsimClk"
}

Parameter SYS_CLK_RATE   {
    DEFAULT     50
}

Parameter SYS_CLK_RATE_MIN {
    DEFAULT     1
}

Parameter SYS_CLK_RATE_MAX {
    DEFAULT     100
}

Parameter AUXCLK_TIMER_NAME {
    DEFAULT         "vxsimAuxClk"
}

Parameter AUX_CLK_RATE_MIN {
    DEFAULT     1
}

Parameter TIMESTAMP_TIMER_NAME {
    DEFAULT     "vxsimTimestamp"
}

Parameter NV_RAM_SIZE {
   NAME         Size of NVRAM
   TYPE         UINT
   DEFAULT      0x100
}

Parameter RAM_DISK_SIZE {
   DEFAULT      0x800000
}

Parameter BOOT_LINE_OFFSET {
   DEFAULT      0x0
}

Parameter BOOT_LINE_SIZE {
   DEFAULT      1500
}

Parameter EXC_MSG_OFFSET {
   DEFAULT      0xA20
}

Parameter EXC_MSG_ADRS {
   NAME         exception message address
   SYNOPSIS     Exception message address
   DEFAULT      (LOCAL_MEM_LOCAL_ADRS + EXC_MSG_OFFSET)
}

Parameter EXC_MSG_SIZE {
   NAME         exception message size
   SYNOPSIS     Exception mesage address
   DEFAULT      (DTB_RELOC_ADDR - EXC_MSG_ADRS)
}

Parameter PM_RESERVED_MEM {
   DEFAULT      0x400000
}

Parameter EDR_ERRLOG_SIZE {
    DEFAULT         (pmFreeSpace (EDR_PM_ARENA) / 4)
}

Parameter KERNEL_APPL_CORE_DUMP_ENABLE {
   DEFAULT      TRUE
}
