/* 30bsp.cdf - Windows BSP component description file */

/*
 * Copyright (c) 2014, 2018, 2021 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Selection BOARD_SELECTION {
    NAME        Board Selection
    COUNT       1-1
    CHILDREN    INCLUDE_WRS_VXSIM_WINDOWS
    DEFAULTS    INCLUDE_WRS_VXSIM_WINDOWS
    _CHILDREN   FOLDER_HIDDEN
}

Component INCLUDE_WRS_VXSIM_WINDOWS {
    NAME        Wind River VxWorks Simulator for Windows
    SYNOPSIS    This component provides support for the \
                Wind River VxWorks Simulator for Windows.
}

Parameter LOCAL_MEM_LOCAL_ADRS {
#ifdef _WRS_CONFIG_LP64
	DEFAULT		0x180000000
#else
	DEFAULT		0x10000000
#endif
}

Parameter RAM_LOW_ADRS {
#ifdef _WRS_CONFIG_LP64
	DEFAULT		0x180010000
#else
	DEFAULT		0x10010000
#endif
}

Parameter VM_PAGE_SIZE {
        DEFAULT         0x10000
}

Parameter DTB_RELOC_ADDR {
        DEFAULT         (RAM_LOW_ADRS-0x5000)
}

Parameter TIMESTAMP_TIMER_NAME {
    DEFAULT     (INCLUDE_TIMESTAMP64)::("vxsimTimestamp64") "vxsimTimestamp"
}
